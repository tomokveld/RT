#ifndef Capsule_hpp
#define Capsule_hpp

#include "Shape.hpp"

class Capsule : public Shape {
public:
    Capsule();
    Capsule(Tuple pa, Tuple pb, float ra);
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
    void UVMappedPoint(const Tuple &p, float *u, float *v) const override;
    const Tuple pa() const;
    const Tuple pb() const;
    const float ra() const;
private:
    const float ra_;
    const Tuple pa_, pb_;
};

#endif /* Capsule_hpp */
