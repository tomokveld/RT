#include "CSG.hpp"

bool csgUnionOperator::intersection_allowed(bool lhit, bool inl, bool inr) const {
    return (lhit && (!inr)) || ((!lhit) && (!inl));
}

bool csgIntersectionOperator::intersection_allowed(bool lhit, bool inl, bool inr) const {
    return (lhit && inr) || ((!lhit) && inl);
}

bool csgDifferenceOperator::intersection_allowed(bool lhit, bool inl, bool inr) const {
    return (lhit && (!inr)) || ((!lhit) && inl);
}

CSG::CSG(std::shared_ptr<csgOperator> op, ShapePtr left, ShapePtr right) :
    op_(op),
    left_(left),
    right_(right)
{}

void CSG::init() {
    left_->set_parent(shared_from_this());
    right_->set_parent(shared_from_this());
    update_bounds();
}

void CSG::update_bounds() {
    Bounds box = Bounds();
    box.merge(left_->bounds_transform);
    box.merge(right_->bounds_transform);
    bounds = box;
    bounds_transform = bounds * get_transform();
}

//void CSG::intersect(const Ray &r, std::vector<Intersection> &xs) const {
//    if (bounds.intersects(r)) {
//        std::vector<Intersection> ys;
//        r.intersect(left_, ys);
//        r.intersect(right_, ys);
//        std::sort(ys.begin(), ys.end());
//        filter_intersections(ys);
//        std::move(ys.begin(), ys.end(), std::back_inserter(xs));
//        std::sort(xs.begin(), xs.end()); // TODO: Is this sort needed?
//    }
//}


void CSG::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    if (bounds.intersects(r)) {
        std::vector<Intersection> ys;
        r.intersect(left_, ys);
        r.intersect(right_, ys);
        std::sort(ys.begin(), ys.end());
        filter_intersections(ys);
        std::move(ys.begin(), ys.end(), std::back_inserter(xs));
        std::sort(xs.begin(), xs.end()); // TODO: Is this sort needed?
    }
}


void CSG::divide(int threshold) {
    left_->divide(threshold);
    right_->divide(threshold);
    update_bounds();
}

bool CSG::includes(const ShapePtr &s) const {
    return left_->includes(s) || right_->includes(s);
}

void CSG::filter_intersections(std::vector<Intersection> &xs) const {
    bool inl = false;
    bool inr = false;
    std::vector<Intersection> res;
    for (const Intersection &i : xs) {
        bool lhit = left_->includes(std::const_pointer_cast<Shape>(i.get_shape()));
        if (op_->intersection_allowed(lhit, inl, inr))
            res.push_back(i);
        if (lhit)
            inl = !inl;
        else
            inr = !inr;
    }
    xs = res;
}

Tuple CSG::normal_at_local(const Tuple &p, const Intersection &i) const {
    assert(false);
    return Vector(0.0f, 0.0f, 0.0f);
}

std::shared_ptr<csgOperator> CSG::op() const {
    return op_;
}

bool CSG::operator==(const Shape &rhs) const {
    // TODO: Implement
    return false;
}

ShapePtr CSG::left() const {
    return left_;
}

ShapePtr CSG::right() const {
    return right_;
}
