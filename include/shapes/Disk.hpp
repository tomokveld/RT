#ifndef Disk_hpp
#define Disk_hpp

#include "Shape.hpp"

class Disk : public Shape {
public:
    Disk();
    Disk(float radius, float inner_radius, float phi_max, float height);
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
private:
    bool check_cap(const Ray &r, const float t, const float rd) const;
    float radius, inner_radius, phi_max, height;
};

#endif /* Disk_hpp */
