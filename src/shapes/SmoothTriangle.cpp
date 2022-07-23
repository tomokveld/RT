#include "SmoothTriangle.hpp"

SmoothTriangle::SmoothTriangle(Tuple p1, Tuple p2, Tuple p3, Tuple n1, Tuple n2, Tuple n3) :
    p1_(p1),
    p2_(p2),
    p3_(p3),
    n1_(n1),
    n2_(n2),
    n3_(n3),
    e1_(p2_ - p1_),
    e2_(p3_ - p1_)
{
    //
    D.n0 = e1_.cross(e2_);
    D.d0 = D.n0.dot(p1_);

    float inv_denom = 1.0f / D.n0.dot(D.n0);

    D.n1 = e2_.cross(D.n0) * inv_denom;
    D.d1 = -D.n1.dot(p1_);

    D.n2 = D.n0.cross(e1_) * inv_denom;
    D.d2 = -D.n2.dot(p1_);
    //    

    float minx = std::min(p1_.get_x(), std::min(p2_.get_x(), p3_.get_x()));
    float miny = std::min(p1_.get_y(), std::min(p2_.get_y(), p3_.get_y()));
    float minz = std::min(p1_.get_z(), std::min(p2_.get_z(), p3_.get_z()));

    float maxx = std::max(p1_.get_x(), std::max(p2_.get_x(), p3_.get_x()));
    float maxy = std::max(p1_.get_y(), std::max(p2_.get_y(), p3_.get_y()));
    float maxz = std::max(p1_.get_z(), std::max(p2_.get_z(), p3_.get_z()));

    bounds = Bounds(Point(minx, miny, minz), Point(maxx, maxy, maxz));
    
    bounds_transform = bounds;
}

const Tuple SmoothTriangle::p1() const {
    return p1_;
}

const Tuple SmoothTriangle::p2() const {
    return p2_;
}

const Tuple SmoothTriangle::p3() const {
    return p3_;
}

const Tuple SmoothTriangle::n1() const {
    return n1_;
}

const Tuple SmoothTriangle::n2() const {
    return n2_;
}

const Tuple SmoothTriangle::n3() const {
    return n3_;
}

const Tuple SmoothTriangle::e1() const {
    return e1_;
}

const Tuple SmoothTriangle::e2() const {
    return e2_;
}

void SmoothTriangle::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    Tuple o = r.get_origin();
    Tuple d = r.get_direction();
    float det = D.n0.dot(d);
    float dett = D.d0 - o.dot(D.n0);
    Tuple wr = o * det + d * dett;
    float u = wr.dot(D.n1) + det * D.d1;
    float v = wr.dot(D.n2) + det * D.d2;

    float tmpdet0 = det - u - v;
    int pdet0 = BW_FLOAT_TO_UINT(tmpdet0);
    int pdetu = BW_FLOAT_TO_UINT(u);
    int pdetv = BW_FLOAT_TO_UINT(v);
    pdet0 = pdet0 ^ pdetu;
    pdet0 = pdet0 | (pdetu ^ pdetv);
    if (pdet0 & 0x80000000)
        return;

    float rdet = 1 / det;
    u *= rdet;
    v *= rdet;

    float t = dett * rdet;

    if (t >= ISECT_NEAR && t <= ISECT_FAR) // TODO: Look at this
        xs.push_back(Intersection(t, shared_from_this(), u, v));
}

// This is basically identical to the triangle code except for adding of the intersection, combine
//void SmoothTriangle::intersect(const Ray &r, std::vector<Intersection> &xs) const {
//    Tuple dir_cross_e2 = r.get_direction().cross(e2_);
//    float det = e1_.dot(dir_cross_e2);
//
//    if (fabs(det) < EPSILON)
//        return;
//
//    float f = 1.0f / det;
//    Tuple p1_to_origin = r.get_origin() - p1_;
//    float u = f * p1_to_origin.dot(dir_cross_e2);
//
//    if (u < 0.0f || u > 1.0f)
//        return;
//
//    Tuple origin_cross_e1 = p1_to_origin.cross(e1_);
//    float v = f * r.get_direction().dot(origin_cross_e1);
//
//    if (v < 0.0f || (u + v) > 1.0f)
//        return;
//
//    float t = f * e2_.dot(origin_cross_e1);
//
//    xs.push_back(Intersection(t, shared_from_this(), u, v));
//}

Tuple SmoothTriangle::normal_at_local(const Tuple &p, const Intersection &i) const {
    return n2_ * i.u() + n3_ * i.v() + n1_ * (1.0f - i.u() - i.v());
}

bool SmoothTriangle::operator==(const Shape &rhs) const {
    const SmoothTriangle *rhs_smoothtriangle = dynamic_cast<const SmoothTriangle*>(&rhs);
    return rhs_smoothtriangle && Shape::operator==(rhs);
}

