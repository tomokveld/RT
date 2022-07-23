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
#include "SmoothTriangle.hpp"

const Tuple p1 = Point(0, 1, 0);
const Tuple p2 = Point(-1, 0, 0);
const Tuple p3 = Point(1, 0, 0);
const Tuple n1 = Vector(0, 1, 0);
const Tuple n2 = Vector(-1, 0, 0);
const Tuple n3 = Vector(1, 0, 0);
const std::shared_ptr<SmoothTriangle> tri = std::make_shared<SmoothTriangle>(p1, p2, p3, n1, n2, n3);

SCENARIO("Constructing a smooth triangle") {
    GIVEN("A smooth triangle") {
        THEN("It should hold") {
            REQUIRE(tri->p1() == p1);
            REQUIRE(tri->p2() == p2);
            REQUIRE(tri->p3() == p3);
            REQUIRE(tri->n1() == n1);
            REQUIRE(tri->n2() == n2);
            REQUIRE(tri->n3() == n3);
        }
    }
}

SCENARIO("An intersection with a smooth triangle stores u/v") {
    GIVEN("A smooth triangle") {
        WHEN("Getting the intersection") {
            const Ray r = Ray(Point(-0.2, 0.3, -2), Vector(0, 0, 1));
            std::vector<Intersection> xs;
            tri->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs[0].u() == Approx(0.45));
                REQUIRE(xs[0].v() == Approx(0.25));
            }
        }
    }
}

SCENARIO("A smooth triangle uses u/v to interpolate the normal") {
    GIVEN("A smooth triangle") {
        WHEN("Getting the intersection") {
            const Intersection i = Intersection(1, tri, 0.45, 0.25);
            const Tuple n = tri->normal_at(Point(0, 0, 0), i);
            THEN("It should hold") {
                REQUIRE(n == Vector(-0.5547, 0.83205, 0));
            }
        }
    }
}

SCENARIO("Preparing the normal on a smooth triangle") {
    GIVEN("A smooth triangle") {
        WHEN("Getting the intersection") {
            const Intersection i = Intersection(1, tri, 0.45, 0.25);
            const Ray r = Ray(Point(-0.2, 0.3, -2), Vector(0, 0, 1));
            std::vector<Intersection> xs;
            tri->intersect(r, xs);
            const IntersectionComp comps = r.prepare_computations(i);
            THEN("It should hold") {
                REQUIRE(comps.normalv == Vector(-0.5547, 0.83205, 0));
            }
        }
    }
}
