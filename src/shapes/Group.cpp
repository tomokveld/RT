#include "Group.hpp"

Group::Group() {}

void Group::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    if (bounds.intersects(r)) {
        for (const auto &child : members) {
            r.intersect(child, xs);
        }
    }
}

Tuple Group::normal_at_local(const Tuple &p, const Intersection &i) const {
    throw std::runtime_error("A group does not have a normal vector!");
}

// TODO: Implement operator==
bool Group::operator==(const Shape &rhs) const {
    throw std::runtime_error("Not implemented yet!");
//    return false;
}

bool Group::empty() const {
    return members.empty();
}

void Group::add_child(const ShapePtr &child, bool update) {
    members.push_back(child);
    child->set_parent(shared_from_this());
    if (update)
         update_bounds();
}

size_t Group::count() const {
    return members.size();
}

bool Group::includes(const ShapePtr &child) const {
    return std::find(members.begin(), members.end(), child) != members.end();
}

void Group::update_bounds() {
    Bounds box = Bounds();
    for (auto child : members)
        box.merge(child->bounds_transform);
    bounds = box;
    bounds_transform = bounds * get_transform();
}

void Group::partition_children(std::shared_ptr<Group> *left_g, std::shared_ptr<Group> *right_g) {
    Bounds left_b, right_b;
    bounds.split_bounds(&left_b, &right_b);

    std::vector<ShapePtr> remaining;
    for (auto child : members) {
        if (left_b.contains_bounds(child->bounds_transform))
            (*left_g)->add_child(child);
        else if (right_b.contains_bounds(child->bounds_transform))
            (*right_g)->add_child(child);
        else
            remaining.push_back(child);
    }
    members = remaining;
}

ShapePtr Group::operator[](size_t x) const {
    return members[x];
}

ShapePtr& Group::operator[](size_t x) {
    return members[x];
}

void Group::divide(int threshold) {
    if (threshold <= count()) {
        std::shared_ptr<Group> left = std::make_shared<Group>();
        std::shared_ptr<Group> right = std::make_shared<Group>();
        partition_children(&left, &right);

        if (!left->empty()) {
            const std::shared_ptr<Group> subgroup = std::make_shared<Group>();
            for (int i = 0; i < left->count(); i++)
                subgroup->add_child((*left)[i]);
            add_child(subgroup);
        }
        if (!right->empty()) {
            const std::shared_ptr<Group> subgroup = std::make_shared<Group>();
            for (int i = 0; i < right->count(); i++)
                subgroup->add_child((*right)[i]);
            add_child(subgroup);
        }
    }

    for (auto child : members)
        child->divide(threshold);
    update_bounds();
}

std::shared_ptr<Cube> Group::get_bbox() {
    Tuple scaling = (bounds_transform.max() - bounds_transform.min()) / 2;
    Tuple translate = (bounds_transform.min() + bounds_transform.max()) / 2;

    std::shared_ptr<Cube> bbox = std::make_shared<Cube>();
    bbox->set_transform(Transform::translation(translate[0] - 0.0001,
                                               translate[1] - 0.0001,
                                               translate[2] - 0.0001) *
                        Transform::scaling(scaling[0],
                                           scaling[1],
                                           scaling[2]));
    bbox->set_material(bbox_m());
    return bbox;
}

// TODO: This is still not ideal
//void recursive_bound(ShapePtr s, World &w, int max_depth, int cur_depth, int target_depth) {
//    if (cur_depth >= max_depth || !std::dynamic_pointer_cast<Group>(s)) {
//        std::cout << "Exit\n";
//        return;
//    }
//
//    auto group = std::dynamic_pointer_cast<Group>(s);
//
//    if (target_depth > 0) {
//        if (cur_depth == target_depth) {
//            w.insert(group->get_bbox());
//            return;
//        }
//    } else {
//        w.insert(group->get_bbox());
//    }
//
//    for (auto child : group->members)
//        recursive_bound(child, w, max_depth, cur_depth + 1, target_depth);
//}
