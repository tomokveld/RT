#ifndef Cone_hpp
#define Cone_hpp

#include "Shape.hpp"
#include "Cylinder.hpp"

class Cone : public Cylinder {
public:
    Cone();
    Cone(float min, float max, bool capped);
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
protected:
    float get_radius(const float y) const override;
};


#endif /* Cone_hpp */
