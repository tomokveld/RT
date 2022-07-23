#ifndef Group_hpp
#define Group_hpp

#include "Shape.hpp"
#include "Cube.hpp"

class Group : public Shape {
public:
    Group();
    // TODO: Add constructor using fold expression
    void intersect(const Ray &r, std::vector<Intersection> &xs) const override;
    Tuple normal_at_local(const Tuple &p, const Intersection &i) const override;
    bool operator==(const Shape &rhs) const override;
    bool includes(const ShapePtr &child) const override;
    void add_child(const ShapePtr &child, bool update=true);
    bool empty() const;
    size_t count() const;
    void partition_children(std::shared_ptr<Group> *left_g, std::shared_ptr<Group> *right_g);
    template <class... Ts>
    void add_children(Ts const&... args);
    template <class... Ts>
    void make_subgroup(Ts const&... args);
    void divide(int threshold) override;
    void update_bounds();
    ShapePtr operator[](size_t x) const;
    ShapePtr& operator[](size_t x);

    std::shared_ptr<Cube> get_bbox();
    std::vector<ShapePtr> members;
private:
};

template <class... Ts>
void Group::add_children(Ts const&... args) {    
    ((add_child(args)), ...);
}

template <class... Ts>
void Group::make_subgroup(Ts const&... args) {
    const std::shared_ptr<Group> subgroup = std::make_shared<Group>();
    subgroup->add_children(args...);
    add_child(subgroup);
}

#endif /* Group_hpp */
