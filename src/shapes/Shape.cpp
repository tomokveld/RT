#include "Shape.hpp"

Shape::Shape() :
    transform(Matrix<4, 4>::identity()),
    transform_inv(Matrix<4, 4>::identity()),
    material(),
    parent(),
    bounds(),
    bounds_transform()
{}

ShapePtr Shape::get_parent() {
    return parent;
}

void Shape::set_parent(ShapePtr parent_n) {
    parent = parent_n;
}

void Shape::set_bounds(const Bounds &b) {
    bounds = b;
}

const Bounds& Shape::get_bounds() const {
    return bounds;
}

Bounds& Shape::mod_bounds() {
    return bounds;
}

Matrix<4, 4> const& Shape::get_transform() const {
    return transform;
}

Matrix<4, 4> const& Shape::get_transform_inv() const {
    return transform_inv;
}

void Shape::set_transform(const Matrix<4, 4> &t) {
    transform = t;
    bounds_transform = bounds * transform;
    transform_inv = t.inverse();
}

Tuple Shape::normal_at(const Tuple &p, const Intersection &i) const {
    Tuple object_p = world_to_object(p);
    Tuple object_n = normal_at_local(object_p, i);
    return normal_to_world(object_n);
}

Tuple Shape::normal_to_world(const Tuple &n) const {
    Tuple world_n = transform_inv.transposed() * n;
    world_n[3] = 0.0f;
    world_n = world_n.normalize();
    if (parent != nullptr) {
        world_n = parent->normal_to_world(world_n);
    }
    return world_n;
}

Tuple Shape::world_to_object(const Tuple &world_p) const {
    // TODO: Add overload for operator= to reduce allocation
    Tuple p = world_p;
    if (parent != nullptr) {
        p = parent->world_to_object(world_p);
    }
    return transform_inv * p;
}

const Material& Shape::get_material() const {
    return material;
}

void Shape::set_material(const Material &m) {
    material = m;
}

Material& Shape::mod_material() {
    return material;
}

bool Shape::is_equal(const Shape &rhs) const {
    // TODO: Expand
    return material == rhs.get_material() &&
           transform == rhs.get_transform() &&
           transform_inv == rhs.get_transform_inv();
}

bool Shape::operator==(const Shape &rhs) const {
    return is_equal(rhs);
}

void Shape::divide(int threshold) {
    ;
}

bool Shape::includes(const ShapePtr &s) const {
    // TODO: Check this!
    return *this == *s;
}

void Shape::UVMappedPoint(const Tuple &p, float *u, float *v) const {
    ;
}
