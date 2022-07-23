#ifndef AreaLight_hpp
#define AreaLight_hpp

#include "World.hpp"
#include "Light.hpp"

class AreaLight : public Light {
public:
    AreaLight(Tuple position, Color intensity, Tuple v1, Tuple v2, int usteps, int vsteps);
    float intensity_at(const Tuple &p, const World &w) const override;
    Tuple point_on_light(float u, float v) const;
    Tuple corner() const;
    Tuple uvec() const;
    Tuple vvec() const;
    int usteps() const;
    int vsteps() const;
    float jitter_by() const;
    void set_jitter(std::function<float()>);
    bool operator==(const Light &rhs) const override;
private:
    std::function<float()> jitter_;
    Tuple corner_;
    Tuple uvec_;
    Tuple vvec_;
    int usteps_;
    int vsteps_;
};

#endif /* AreaLight_hpp */
