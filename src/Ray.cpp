#include "Ray.hpp"
#include "Shape.hpp" // TODO: Fix imports

Ray::Ray(const Tuple &origin, const Tuple &direction) : origin{origin}, direction{direction} { }

void Ray::intersect(const ShapePtr &shape, std::vector<Intersection>& xs) const {
    Ray const r = shape->get_transform_inv() * (*this);
    shape->intersect(r, xs);
}

void Ray::intersect(const World &world, std::vector<Intersection>& xs) const {
    for (const auto &shape : world.get_objects()) {
        intersect(shape, xs);
    }
    std::sort(xs.begin(), xs.end());
}

Tuple Ray::get_origin() const {
    return origin;
}

Tuple Ray::get_direction() const {
    return direction;
}

Tuple Ray::position(float t) const {
    return origin + (direction * t);
}

Ray operator*(const Matrix<4, 4> &m, const Ray &r) {
    return Ray{m * r.get_origin(), m * r.get_direction()};
}

IntersectionComp Ray::prepare_computations(const Intersection &i, const std::vector<Intersection> &xs) const {
    IntersectionComp comps;
    comps.distance = i.get_distance();
    comps.object = i.get_shape();
    comps.point = position(comps.distance);
    comps.eyev = -direction;
    comps.normalv = comps.object->normal_at(comps.point, i);

    // TODO: Add epsilon?
    if (comps.normalv.dot(comps.eyev) < 0.0f) {
        comps.inside = true;
        comps.normalv = -comps.normalv;
    } else {
        comps.inside = false;
    }

    // TODO: Shadow bias is a constant pain
    comps.over_point = comps.point + (comps.normalv * SHADOW_BIAS);
    comps.under_point = comps.point - (comps.normalv * SHADOW_BIAS);
    comps.reflectv = direction.reflect(comps.normalv);

    std::vector<ShapeConstPtr> containers;
    for (const auto &si : xs) {
        const bool i_is_hit = si == i;
        if (i_is_hit) {
            if (containers.empty()) {
                comps.n1 = 1.0f;
            } else {
                comps.n1 = containers.back()->get_material().get_refractive_index();
            }
        }

        auto it = std::find(containers.begin(), containers.end(), si.get_shape());
        if (it != containers.end()) {
            containers.erase(it);
        } else {
            containers.push_back(si.get_shape());
        }

        if (i_is_hit) {
            if (containers.empty()) {
                comps.n2 = 1.0f;
            } else {
                comps.n2 = containers.back()->get_material().get_refractive_index();
                break;
            }
        }
    }
    return comps;
}
