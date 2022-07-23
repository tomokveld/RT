#include "Disk.hpp"

Disk::Disk() :
    Disk(1.0f, 0.0f, 360.0f, 0.0f)
{
    bounds = Bounds(Point(-radius, height, -radius), Point(radius, height, radius));
    bounds_transform = bounds;
}

Disk::Disk(float radius, float inner_radius, float phi_max, float height) :
    radius(radius),
    inner_radius(inner_radius),
    phi_max(radians(fclamp(phi_max, 0.0f, 360.0f))),
    height(height)
{
    bounds = Bounds(Point(-radius, height, -radius), Point(radius, height, radius));
    bounds_transform = bounds;
}

void Disk::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    // Reject intersections for rays that are parallel to the disk's plane
    if (fabs(r.get_direction().get_y()) < EPSILON)
        return;
    
    const float t = (height - r.get_origin().get_y()) / r.get_direction().get_y(); // tShapeHit
    if (t < EPSILON)
        return;

    const float x = r.get_origin().get_x() + (t * r.get_direction().get_x()); // pHit.x
    const float z = r.get_origin().get_z() + (t * r.get_direction().get_z()); // pHit.y
    const float dist2 = (x * x) + (z * z);
    if (dist2 > (radius * radius + EPSILON) || dist2 < inner_radius * inner_radius)
        return;

    float phi = std::atan2(z, x);
    if (phi < 0.0f)
        phi += 2.0f * M_PI;
    if (phi > phi_max)
        return;
    
    xs.push_back({t, shared_from_this()});
}

Tuple Disk::normal_at_local(const Tuple &p, const Intersection &i) const {
    return Vector(0.0f, 1.0f, 0.0f);
}

bool Disk::operator==(const Shape &rhs) const {
    const Disk *rhs_disk = dynamic_cast<const Disk*>(&rhs);
    return rhs_disk && Shape::operator==(rhs);
}



