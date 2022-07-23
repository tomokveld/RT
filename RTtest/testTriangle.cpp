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
#include "Triangle.hpp"

SCENARIO("Constructing a triangle") {
    GIVEN("Three points used to construct a triangle") {
        const Tuple p1 = Point(0, 1, 0);
        const Tuple p2 = Point(-1, 0, 0);
        const Tuple p3 = Point(1, 0, 0);
        const std::shared_ptr<Triangle> t = std::make_shared<Triangle>(p1, p2, p3);
        THEN("It should hold") {
            REQUIRE(t->p1() == p1);
            REQUIRE(t->p2() == p2);
            REQUIRE(t->p3() == p3);
            REQUIRE(t->e1() == Vector(-1, -1, 0));
            REQUIRE(t->e2() == Vector(1, -1, 0));
            REQUIRE(t->normal() == Vector(0, 0, -1));
        }
    }
}

SCENARIO("Finding the normal on a triangle") {
    GIVEN("A triangle") {
        const std::shared_ptr<Triangle> t = std::make_shared<Triangle>(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
        WHEN("Calculating the normal") {
            const Intersection i = Intersection(1, t);
            const Tuple n1 = t->normal_at_local(Point(0, 0.5, 0), i);
            const Tuple n2 = t->normal_at_local(Point(-0.5, 0.75, 0), i);
            const Tuple n3 = t->normal_at_local(Point(0.5, 0.25, 0), i);
            THEN("It should hold") {
                REQUIRE(n1 == t->normal());
                REQUIRE(n2 == t->normal());
                REQUIRE(n3 == t->normal());
            }
        }
    }
}

SCENARIO("Intersecting a ray parallel to the triangle") {
    GIVEN("A triangle and a ray") {
        const std::shared_ptr<Triangle> t = std::make_shared<Triangle>(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
        const Ray r = Ray(Point(0, -1, -2), Vector(0, 1, 0));
        WHEN("Finding intersections") {
            std::vector<Intersection> xs;
            t->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.empty());
            }
        }
    }
}

SCENARIO("A ray misses the p1-p3 edge") {
    GIVEN("A triangle and a ray") {
        const std::shared_ptr<Triangle> t = std::make_shared<Triangle>(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
        const Ray r = Ray(Point(1, 1, -2), Vector(0, 0, 1));
        WHEN("Finding intersections") {
            std::vector<Intersection> xs;
            t->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.empty());
            }
        }
    }
}

SCENARIO("A ray misses the p1-p2 edge") {
    GIVEN("A triangle and a ray") {
        const std::shared_ptr<Triangle> t = std::make_shared<Triangle>(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
        const Ray r = Ray(Point(-1, 1, -2), Vector(0, 0, 1));
        WHEN("Finding intersections") {
            std::vector<Intersection> xs;
            t->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.empty());
            }
        }
    }
}

SCENARIO("A ray misses the p2-p3 edge") {
    GIVEN("A triangle and a ray") {
        const std::shared_ptr<Triangle> t = std::make_shared<Triangle>(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
        const Ray r = Ray(Point(0, -1, -2), Vector(0, 0, 1));
        WHEN("Finding intersections") {
            std::vector<Intersection> xs;
            t->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.empty());
            }
        }
    }
}

SCENARIO("A ray strikes a triangle") {
    GIVEN("A triangle and a ray") {
        const std::shared_ptr<Triangle> t = std::make_shared<Triangle>(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
        const Ray r = Ray(Point(0, 0.5, -2), Vector(0, 0, 1));
        WHEN("Finding intersections") {
            std::vector<Intersection> xs;
            t->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 1);
                REQUIRE(xs[0].get_distance() == 2);
            }
        }
    }
}

SCENARIO("A triangle has a bounding box") {
    GIVEN("Three points used to construct a triangle") {
        const Tuple p1 = Point(-3, 7, 2);
        const Tuple p2 = Point(6, 2, -4);
        const Tuple p3 = Point(2, -1, -1);
        const std::shared_ptr<Triangle> t = std::make_shared<Triangle>(p1, p2, p3);
        WHEN("Getting the bounds") {
            const Bounds box = t->bounds;
            THEN("It should hold") {
                REQUIRE(box.min() == Point(-3, -1, -4));
                REQUIRE(box.max() == Point(6, 7, 2));
            }
        }
    }
}
