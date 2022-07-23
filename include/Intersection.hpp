#ifndef Intersection_hpp
#define Intersection_hpp

#pragma once

#include "Types.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Helper.hpp"

#include <vector>

struct IntersectionComp {
    ShapeConstPtr object;
    Tuple over_point;
    Tuple under_point;
    Tuple point;
    Tuple eyev;
    Tuple normalv;
    Tuple reflectv;
    float n1;
    float n2;
    float distance;
    bool inside;
};

class Intersection {
public:
    Intersection(float distance, const ShapeConstPtr &s);
    Intersection(float distance, const ShapeConstPtr &s, float u, float v);
    float get_distance() const;
    ShapeConstPtr get_shape() const;
    float u() const;
    float v() const;
    friend bool operator==(const Intersection &lhs, const Intersection &rhs);
    friend bool operator<(const Intersection &lhs, const Intersection &rhs);
private:
    ShapeConstPtr shape;
    float distance;
    float u_;
    float v_;
};

Intersection Hit(const std::vector<Intersection> &xs);

template<typename ...Args>
std::vector<Intersection> Intersections(Args const&... args) {
    std::vector<Intersection> xs{ args... };
    std::sort(xs.begin(), xs.end());
    return xs;
}

float Schlick(const IntersectionComp &comps);
float Fresnel(const IntersectionComp &comps);

#endif /* Intersection_hpp */
