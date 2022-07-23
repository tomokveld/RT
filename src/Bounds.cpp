#include "Bounds.hpp"

Bounds::Bounds() :
    min_(Point(INF, INF, INF)),
    max_(Point(-INF, -INF, -INF))
{}

Bounds::Bounds(const Tuple &min, const Tuple &max) :
    min_(min),
    max_(max)
{}

const Tuple Bounds::min() const {
    return min_;
}

const Tuple Bounds::max() const {
    return max_;
}

void Bounds::update(const Tuple &p) {
    min_ = Point(std::min(min_.get_x(), p.get_x()),
                std::min(min_.get_y(), p.get_y()),
                std::min(min_.get_z(), p.get_z()));
    max_ = Point(std::max(max_.get_x(), p.get_x()),
                std::max(max_.get_y(), p.get_y()),
                std::max(max_.get_z(), p.get_z()));
}

void Bounds::merge(const Bounds &b) {
    min_ = Point(std::min(min_.get_x(), b.min_.get_x()),
                 std::min(min_.get_y(), b.min_.get_y()),
                 std::min(min_.get_z(), b.min_.get_z()));
    max_ = Point(std::max(max_.get_x(), b.max_.get_x()),
                std::max(max_.get_y(), b.max_.get_y()),
                std::max(max_.get_z(), b.max_.get_z()));
}

bool Bounds::contains_point(const Tuple &p) {
    return (p.get_x() >= min_.get_x()) && (p.get_x() <= max_.get_x()) &&
           (p.get_y() >= min_.get_y()) && (p.get_y() <= max_.get_y()) &&
           (p.get_z() >= min_.get_z()) && (p.get_z() <= max_.get_z());
}

bool Bounds::contains_bounds(const Bounds &b) {
    return (b.min_.get_x() >= min_.get_x()) && (b.max_.get_x() <= max_.get_x()) &&
           (b.min_.get_y() >= min_.get_y()) && (b.max_.get_y() <= max_.get_y()) &&
           (b.min_.get_z() >= min_.get_z()) && (b.max_.get_z() <= max_.get_z());
}

Bounds operator*(const Bounds &b, const Matrix<4, 4> &m) {
    Tuple points[8] = {m * b.min_,
                       m * Point(b.min_.get_x(), b.min_.get_y(), b.max_.get_z()),
                       m * Point(b.min_.get_x(), b.max_.get_y(), b.min_.get_z()),
                       m * Point(b.min_.get_x(), b.max_.get_y(), b.max_.get_z()),
                       m * Point(b.max_.get_x(), b.min_.get_y(), b.min_.get_z()),
                       m * Point(b.max_.get_x(), b.min_.get_y(), b.max_.get_z()),
                       m * Point(b.max_.get_x(), b.max_.get_y(), b.min_.get_z()),
                       m * b.max_};
    
    Tuple min = Point(INF, INF, INF);
    Tuple max = Point(-INF, -INF, -INF);
    
    for (const Tuple p : points) {
        if (p.get_x() < min.get_x())
            min.mod_x() = p.get_x();
        if (p.get_y() < min.get_y())
            min.mod_y() = p.get_y();
        if (p.get_z() < min.get_z())
            min.mod_z() = p.get_z();
        
        if (p.get_x() > max.get_x())
            max.mod_x() = p.get_x();
        if (p.get_y() > max.get_y())
            max.mod_y() = p.get_y();
        if (p.get_z() > max.get_z())
            max.mod_z() = p.get_z();
    }
    return Bounds(min, max);
}


// Optimized slab method
// Does not nicely handle case if the ray lies exactly on a slab
// 82.88s
//bool Bounds::intersects(const Ray &r) const {
//    Tuple o = r.get_origin();
//    Tuple d = Vector(1.0/r.get_direction().get_x(), 1.0/r.get_direction().get_y(), 1.0/r.get_direction().get_z());
//
//    double tmin = -INFINITY, tmax = INFINITY;
//    for (int i = 0; i < 3; i++) {
//        if (d[i] != 0.0) {
//            double t1 = (min_[i] - o[i]) * d[i];
//            double t2 = (max_[i] - o[i]) * d[i];
//
//            tmin = std::max(tmin, std::min(t1, t2));
//            tmax = std::min(tmax, std::max(t1, t2));
//        }
//    }
//    return tmax > std::max(tmin, 0.0);
//}

