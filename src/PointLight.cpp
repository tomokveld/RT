#include "PointLight.hpp"

PointLight::PointLight(Tuple position, Color intensity) :
    Light(position, intensity)
{
    n_samples_ = 1;
    sample_cache_.push_back(position);
}

bool operator==(const PointLight &lhs, const PointLight &rhs) {
    return lhs.intensity() == rhs.intensity() && lhs.position() == rhs.position();
}

float PointLight::intensity_at(const Tuple &p, const World &w) const {
    if (w.is_shadowed(p, position_))
        return 0.0f;
    else
        return 1.0f;
}

bool PointLight::operator==(const Light &rhs) const {
    const PointLight *rhs_pointlight = dynamic_cast<const PointLight*>(&rhs);
    return rhs_pointlight && Light::operator==(rhs);
}
