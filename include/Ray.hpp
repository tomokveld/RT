#ifndef Ray_hpp
#define Ray_hpp

#pragma once

#include "Intersection.hpp"
#include "Matrix.hpp"
#include "Tuple.hpp"
#include "World.hpp"

#include <vector>

class Ray {
public:
    Ray(const Tuple &origin, const Tuple &direction);
    Tuple get_origin() const;
    Tuple get_direction() const;
    Tuple position(float t) const;
    friend Ray operator*(const Matrix<4,4> &m, const Ray &r);
    void intersect(const ShapePtr &shape, std::vector<Intersection>& xs) const;
    void intersect(const World &world, std::vector<Intersection>& xs) const;
    IntersectionComp prepare_computations(const Intersection &i, const std::vector<Intersection> &xs = {}) const;
private:
    Tuple origin; // x0
    Tuple direction; // n
};

#endif /* Ray_hpp */
