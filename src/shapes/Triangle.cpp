#include "Triangle.hpp"

Triangle::Triangle(Tuple p1, Tuple p2, Tuple p3) :
    p1_(p1),
    p2_(p2),
    p3_(p3),
    e1_(p2_ - p1_),
    e2_(p3_ - p1_),
    normal_(e2_.cross(e1_).normalize())
{
    // Havel and Herout
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

const Tuple Triangle::p1() const {
    return p1_;
}

const Tuple Triangle::p2() const {
    return p2_;
}

const Tuple Triangle::p3() const {
    return p3_;
}

const Tuple Triangle::e1() const {
    return e1_;
}

const Tuple Triangle::e2() const {
    return e2_;
}

const Tuple Triangle::normal() const {
    return normal_;
}

// Havel and Herout
void Triangle::intersect(const Ray &r, std::vector<Intersection> &xs) const {
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
        xs.push_back(Intersection(t, shared_from_this()));
}

// Möller–Trumbore adaptation
//void Triangle::intersect(const Ray &r, std::vector<Intersection> &xs) const {
//    Tuple o = r.get_origin();
//    Tuple d = r.get_direction();
//    Tuple pvec = d.cross(e2_);
//    float det = e1_.dot(pvec);
//
//    if (det < EPSILON)
//        return;
//
//    float inv_det = 1.0f / det;
//    Tuple tvec = o - p1_;
//    float u = tvec.dot(pvec) * inv_det;
//
//    if (u < 0.0f || u > 1.0f)
//        return;
//
//    Tuple qvec = tvec.cross(e1_);
//    float v = d.dot(qvec) * inv_det;
//    if (v < 0.0f || (u + v) > 1.0f)
//        return;
//
//    float t = e2_.dot(qvec) * inv_det;
//
//    if (t >= ISECT_NEAR && t <= ISECT_FAR)
//        xs.push_back(Intersection(t, shared_from_this()));
//}


// Möller–Trumbore
//void Triangle::intersect(const Ray &r, std::vector<Intersection> &xs) const {
//    const Tuple h = r.get_direction().cross(e2_);
//    const float det = e1_.dot(h);
//
//    if (fabs(det) < EPSILON)
//        return;
//
//    const float f = 1.0f / det;
//    const Tuple s = r.get_origin() - p1_;
//    const float u = f * s.dot(h);
//
//    if (u < 0.0f || u > 1.0f)
//        return;
//
//    const Tuple q = s.cross(e1_);
//    const float v = f * r.get_direction().dot(q);
//
//    if (v < 0.0f || (u + v) > 1.0f)
//        return;
//
//    const float t = f * e2_.dot(q);
//
//    xs.push_back(Intersection(t, shared_from_this()));
//}

Tuple Triangle::normal_at_local(const Tuple &p, const Intersection &i) const {
    return normal_;
}

// TODO: This is basically the same in all my shapes, create a generic version.
bool Triangle::operator==(const Shape &rhs) const {
    const Triangle *rhs_triangle = dynamic_cast<const Triangle*>(&rhs);
    return rhs_triangle && Shape::operator==(rhs);
}
