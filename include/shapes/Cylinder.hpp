#ifndef Cylinder_hpp
#define Cylinder_hpp

#include "Shape.hpp"

class Cylinder : public Shape {
public:
    Cylinder();
    Cylinder(Tuple p1, Tuple p2);
    Cylinder(float min, float max, bool capped);
    float get_min() const;
    float get_max() const;
    bool get_capped() const;
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
    void UVMappedPoint(const Tuple &p, float *u, float *v) const override;
protected:
    bool check_cap(const Ray &r, const float t, const float rd) const;
    void intersect_caps(const Ray &r, std::vector<Intersection> &xs) const;
    virtual float get_radius(const float y) const;
    float min, max;
    bool capped;
};

#endif /* Cylinder_hpp */
