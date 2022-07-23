#include "Plane.hpp"

Plane::Plane() {
    bounds = Bounds(Point(-INF, 0.0f, -INF), Point(INF, 0.0f, INF));
    bounds_transform = bounds;
}

void Plane::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    const float dir_y = r.get_direction().get_y();

    if (abs(dir_y) < EPSILON)
        return;

    const float t = -r.get_origin().get_y() / dir_y;
    xs.push_back(Intersection(t, shared_from_this()));
}

Tuple Plane::normal_at_local(const Tuple &p, const Intersection &i) const {
    return Vector(0.0f, 1.0f, 0.0f);
}

bool Plane::operator==(const Shape &rhs) const {
    const Plane *rhs_plane = dynamic_cast<const Plane*>(&rhs);
    return rhs_plane && Shape::operator==(rhs);
}

void Plane::UVMappedPoint(const Tuple &p, float *u, float *v) const {
    float _u = fmod(p[0], 1.0f);
    float _v = fmod(p[2], 1.0f);

    if (_u < 0.0f)
        _u += 1.0f;
    if (_v < 0.0f)
        _v += 1.0f;

    *u = _u;
    *v = _v;
}
