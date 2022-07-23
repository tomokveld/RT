#ifndef testHelper_hpp
#define testHelper_hpp

#include "catch.hpp"
#include "Shape.hpp"
#include "Tuple.hpp"
#include "Sphere.hpp"
#include "Cube.hpp"
#include "Pattern.hpp"
#include "PointLight.hpp"

class TestShape : public Shape {
public:
    TestShape();
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
    Ray local_ray;
};


class TestPattern : public Pattern {
public:
    TestPattern();
    Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const override;
};


World default_world();
ShapePtr glass_sphere();
ShapePtr MappedCube();

#endif /* testHelper_hpp */

