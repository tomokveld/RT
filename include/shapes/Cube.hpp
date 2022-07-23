#ifndef Cube_hpp
#define Cube_hpp

#include "Shape.hpp"
#include "Ray.hpp"
#include "Helper.hpp"

class Cube : public Shape {
public:
    Cube();
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
    void UVMappedPoint(const Tuple &p, float *u, float *v) const override;
};

#endif /* Cube_hpp */
