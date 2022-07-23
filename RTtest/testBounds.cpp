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
#include "Bounds.hpp"

SCENARIO("Creating an empty bounding box") {
    GIVEN("An empty bounding box") {
        const Bounds box = Bounds();
        THEN("It holds") {
            const Tuple min = box.min();
            const Tuple max = box.max();
            // The equal function used for Tuple equivalence using subtraction with an epsilon, which doesn't work nicely with infinity, workaround.
            REQUIRE((min.get_x() == INF && min.get_y() == INF && min.get_z() == INF));
            REQUIRE((max.get_x() == -INF && max.get_y() == -INF && max.get_z() == -INF));

        }
    }
}

SCENARIO("Creating a bounding box with volume") {
    GIVEN("An bounding box") {
        const Bounds box = Bounds(Point(-1, -2, -3), Point(3, 2, 1));
        THEN("It holds") {
            REQUIRE(box.min() == Point(-1, -2, -3));
            REQUIRE(box.max() == Point(3, 2, 1));
        }
    }
}

SCENARIO("Adding points to an empty bounding box") {
    GIVEN("An empty bounding box and two points") {
        Bounds box = Bounds();
        const Tuple p1 = Point(-5, 2, 0);
        const Tuple p2 = Point(7, 0, -3);
        WHEN("Adding the points to the bounding box") {
            box.update(p1);
            box.update(p2);
            THEN("It holds") {
                REQUIRE(box.min() == Point(-5, 0, -3));
                REQUIRE(box.max() == Point(7, 2, 0));
            }
        }
        
    }
}

SCENARIO("Adding one bounding box to another") {
    GIVEN("Given two bounding boxes") {
        Bounds box1 = Bounds(Point(-5, -2, 0), Point(7, 4, 4));
        Bounds box2 = Bounds(Point(8, -7, -2), Point(14, 2, 8));
        WHEN("Adding one bounding box to another") {
            box1.merge(box2);
            THEN("It holds") {
                REQUIRE(box1.min() == Point(-5, -7, -2));
                REQUIRE(box1.max() == Point(14, 4, 8));
            }
        }
    }
}

SCENARIO("Checking to see if a box contains a given point") {
    std::tuple<Tuple, bool> p[] = {{Point(5, -2, 0), true},
                                   {Point(11, 4, 7), true},
                                   {Point(8, 1, 3), true},
                                   {Point(3, 0, 3), false},
                                   {Point(8, -4, 3), false},
                                   {Point(8, 1, -1), false},
                                   {Point(13, 1, 3), false},
                                   {Point(8, 5, 3), false},
                                   {Point(8, 1, 8), false}};
    for (auto &[point, result] : p) {
        Bounds box = Bounds(Point(5, -2, 0), Point(11, 4, 7));
        REQUIRE(box.contains_point(point) == result);
    }
}

SCENARIO("Checking to see if a box contains a given box") {
    std::tuple<Tuple, Tuple, bool> p[] = {{Point(5, -2, 0), Point(11, 4, 7), true},
                                          {Point(6, -1, 1), Point(10, 3, 6), true},
                                          {Point(4, -3, -1), Point(10, 3, 6), false},
                                          {Point(6, -1, 1), Point(12, 5, 8), false}};
    for (auto &[min, max, result] : p) {
        Bounds box1 = Bounds(Point(5, -2, 0), Point(11, 4, 7));
        Bounds box2 = Bounds(min, max);
        REQUIRE(box1.contains_bounds(box2) == result);

    }
}

SCENARIO("Transforming a bounding box") {
    GIVEN("Given a bounding box and a matrix") {
        Bounds box1 = Bounds(Point(-1, -1, -1), Point(1, 1, 1));
        Matrix<4, 4> matrix = Transform::rotation_x(M_PI_4) * Transform::rotation_y(M_PI_4);
        WHEN("Multiplying a matrix with a bounding box") {
            Bounds box2 = box1 * matrix;
            THEN("It holds") {
                REQUIRE(box2.min() == Point(-1.41421, -1.70711, -1.70711));
                REQUIRE(box2.max() == Point(1.4142, 1.7071, 1.7071));
            }
        }
    }
}

SCENARIO("Intersecting a ray with a bounding box at the origin") {
    std::tuple<Tuple, Tuple, bool> p[] = {{Point(5, 0.5, 0), Vector(-1, 0, 0), true},
                                          {Point(-5, 0.5, 0), Vector(1, 0, 0), true},
                                          {Point(0.5, 5, 0), Vector(0, -1, 0), true},
                                          {Point(0.5, -5, 0), Vector(0, 1, 0), true},
                                          {Point(0.5, 0, 5), Vector(0, 0, -1), true},
                                          {Point(0.5, 0, -5), Vector(0, 0, 1), true},
                                          {Point(0, 0.5, 0), Vector(0, 0, 1), true},
                                          {Point(-2, 0, 0), Vector(2, 4, 6), false},
                                          {Point(0, -2, 0), Vector(6, 2, 4), false},
                                          {Point(0, 0, -2), Vector(4, 6, 2), false},
                                          {Point(2, 0, 2), Vector(0, 0, -1), false},
                                          {Point(0, 2, 2), Vector(0, -1, 0), false},
                                          {Point(2, 2, 0), Vector(-1, 0, 0), false}};
    for (auto &[origin, direction, result] : p) {
        Bounds box = Bounds(Point(-1, -1, -1), Point(1, 1, 1));
        direction = direction.normalize();
        const Ray r = Ray(origin, direction);
        REQUIRE(box.intersects(r) == result);
    }
}

