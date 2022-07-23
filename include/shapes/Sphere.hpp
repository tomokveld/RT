#ifndef Sphere_hpp
#define Sphere_hpp

#include "Shape.hpp"

class Sphere : public Shape {
public:
    Sphere();
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
    void UVMappedPoint(const Tuple &p, float *u, float *v) const override;
};

#endif /* Sphere_hpp */
