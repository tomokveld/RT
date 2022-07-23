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
#include "CSG.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"

SCENARIO("CSG is created with an operation and two shapes") {
    GIVEN("A sphere and a cube") {
        std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        std::shared_ptr<Cube> s2 = std::make_shared<Cube>();
        WHEN("Creating a CSG object") {
            std::shared_ptr<CSG> c = CSG::create(std::make_shared<csgUnionOperator>(), s1, s2);
            THEN("It should hold") {
                REQUIRE(c->left() == s1);
                REQUIRE(c->right() == s2);
                REQUIRE(s1->get_parent() == c);
                REQUIRE(s2->get_parent() == c);
            }
        }
    }
}

SCENARIO("Evaluating the rule for the CSG union operation") {
    std::tuple<bool, bool, bool, bool> p[] = {{true, true, true, false},
                                              {true, true, false, true},
                                              {true, false, true, false},
                                              {true, false, false, true},
                                              {false, true, true, false},
                                              {false, true, false, false},
                                              {false, false, true, true},
                                              {false, false, false, true}};
    for (auto &[lhit, inl, inr, result] : p) {
        std::shared_ptr<csgUnionOperator> c = std::make_shared<csgUnionOperator>();
        REQUIRE(c->intersection_allowed(lhit, inl, inr) == result);
    }
}

SCENARIO("Evaluating the rule for the CSG intersection operation") {
    std::tuple<bool, bool, bool, bool> p[] = {{true, true, true, true},
                                              {true, true, false, false},
                                              {true, false, true, true},
                                              {true, false, false, false},
                                              {false, true, true, true},
                                              {false, true, false, true},
                                              {false, false, true, false},
                                              {false, false, false, false}};
    for (auto &[lhit, inl, inr, result] : p) {
        std::shared_ptr<csgIntersectionOperator> c = std::make_shared<csgIntersectionOperator>();
        REQUIRE(c->intersection_allowed(lhit, inl, inr) == result);
    }
}

SCENARIO("Evaluating the rule for the CSG difference operation") {
    std::tuple<bool, bool, bool, bool> p[] = {{true, true, true, false},
                                              {true, true, false, true},
                                              {true, false, true, false},
                                              {true, false, false, true},
                                              {false, true, true, true},
                                              {false, true, false, true},
                                              {false, false, true, false},
                                              {false, false, false, false}};
    for (auto &[lhit, inl, inr, result] : p) {
        std::shared_ptr<csgDifferenceOperator> c = std::make_shared<csgDifferenceOperator>();
        REQUIRE(c->intersection_allowed(lhit, inl, inr) == result);
    }
}

SCENARIO("Filtering a list of intersections") {
    std::tuple<std::shared_ptr<csgOperator>, int, int> p[] = {{std::make_shared<csgUnionOperator>(), 0, 3},
                                                              {std::make_shared<csgIntersectionOperator>(), 1, 2},
                                                              {std::make_shared<csgDifferenceOperator>(), 0, 1}};
    for (auto &[op, x0, x1] : p) {
        std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        std::shared_ptr<Cube> s2 = std::make_shared<Cube>();
        std::shared_ptr<CSG> c = CSG::create(op, s1, s2);
        std::vector<Intersection> xs {{1, s1}, {2, s2}, {3, s1}, {4, s2}};
        std::vector<Intersection> res {{1, s1}, {2, s2}, {3, s1}, {4, s2}};
        c->filter_intersections(res);
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == xs[x0]);
        REQUIRE(res[1] == xs[x1]);
    }
}

SCENARIO("A ray misses a CSG object") {
    GIVEN("A CSG and a ray") {
        std::shared_ptr<CSG> c = CSG::create(std::make_shared<csgUnionOperator>(),
                                             std::make_shared<Sphere>(),
                                             std::make_shared<Cube>());
        const Ray r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
        WHEN("Building the intersection") {
            std::vector<Intersection> xs;
            c->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.empty());
            }
        }
    }
}

SCENARIO("A ray hits a CSG object") {
    GIVEN("A CSG and a ray") {
        std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        std::shared_ptr<Cube> s2 = std::make_shared<Cube>();
        s2->set_transform(Transform::translation(0, 0, 0.5));
        std::shared_ptr<CSG> c = CSG::create(std::make_shared<csgUnionOperator>(),
                                             std::make_shared<Sphere>(),
                                             std::make_shared<Cube>());
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        WHEN("Building the intersection") {
            std::vector<Intersection> xs;
            c->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 2);
            }
        }
    }
}

