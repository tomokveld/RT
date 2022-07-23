#ifndef PointLight_hpp
#define PointLight_hpp

#include "World.hpp"
#include "Light.hpp"

class PointLight : public Light {
public:
    PointLight(Tuple position, Color intensity);
    float intensity_at(const Tuple &p, const World &w) const override;
    bool operator==(const Light &rhs) const override;
};

#endif /* PointLight_hpp */
