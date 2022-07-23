#ifndef Plane_hpp
#define Plane_hpp

#include "Shape.hpp"

class Plane : public Shape {
public:
    Plane();
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
    void UVMappedPoint(const Tuple &p, float *u, float *v) const override;
};

#endif /* Plane_hpp */