SCENARIO("A CSG shape has a bounding box that contains its children") {
    GIVEN("Some shapes and a CSG") {
        const std::shared_ptr<Sphere> left = std::make_shared<Sphere>();
       
        const std::shared_ptr<Sphere> right = std::make_shared<Sphere>();
        right->set_transform(Transform::translation(2, 3, 4));
        std::shared_ptr<CSG> shape = CSG::create(std::make_shared<csgDifferenceOperator>(), left, right);
        WHEN("Getting the bounds of the group") {
            Bounds box = shape->bounds;
            THEN("It holds") {
                REQUIRE(box.min() == Point(-1, -1, -1));
                REQUIRE(box.max() == Point(3, 4, 5));
            }
        }
    }
}


SCENARIO("Intersecting ray+csg doesn't test children if box is missed") {
    GIVEN("Given two shapes, a CSG, and a ray") {
        const std::shared_ptr<TestShape> left = std::make_shared<TestShape>();
        const std::shared_ptr<TestShape> right = std::make_shared<TestShape>();
        std::shared_ptr<CSG> shape = CSG::create(std::make_shared<csgDifferenceOperator>(), left, right);
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 1, 0));
        WHEN("When finding the intersection") {
            std::vector<Intersection> xs;
            shape->intersect(r, xs);
            THEN("It holds") {
                REQUIRE(left->local_ray.get_origin() == Point(0, 0, 0));
                REQUIRE(left->local_ray.get_direction() == Vector(0, 0, 0));
                REQUIRE(right->local_ray.get_origin() == Point(0, 0, 0));
                REQUIRE(right->local_ray.get_direction() == Vector(0, 0, 0));
            }
        }
    }
}

SCENARIO("Intersecting ray+csg tests children if box is hit") {
    GIVEN("Given two shapes, a CSG, and a ray") {
        const std::shared_ptr<TestShape> left = std::make_shared<TestShape>();
        const std::shared_ptr<TestShape> right = std::make_shared<TestShape>();
        std::shared_ptr<CSG> shape = CSG::create(std::make_shared<csgDifferenceOperator>(), left, right);
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        WHEN("When finding the intersection") {
            std::vector<Intersection> xs;
            shape->intersect(r, xs);
            THEN("It holds") {
                REQUIRE(left->local_ray.get_origin() == Point(0, 0, -5));
                REQUIRE(left->local_ray.get_direction() == Vector(0, 0, 1));
                REQUIRE(right->local_ray.get_origin() == Point(0, 0, -5));
                REQUIRE(right->local_ray.get_direction() == Vector(0, 0, 1));
            }
        }
    }
}

SCENARIO("Subdividing a CSG shape subdivides its children") {
    GIVEN("Given four shapes, two groups, and a CSG") {
        const std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
        s1->set_transform(Transform::translation(-1.5, 0, 0));
        const std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>();
        s2->set_transform(Transform::translation(1.5, 0, 0));
        const std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>();
        const std::shared_ptr<Group> left = std::make_shared<Group>();
        left->add_children(s1, s2);
        s3->set_transform(Transform::translation(0, 0, -1.5));
        const std::shared_ptr<Sphere> s4 = std::make_shared<Sphere>();
        s4->set_transform(Transform::translation(0, 0, 1.5));
        const std::shared_ptr<Group> right = std::make_shared<Group>();
        right->add_children(s3, s4);
        std::shared_ptr<CSG> shape = CSG::create(std::make_shared<csgDifferenceOperator>(), left, right);
        WHEN("When dividing") {
            shape->divide(1);
            THEN("It holds") {
                REQUIRE(std::dynamic_pointer_cast<Sphere>(std::dynamic_pointer_cast<Group>(left->operator[](0))->operator[](0)) == s1);
                REQUIRE(std::dynamic_pointer_cast<Sphere>(std::dynamic_pointer_cast<Group>(left->operator[](1))->operator[](0)) == s2);
                REQUIRE(std::dynamic_pointer_cast<Sphere>(std::dynamic_pointer_cast<Group>(right->operator[](0))->operator[](0)) == s3);
                REQUIRE(std::dynamic_pointer_cast<Sphere>(std::dynamic_pointer_cast<Group>(right->operator[](1))->operator[](0)) == s4);
            }
        }
    }
}