// Optimized slab method
// Should be able to handle the special case
// 81.85s
//bool Bounds::intersects(const Ray &r) const {
//    Tuple o = r.get_origin();
//    Tuple d = Vector(1.0/r.get_direction().get_x(), 1.0/r.get_direction().get_y(), 1.0/r.get_direction().get_z());
//    //
//    double t1 = (min_[0] - o[0]) * d[0];
//    double t2 = (max_[0] - o[0]) * d[0];
//
//    double tmin = std::min(t1, t2);
//    double tmax = std::max(t1, t2);
//
//    for (int i = 1; i < 3; i++) {
//            double t1 = (min_[i] - o[i]) * d[i];
//            double t2 = (max_[i] - o[i]) * d[i];
//
//            tmin = std::max(tmin, std::min(std::min(t1, t2), tmax));
//            tmax = std::min(tmax, std::max(std::max(t1, t2), tmin));
//        }
//    return tmax > std::max(tmin, 0.0);
//}


// Optimized slab method
// Should be able to handle the special case, not as consistently, faster
// 83.58s
bool Bounds::intersects(const Ray &r) const {
    Tuple o = r.get_origin();
    Tuple d = Vector(1.0/r.get_direction().get_x(),
                     1.0/r.get_direction().get_y(),
                     1.0/r.get_direction().get_z());

    double t1 = (min_[0] - o[0]) * d[0];
    double t2 = (max_[0] - o[0]) * d[0];

    double tmin = std::min(t1, t2);
    double tmax = std::max(t1, t2);

    for (int i = 1; i < 3; i++) {
            double t1 = (min_[i] - o[i]) * d[i];
            double t2 = (max_[i] - o[i]) * d[i];
            tmin = std::max(tmin, std::min(t1, t2));
            tmax = std::min(tmax, std::max(t1, t2));
        }
    return tmax > std::max(tmin, 0.0);
}

// Prev approach
//bool Bounds::intersects(const Ray &r) const {
//    float xtmin, xtmax, ytmin, ytmax, ztmin, ztmax;
//    check_axis(r.get_origin().get_x(), r.get_direction().get_x(), &xtmin, &xtmax, min_.get_x(), max_.get_x());
//    check_axis(r.get_origin().get_y(), r.get_direction().get_y(), &ytmin, &ytmax, min_.get_y(), max_.get_y());
//    check_axis(r.get_origin().get_z(), r.get_direction().get_z(), &ztmin, &ztmax, min_.get_z(), max_.get_z());
//
//    float tmin = std::max({xtmin, ytmin, ztmin});
//    float tmax = std::min({xtmax, ytmax, ztmax});
//
//    return tmax >= 0.0f && tmin <= tmax;  // Includes both test for checking behind and for intersection
//}

void Bounds::split_bounds(Bounds *left, Bounds *right) const {
    const float dx = max_.get_x() - min_.get_x();
    const float dy = max_.get_y() - min_.get_y();
    const float dz = max_.get_z() - min_.get_z();
    const float greatest = std::max({dx, dy, dz});

    float x0 = min_.get_x();
    float y0 = min_.get_y();
    float z0 = min_.get_z();

    float x1 = max_.get_x();
    float y1 = max_.get_y();
    float z1 = max_.get_z();

    if (greatest == dx) {
        x0 += dx / 2.0f;
        x1 = x0;
    } else if (greatest == dy) {
        y0 += dy / 2.0f;
        y1 = y0;
    } else {
        z0 += dz / 2.0f;
        z1 = z0;
    }

    const Tuple mid_min = Point(x0, y0, z0);
    const Tuple mid_max = Point(x1, y1, z1);

    *left = Bounds(min_, mid_max);
    *right = Bounds(mid_min, max_);
}
