#ifndef Bounds_hpp
#define Bounds_hpp

#include "Helper.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "Tuple.hpp"

class Bounds {
public:
    Bounds();
    Bounds(const Tuple &min, const Tuple &max);
    const Tuple min() const;
    const Tuple max() const;
    void update(const Tuple &p);
    void merge(const Bounds &b);
    bool contains_point(const Tuple &p);
    bool contains_bounds(const Bounds &b);
    bool intersects(const Ray &r) const;
    void split_bounds(Bounds *left, Bounds *right) const;
    friend Bounds operator*(const Bounds &b, const Matrix<4, 4> &m);
private:
    Tuple min_;
    Tuple max_;
};

#endif /* Bounds_hpp */
