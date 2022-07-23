#include "Cylinder.hpp"

Cylinder::Cylinder() : Cylinder(-INF, INF, false) {}

Cylinder::Cylinder(Tuple p1, Tuple p2) : Cylinder(0.0f, 1.0f, true) {
    auto v = p2 - p1;
    auto m = Transform::rotation(Vector(0.0f, 1.0f, 0.0f), v.normalize());
    set_transform(Transform::translation(p1[0], p1[1], p1[2]) *
                  m * Transform::scaling(1.0f, v.magnitude(), 1.0f));
}

Cylinder::Cylinder(float min, float max, bool capped) :
    min(min),
    max(max),
    capped(capped)
{
        bounds = Bounds(Point(-1.0f, min, -1.0f), Point(1.0f, max, 1.0f));
        bounds_transform = bounds;
}

float Cylinder::get_min() const {
    return min;
}

float Cylinder::get_max() const {
    return max;
}

bool Cylinder::get_capped() const {
    return capped;
}

float Cylinder::get_radius(const float y) const {
    return 1.f;
}

bool Cylinder::check_cap(const Ray &r, const float t, const float rd) const {
    const float x = r.get_origin().get_x() + t * r.get_direction().get_x();
    const float z = r.get_origin().get_z() + t * r.get_direction().get_z();
    return ((x * x) + (z * z)) <= (rd * rd + EPSILON);
}

void Cylinder::intersect_caps(const Ray &r, std::vector<Intersection> &xs) const {
    const Tuple r_origin = r.get_origin();
    const Tuple r_direction = r.get_direction();

    // Caps only matter if the cylinder is closed, and might possibly be intersected by the ray.
    if (!capped || fabs(r_direction.get_y()) < EPSILON)
        return;

    // Check for an intersection with the lower end cap by intersecting the ray with the plane at y=cyl.minimum
    float t = (min - r_origin.get_y()) / r_direction.get_y();
    if (check_cap(r, t, get_radius(min)))
        xs.push_back({t, shared_from_this()});

    // Check for an intersection with the upper end cap by intersecting the ray with the plane at y=cyl.maximum
    t = (max - r_origin.get_y()) / r_direction.get_y();
    if (check_cap(r, t, get_radius(max)))
        xs.push_back({t, shared_from_this()});
}

void Cylinder::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    const Tuple ro = r.get_origin();
    const Tuple rd = r.get_direction();

    const float a = (rd.get_x() * rd.get_x()) +
                    (rd.get_z() * rd.get_z());

    if (fabs(a) < EPSILON) {
        intersect_caps(r, xs);
        return;
    }

    const float b = 2.0f * ro.get_x() * rd.get_x() +
                    2.0f * ro.get_z() * rd.get_z();

    const float c = (ro.get_x() * ro.get_x()) +
                    (ro.get_z() * ro.get_z()) - 1.0f;

    float t0, t1;
    if (!solve_quadratic(a, b, c, &t0, &t1))
        return;

    // TODO: Combine into inlined function
    const float y0 = ro.get_y() + t0 * rd.get_y();
    if (min < y0 && y0 < max)
        xs.push_back({t0, shared_from_this()});

    const float y1 = ro.get_y() + t1 * rd.get_y();
    if (min < y1 && y1 < max)
        xs.push_back({t1, shared_from_this()});

    intersect_caps(r, xs);
}

// TODO: The disproportionate amount of shadow acne seems to be caused here.
Tuple Cylinder::normal_at_local(const Tuple &p, const Intersection &i) const {
    const double dist = (p.get_x() * p.get_x()) + (p.get_z() * p.get_z());
    if (dist < 1.0 && (max - EPSILON) <= p.get_y())
        return Vector(0.0f, 1.0f, 0.0f);
    else if (dist < 1.0 && (min + EPSILON) >= p.get_y())
        return Vector(0.0f, -1.0f, 0.0f);
    else
        return Vector(p.get_x(), 0.0f, p.get_z());
}

bool Cylinder::operator==(const Shape &rhs) const {
    const Cylinder *rhs_cylinder = dynamic_cast<const Cylinder*>(&rhs);
    return rhs_cylinder && Shape::operator==(rhs);
}

void Cylinder::UVMappedPoint(const Tuple &p, float *u, float *v) const {
    float theta, raw_u;
    size_t face = max - EPSILON <= p[1] ? 1 : min + EPSILON >= p[1] ? 2 : 0;

    switch (face) {
        case 0: // cylinder body
            theta = atan2(p[0], p[2]);
            raw_u = theta / (2.0f * M_PI);
            *u = 1.0f - (raw_u + 0.5f);
            *v = fmod(p[1], 1.0);
            break;
        case 1: // top cap
            *u = fmod((p[0] + 1.0f), 2.0f) / 2.0f;
            *v = fmod((1.0f - p[2]), 2.0f) / 2.0f;
            break;
        case 2: // bottom cap
            *u = fmod((p[0] + 1.0f), 2.0f) / 2.0f;
            *v = fmod((p[2] + 1.0f), 2.0f) / 2.0f;
            break;
        }
}
