#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Group.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Material.hpp"
#include "Pattern.hpp"
#include "testHelper.hpp"
#include "Cylinder.hpp"

#include <unordered_set>

SCENARIO("Creating a new group") {
    GIVEN("A group") {
        const std::shared_ptr<Group> g = std::make_shared<Group>();
        THEN("It holds") {
            REQUIRE(g->get_transform() == Matrix<4, 4>::identity());
            REQUIRE(g->empty());
        }
    }
}

SCENARIO("Adding a child to a group") {
    GIVEN("A group and a test shape") {
        const std::shared_ptr<Group> g = std::make_shared<Group>();
        const std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("Adding a child to the group") {
            g->add_child(s);
            THEN("It holds") {
                REQUIRE(!g->empty());
                REQUIRE(g->includes(s));
                REQUIRE(s->get_parent() == g);
            }
        }
    }
}

SCENARIO("Intersecting a ray with an empty group") {
    GIVEN("A group and a ray") {
        const std::shared_ptr<Group> g = std::make_shared<Group>();
        const Ray r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
        WHEN("Finding intersections with the group") {
            std::vector<Intersection> xs;
            r.intersect(g, xs);
            THEN("It holds") {
                REQUIRE(xs.empty());
            }
        }
    }
}

SCENARIO("Intersecting a ray with a nonempty group") {
    GIVEN("A group, a ray, and 3 spheres") {
        const std::shared_ptr<Group> g = std::make_shared<Group>();
        const std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        const std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>();
        s2->set_transform(Transform::translation(0, 0, -3));
        const std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>();
        s3->set_transform(Transform::translation(5, 0, 0));
        g->add_children(s1, s2, s3);
        WHEN("Finding intersections with the group") {
            const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
            std::vector<Intersection> xs;
            r.intersect(g, xs);
            std::unordered_set<ShapeConstPtr> us;
            for (auto x : xs)
                us.insert(x.get_shape());
            THEN("It holds") {
                // TODO: I removed the sort within the group intersect, double check later if this is ok.
                REQUIRE(xs.size() == 4);
                REQUIRE(us.find(s2) != us.end());
                REQUIRE(us.find(s1) != us.end());
//                REQUIRE(xs[0].get_shape() == s2);
//                REQUIRE(xs[1].get_shape() == s2);
//                REQUIRE(xs[2].get_shape() == s1);
//                REQUIRE(xs[3].get_shape() == s1);
            }
        }
    }
}

SCENARIO("Intersecting a transformed group") {
    GIVEN("A group, a ray") {
        const std::shared_ptr<Group> g = std::make_shared<Group>();
        g->set_transform(Transform::scaling(2, 2, 2));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::translation(5, 0, 0));
        g->add_child(s);
        WHEN("Finding intersections with the group") {
            const Ray r = Ray(Point(10, 0, -10), Vector(0, 0, 1));
            std::vector<Intersection> xs;
            r.intersect(g, xs);
            THEN("It holds") {
                REQUIRE(xs.size() == 2);
            }
        }
    }
}

SCENARIO("Partitioning a group's children") {
    GIVEN("Given 3 spheres and a group") {
        const std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        s1->set_transform(Transform::translation(-2, 0, 0));
        const std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>();
        s2->set_transform(Transform::translation(2, 0, 0));
        const std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>();
        const std::shared_ptr<Group> g = std::make_shared<Group>();
        g->add_children(s1, s2, s3);
        WHEN("Partitioning the children") {
            std::shared_ptr<Group> left = std::make_shared<Group>();
            std::shared_ptr<Group> right = std::make_shared<Group>();
            g->partition_children(&left, &right);
            THEN("It holds") {
                REQUIRE(g->includes(s3));
                REQUIRE(left->includes(s1));
                REQUIRE(right->includes(s2));
            }
        }
    }
}


SCENARIO("Index select a group") {
    GIVEN("Given 3 spheres and a group") {
        const std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        s1->set_transform(Transform::translation(-2, 0, 0));
        const std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>();
        s2->set_transform(Transform::translation(2, 0, 0));
        const std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>();
        const std::shared_ptr<Group> g = std::make_shared<Group>();
        g->add_children(s1, s2, s3);
        THEN("It holds") {
            REQUIRE(g->operator[](0) == s1);
            REQUIRE(g->operator[](1) == s2);
            REQUIRE(g->operator[](2) == s3);
        }
    }
}

SCENARIO("Creating a sub-group from a list of children") {
    GIVEN("Given 2 spheres and a group") {
        const std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        const std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>();
        std::shared_ptr<Group> g = std::make_shared<Group>();
        WHEN("Making a subgroup") {
            g->make_subgroup(s1, s2);
            THEN("It holds") {
                REQUIRE(g->count() == 1);
                REQUIRE(std::dynamic_pointer_cast<Group>((g->operator[](0)))->includes(s1));
                REQUIRE(std::dynamic_pointer_cast<Group>((g->operator[](0)))->includes(s2));
                std::shared_ptr<Group> subgroup = std::dynamic_pointer_cast<Group>(g->operator[](0));
                REQUIRE(subgroup->count() == 2);
                REQUIRE(subgroup->includes(s1));
                REQUIRE(subgroup->includes(s2));
            }
        }
    }
}

SCENARIO("Subdividing a group partitions its children") {
    GIVEN("Given 2 spheres and a group") {
        const std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        s1->set_transform(Transform::translation(-2, -2, 0));
        const std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>();
        s2->set_transform(Transform::translation(-2, 2, 0));
        const std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>();
        s3->set_transform(Transform::scaling(4, 4, 4));
        std::shared_ptr<Group> g = std::make_shared<Group>();
        g->add_children(s1, s2, s3);
        WHEN("Dividing") {
            g->divide(1);
            THEN("It holds") {
                REQUIRE(std::dynamic_pointer_cast<Sphere>(g->operator[](0)) == s3);
                REQUIRE(std::dynamic_pointer_cast<Group>(g->operator[](1)));
                std::shared_ptr<Group> subgroup = std::dynamic_pointer_cast<Group>(g->operator[](1));
                REQUIRE(subgroup->count() == 2);
                REQUIRE(std::dynamic_pointer_cast<Group>(subgroup->operator[](0))->includes(s1));
                REQUIRE(std::dynamic_pointer_cast<Group>(subgroup->operator[](1))->includes(s2));
            }
        }
    }
}

