#include "Intersection.hpp"

Intersection::Intersection(float distance, ShapeConstPtr const& s) : distance{distance}, shape{s} {}

Intersection::Intersection(float distance, ShapeConstPtr const& s, float u, float v) : distance{distance}, shape{s}, u_(u), v_(v) {}

float Intersection::get_distance() const {
    return distance;
}

ShapeConstPtr Intersection::get_shape() const {
    return shape;
}

float Intersection::u() const {
    return u_;
}

float Intersection::v() const {
    return v_;
}

bool operator==(const Intersection &lhs, const Intersection &rhs) {
    if (&lhs == &rhs)
        return true;
    return lhs.get_shape() == rhs.get_shape() && equal(lhs.get_distance(), rhs.get_distance());
}

Intersection Hit(const std::vector<Intersection> &xs) {
    for (Intersection i : xs) {
        if (i.get_distance() >= 0.0f)
            return i;
    }
    return Intersection(0.0f, nullptr);
}

bool operator<(const Intersection &lhs, const Intersection &rhs) {
    return lhs.get_distance() < rhs.get_distance();
}

float Schlick(const IntersectionComp &comps) {
    // TODO: Worth caching some of these? Since similar things are computed in refracted_color()
    // Find the cosine of the angle between the eye and normal vectors
    float cos = comps.eyev.dot(comps.normalv);
    // Total internal reflection can only occur if n1 > n2
    if (comps.n1 > comps.n2) {
        const float n = comps.n1 / comps.n2;
        const float sin2_t = (n * n) * (1.0f - (cos * cos));
        if (sin2_t > 1.0f)
            return 1.0f;
        // compute cosine of theta_t using trig identity
        const float cos_t = sqrt(1.0f - sin2_t);
        // when n1 > n2, use cos(theta_t) instead
        cos = cos_t;
    }
    const float r0 = powf((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2.0f);
    return r0 + (1 - r0) * powf(1.0f - cos, 5.0f);
}

float Fresnel(const IntersectionComp &comps) {
    float cosThetaI = comps.eyev.dot(comps.normalv);
    float cosTheta = cosThetaI;
    float n1 = comps.n1;
    float n2 = comps.n2;
    float n = n1 / n2;
    float sin2ThetaT = (n * n) * (1.0f - (cosThetaI * cosThetaI));
//    float sinThetaI = sqrt(sin2ThetaT);
    float cosThetaT = sqrt(1.0f - sin2ThetaT);
    if (comps.n1 > comps.n2) {
        if (sin2ThetaT > 1.0f)
            return 1.0f;
        cosTheta = cosThetaT;
    }
    float rs = pow((n1 * cosThetaI - n2 * cosThetaT) /
                   (n1 * cosThetaI + n2 * cosThetaT), 2);
    float rp = pow((n2 * cosThetaI - n1 * cosThetaT) /
                   (n2 * cosThetaI + n1 * cosThetaT), 2);
    return (rs + rp) / 2.0f;
}
