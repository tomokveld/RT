#include "Sphere.hpp"

Sphere::Sphere() {
    bounds = Bounds(Point(-1.0f, -1.0f, -1.0f), Point(1.0f, 1.0f, 1.0f));
    bounds_transform = bounds;
}

void Sphere::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    Tuple ro = r.get_origin();
    ro[3] = 0.0f;
    Tuple rd = r.get_direction();

    const float a = rd.dot(rd); // dx * dx + dy * dy + dz * dz
    const float b = 2.0f * rd.dot(ro); // 2.0 * (dx * ox + dy * oy + dz * oz)
    const float c = ro.dot(ro) - 1.0f; // ox * ox + oy * oy + oz * oz - 1.0

    float t0, t1;
    if (!solve_quadratic(a, b, c, &t0, &t1))
        return;

    xs.push_back({t0, shared_from_this()});
    xs.push_back({t1, shared_from_this()});
}

// The normal of a point can be computed by subtracting the point position to the sphere center, since our spheres are centered at (0, 0, 0), we can just take the point
Tuple Sphere::normal_at_local(const Tuple &p, const Intersection &i) const {
//    return (p - Point(0, 0, 0)).normalize();
    return {p[0], p[1], p[2], 0.0f};
}

bool Sphere::operator==(const Shape &rhs) const {
    const Sphere *rhs_sphere = dynamic_cast<const Sphere*>(&rhs);
    return rhs_sphere && Shape::operator==(rhs);
}

void Sphere::UVMappedPoint(const Tuple &p, float *u, float *v) const {
    float theta = atan2(p.get_x(), p.get_z());
    Tuple vec = Vector(p[0], p[1], p[2]);
    float radius = vec.magnitude();
    float phi = acos(p.get_y() / radius);
    float raw_u = theta / (2.0f * M_PI);
    *u = 1.0f - (raw_u + 0.5f);
    *v = 1.0f - (phi / M_PI);
}

