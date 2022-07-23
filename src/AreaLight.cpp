#include "AreaLight.hpp"

AreaLight::AreaLight(Tuple position, Color intensity, Tuple v1, Tuple v2, int usteps, int vsteps) :
      Light(position + (v1 + v2) / 2.0f, intensity),
      corner_(position),
      uvec_(v1 / usteps),
      vvec_(v2 / vsteps),
      usteps_(usteps),
      vsteps_(vsteps),
      jitter_([](){return 0.5;})
{
    n_samples_ = usteps * vsteps;

    for (int v = 0; v < vsteps_; v++) {
        for (int u = 0; u < usteps_; u++) {
            sample_cache_.push_back(point_on_light(u, v));
        }
    }
}

float AreaLight::intensity_at(const Tuple &p, const World &w) const {
    float total = 0.0;
    for (int v = 0; v < vsteps_; v++) {
        for (int u = 0; u < usteps_; u++) {
            Tuple light_p = point_on_light(u, v);
            if (!w.is_shadowed(p, light_p))
                total += 1.0;
        }
    }
    return total / n_samples_;
}

Tuple AreaLight::point_on_light(float u, float v) const {
    return corner_ +
           uvec_ * (u + jitter_by()) +
           vvec_ * (v + jitter_by());
}

Tuple AreaLight::corner() const {
    return corner_;
}

Tuple AreaLight::uvec() const {
    return uvec_;
}

Tuple AreaLight::vvec() const {
    return vvec_;
}

int AreaLight::usteps() const {
    return usteps_;
}

int AreaLight::vsteps() const {
    return vsteps_;
}

bool AreaLight::operator==(const Light &rhs) const {
    const AreaLight *rhs_arealight = dynamic_cast<const AreaLight*>(&rhs);
    return rhs_arealight && Light::operator==(rhs);
}


float AreaLight::jitter_by() const {
    return jitter_();
}

void AreaLight::set_jitter(std::function<float()> f) {
    jitter_ = f;
}
