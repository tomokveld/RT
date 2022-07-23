#ifndef CSG_hpp
#define CSG_hpp

#include "Shape.hpp"


// TODO: Since my Shape class uses shared pointers I get in a bit of trouble here when initializing the parents of the children of the CSG since the CSG parent is not initialized yet...


class csgOperator : public std::enable_shared_from_this<csgOperator> {
public:
    virtual bool intersection_allowed(bool lhit, bool inl, bool inr) const = 0;
};

class csgUnionOperator : public csgOperator {
public:
    bool intersection_allowed(bool lhit, bool inl, bool inr) const override;
};

class csgIntersectionOperator : public csgOperator {
public:
    bool intersection_allowed(bool lhit, bool inl, bool inr) const override;
};

class csgDifferenceOperator : public csgOperator {
public:
    bool intersection_allowed(bool lhit, bool inl, bool inr) const override;
};


class CSG : public Shape {
public:
    CSG(std::shared_ptr<csgOperator> op, ShapePtr left, ShapePtr right);
    static std::shared_ptr<CSG> create(std::shared_ptr<csgOperator> op, ShapePtr left, ShapePtr right) {
        std::shared_ptr<CSG> ret(std::make_shared<CSG>(op, left, right));
        ret->init();
        return ret;
    }
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    void filter_intersections(std::vector<Intersection> &xs) const;
    bool operator==(const Shape &rhs) const override;
    ShapePtr left() const;
    ShapePtr right() const;
    std::shared_ptr<csgOperator> op() const;
    bool includes(const ShapePtr &s) const override;
    void update_bounds();
    void divide(int threshold) override;
private:
    void init();
    std::shared_ptr<csgOperator> op_;
    ShapePtr left_;
    ShapePtr right_;
};

#endif /* CSG_hpp */
