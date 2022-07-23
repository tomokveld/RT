#include "Cone.hpp"

Cone::Cone() : Cone(-INF, INF, false) {}

Cone::Cone(float min, float max, bool capped) : Cylinder(min, max, capped) {
    const float limit = std::max(std::abs(min), std::abs(max));
    bounds = Bounds(Point(-limit, min, -limit), Point(limit, max, limit));
    bounds_transform = bounds;
}

float Cone::get_radius(const float y) const {
    return std::fabs(y);
}

void Cone::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    const Tuple r_origin = r.get_origin();
    const Tuple r_direction = r.get_direction();

    const float a = (r_direction.get_x() * r_direction.get_x()) -
                    (r_direction.get_y() * r_direction.get_y()) +
                    (r_direction.get_z() * r_direction.get_z());

    const float b = 2.0f * ((r_origin.get_x() * r_direction.get_x()) -
                    (r_origin.get_y() * r_direction.get_y()) +
                    (r_origin.get_z() * r_direction.get_z()));

    const float c = (r_origin.get_x() * r_origin.get_x()) -
                    (r_origin.get_y() * r_origin.get_y()) +
                    (r_origin.get_z() * r_origin.get_z());

    // TODO: Using less than zero gives unexpected results in the tests
//      if (fabs(a) < 0) {  //    if (abs(a) < EPSILON) {
//          if (fabs(b) > 0) {  //        if (abs(b) >= EPSILON) {
    if (fabs(a) < EPSILON) {
        if (fabs(b) >= EPSILON) {
            const float t = -c / (2.0f * b);
            xs.push_back({t, shared_from_this()});
            intersect_caps(r, xs);
        }
        return;
    }
    float t0, t1;
    if (!solve_quadratic(a, b, c, &t0, &t1))
        return;

    const float y0 = r_origin.get_y() + t0 * r_direction.get_y();
    if (min < y0 && y0 < max)
        xs.push_back({t0, shared_from_this()});

    const float y1 = r_origin.get_y() + t1 * r_direction.get_y();
    if (min < y1 && y1 < max)
        xs.push_back({t1, shared_from_this()});

    intersect_caps(r, xs);
}

Tuple Cone::normal_at_local(const Tuple &p, const Intersection &i) const {
    float y = std::sqrtf((p.get_x() * p.get_x()) + (p.get_z() * p.get_z()));
    if (p.get_y() > 0.0f)
        y = -y;
    if (y <= (max * max) && p.get_y() >= (max - EPSILON))
        return Vector(0.0f, 1.0f, 0.0f);
    else if (y <= (min * min) && p.get_y() <= (min + EPSILON))
        return Vector(0.0f, -1.0f, 0.0f);
    else
        return Vector(p.get_x(), y, p.get_z());
}

bool Cone::operator==(const Shape &rhs) const {
    const Cone *rhs_cone = dynamic_cast<const Cone*>(&rhs);
    return rhs_cone && Shape::operator==(rhs);
}
