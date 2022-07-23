#include "Cube.hpp"

Cube::Cube() {
    bounds = Bounds(Point(-1.0f, -1.0f, -1.0f), Point(1.0f, 1.0f, 1.0f));
    bounds_transform = bounds;
}

//void Cube::intersect(const Ray &r, std::vector<Intersection> &xs) const {
//    float xtmin, xtmax, ytmin, ytmax, ztmin, ztmax;
//    check_axis(r.get_origin().get_x(), r.get_direction().get_x(), &xtmin, &xtmax);
//    check_axis(r.get_origin().get_y(), r.get_direction().get_y(), &ytmin, &ytmax);
//    check_axis(r.get_origin().get_z(), r.get_direction().get_z(), &ztmin, &ztmax);
//
//    float tmin = std::max({xtmin, ytmin, ztmin});
//    float tmax = std::min({xtmax, ytmax, ztmax});
//
//    // Ray is intersecting but the cube is behind us or does not intersect
//    if (tmax < 0.0f || tmin > tmax)
//        return;
//
//    xs.push_back({tmin, shared_from_this()});
//    xs.push_back({tmax, shared_from_this()});
//}


void Cube::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    Tuple o = r.get_origin();
    Tuple d = Vector(1.0/r.get_direction().get_x(),
                     1.0/r.get_direction().get_y(),
                     1.0/r.get_direction().get_z());

    double t1 = (-1 - o[0]) * d[0];
    double t2 = (1 - o[0]) * d[0];

    double tmin = std::min(t1, t2);
    double tmax = std::max(t1, t2);

    for (int i = 1; i < 3; i++) {
            double t1 = (-1 - o[i]) * d[i];
            double t2 = (1 - o[i]) * d[i];
            tmin = std::max(tmin, std::min(t1, t2));
            tmax = std::min(tmax, std::max(t1, t2));
        }
    if (tmax > std::max(tmin, 0.0)) {
        xs.push_back({static_cast<float>(tmin), shared_from_this()});
        xs.push_back({static_cast<float>(tmax), shared_from_this()});
    }
}

Tuple Cube::normal_at_local(const Tuple &p, const Intersection &i) const {
    const float x = p.get_x(), y = p.get_y(), z = p.get_z();
    const float maxc = std::max({abs(x), abs(y), abs(z)});
    if (maxc == abs(x))
        return Vector(x, 0.0f, 0.0f);
    else if (maxc == abs(y))
        return Vector(0.0f, y, 0.0f);
    else
        return Vector(0.0f, 0.0f, z);
}

bool Cube::operator==(const Shape &rhs) const {
    const Cube *rhs_cube = dynamic_cast<const Cube*>(&rhs);
    return rhs_cube && Shape::operator==(rhs);
}

void Cube::UVMappedPoint(const Tuple &p, float *u, float *v) const {
    int face = p.face_from_point();
    switch (face) {
        case Faces::right:
            *u = fmod((1.0f - p[2]), 2.0f) / 2.0f;
            *v = fmod((p[1] + 1.0f), 2.0f) / 2.0f;
            break;
        case Faces::left:
            *u = fmod((p[2] + 1.0f), 2.0f) / 2.0f;
            *v = fmod((p[1] + 1.0f), 2.0f) / 2.0f;
            break;
        case Faces::up:
            *u = fmod((p[0] + 1.0f), 2.0f) / 2.0f;
            *v = fmod((1.0f - p[2]), 2.0f) / 2.0f;
            break;
        case Faces::down:
            *u = fmod((p[0] + 1.0f), 2.0f) / 2.0f;
            *v = fmod((p[2] + 1.0f), 2.0f) / 2.0f;
            break;
        case Faces::front:
            *u = fmod((p[0] + 1.0f), 2.0f) / 2.0f;
            *v = fmod((p[1] + 1.0f), 2.0f) / 2.0f;
            break;
        case Faces::back:
            *u = fmod((1.0f - p[0]), 2.0f) / 2.0f;
            *v = fmod((p[1] + 1.0f), 2.0f) / 2.0f;
            break;
    }
}