SCENARIO("Intersecting a ray with a non-cubic bounding box") {
    std::tuple<Tuple, Tuple, bool> p[] = {{Point(15, 1, 2), Vector(-1, 0, 0), true},
                                          {Point(-5, -1, 4), Vector(1, 0, 0), true},
                                          {Point(7, 6, 5), Vector(0, -1, 0), true},
                                          {Point(9, -5, 6), Vector(0, 1, 0), true},
                                          {Point(8, 2, 12), Vector(0, 0, -1), true},
                                          {Point(6, 0, -5), Vector(0, 0, 1), true},
                                          {Point(8, 1, 3.5), Vector(0, 0, 1), true},
                                          {Point(9, -1, -8), Vector(2, 4, 6), false},
                                          {Point(8, 3, -4), Vector(6, 2, 4), false},
                                          {Point(9, -1, -2), Vector(4, 6, 2), false},
                                          {Point(4, 0, 9), Vector(0, 0, -1), false},
                                          {Point(8, 6, -1), Vector(0, -1, 0), false},
                                          {Point(12, 5, 4), Vector(-1, 0, 0), false}};
    for (auto &[origin, direction, result] : p) {
        Bounds box = Bounds(Point(5, -2, 0), Point(11, 4, 7));
        direction = direction.normalize();
        const Ray r = Ray(origin, direction);
        REQUIRE(box.intersects(r) == result);
    }
}

SCENARIO("Intersecting ray+group doesn't test children if box is missed") {
    GIVEN("Given a shape, group, and a ray") {
        const std::shared_ptr<TestShape> child = std::make_shared<TestShape>();
        const std::shared_ptr<Group> shape = std::make_shared<Group>();
        shape->add_child(child);
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 1, 0));
        WHEN("When finding the intersection") {
            std::vector<Intersection> xs;
            shape->intersect(r, xs);
            THEN("It holds") {
                REQUIRE(child->local_ray.get_origin() == Point(0, 0, 0));
                REQUIRE(child->local_ray.get_direction() == Vector(0, 0, 0));
            }
        }
    }
}

SCENARIO("Intersecting ray+group tests children if box is hit") {
    GIVEN("Given a shape, group, and a ray") {
        const std::shared_ptr<TestShape> child = std::make_shared<TestShape>();
        const std::shared_ptr<Group> shape = std::make_shared<Group>();
        shape->add_child(child);
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        WHEN("When finding the intersection") {
            std::vector<Intersection> xs;
            shape->intersect(r, xs);
            THEN("It holds") {
                REQUIRE(child->local_ray.get_origin() == Point(0, 0, -5));
                REQUIRE(child->local_ray.get_direction() == Vector(0, 0, 1));
            }
        }
    }
}

SCENARIO("Splitting a perfect cube") {
    GIVEN("Given a box") {
        const Bounds box = Bounds(Point(-1, -4, -5), Point(9, 6, 5));
        WHEN("Splitting the box") {
            Bounds left, right;
            box.split_bounds(&left, &right);
            THEN("It holds") {
                REQUIRE(left.min() == Point(-1, -4, -5));
                REQUIRE(left.max() == Point(4, 6, 5));
                REQUIRE(right.min() == Point(4, -4, -5));
                REQUIRE(right.max() == Point(9, 6, 5));
            }
        }
    }
}

SCENARIO("Splitting an x-wide box") {
    GIVEN("Given a box") {
        const Bounds box = Bounds(Point(-1, -2, -3), Point(9, 5.5, 3));
        WHEN("Splitting the box") {
            Bounds left, right;
            box.split_bounds(&left, &right);
            THEN("It holds") {
                REQUIRE(left.min() == Point(-1, -2, -3));
                REQUIRE(left.max() == Point(4, 5.5, 3));
                REQUIRE(right.min() == Point(4, -2, -3));
                REQUIRE(right.max() == Point(9, 5.5, 3));
            }
        }
    }
}

SCENARIO("Splitting an y-wide box") {
    GIVEN("Given a box") {
        const Bounds box = Bounds(Point(-1, -2, -3), Point(5, 8, 3));
        WHEN("Splitting the box") {
            Bounds left, right;
            box.split_bounds(&left, &right);
            THEN("It holds") {
                REQUIRE(left.min() == Point(-1, -2, -3));
                REQUIRE(left.max() == Point(5, 3, 3));
                REQUIRE(right.min() == Point(-1, 3, -3));
                REQUIRE(right.max() == Point(5, 8, 3));
            }
        }
    }
}

SCENARIO("Splitting an z-wide box") {
    GIVEN("Given a box") {
        const Bounds box = Bounds(Point(-1, -2, -3), Point(5, 3, 7));
        WHEN("Splitting the box") {
            Bounds left, right;
            box.split_bounds(&left, &right);
            THEN("It holds") {
                REQUIRE(left.min() == Point(-1, -2, -3));
                REQUIRE(left.max() == Point(5, 3, 2));
                REQUIRE(right.min() == Point(-1, -2, 2));
                REQUIRE(right.max() == Point(5, 3, 7));
            }
        }
    }
}
