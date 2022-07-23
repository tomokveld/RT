#include "Capsule.hpp"

Capsule::Capsule() :
    Capsule(Vector(-0.3,-0.1,-0.1), Vector(-0.3,-0.1,-0.1), 0.2)
{}

Capsule::Capsule(Tuple pa, Tuple pb, float ra) :
    pa_(pa),
    pb_(pb),
    ra_(ra)
{
    bounds = Bounds((pa_ - ra_).min(pb_ - ra_), (pa_ + ra_).max(pb_ + ra_));
    bounds_transform = bounds;
}

void Capsule::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    Tuple ro = r.get_origin();
    ro[3] = 0.0f;
    Tuple rd = r.get_direction();
    Tuple ba = pb_ - pa_;
    Tuple oa = ro - pa_;

    float baba = ba.dot(ba);
    float bard = ba.dot(rd);
    float baoa = ba.dot(oa);
    float rdoa = rd.dot(oa);
    float oaoa = oa.dot(oa);

    float a = baba - bard*bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - ra_ * ra_ * baba;
    float h = b * b - a * c;
    if (h >= 0.0f) {
        float t = (-b - sqrt(h)) / a;
        float y = baoa + t * bard;
        if (y > 0.0f && y < baba) {  // body
            xs.push_back(Intersection(t, shared_from_this()));
        } else {  // caps
            Tuple oc = (y <= 0.0f) ? oa : ro - pb_;
            b = rd.dot(oc);
            c = oc.dot(oc) - ra_ * ra_;
            h = b * b - c;
            if (h > 0.0f)
                xs.push_back(Intersection(-b - sqrt(h), shared_from_this()));
        }
    }
}

Tuple Capsule::normal_at_local(const Tuple &p, const Intersection &i) const {
    Tuple ba = pb_ - pa_;
    Tuple pa = p - pa_;
    float h = fclamp(pa.dot(ba) / ba.dot(ba), 0.0, 1.0);
    return (pa - h * ba) / ra_;
}

bool Capsule::operator==(const Shape &rhs) const {
    return false;
}

const Tuple Capsule::pa() const {
    return pa_;
}

const Tuple Capsule::pb() const {
    return pb_;
}

const float Capsule::ra() const {
    return ra_;
}

void Capsule::UVMappedPoint(const Tuple &p, float *u, float *v) const {
    Tuple w_vec = (pb_ - pa_).normalize();
    Tuple u_vec = w_vec.cross(Vector(0, 0, 1));
    Tuple v_vec = u_vec.cross(w_vec).normalize();
    // TODO: Clean up this mess
    Tuple q_vec = Matrix<4, 4> {{u_vec[0], u_vec[1], u_vec[2], u_vec[3]},
                                {v_vec[0], v_vec[1], v_vec[2], v_vec[3]},
                                {w_vec[0], w_vec[1], w_vec[2], w_vec[3]},
                                {0, 0, 0, 0}} * (p - pa_);
    *u = atan2(q_vec[1], q_vec[0]);
    *v = q_vec[2];
}
