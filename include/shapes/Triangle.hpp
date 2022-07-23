#ifndef Triangle_hpp
#define Triangle_hpp

#include "Shape.hpp"

class Triangle : public Shape {
public:
    Triangle(Tuple p1, Tuple p2, Tuple p3);
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;    
    const Tuple p1() const;
    const Tuple p2() const;
    const Tuple p3() const;
    const Tuple e1() const;
    const Tuple e2() const;
    const Tuple normal() const;
private:
    const Tuple p1_, p2_, p3_;
    const Tuple e1_, e2_;
    const Tuple normal_;
    isect_hh_data D;
};

#endif /* Triangle_hpp */
