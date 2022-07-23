#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Material.hpp"

SCENARIO("The normal of a plane is constant everywhere") {
    GIVEN("A plane") {
        const std::shared_ptr<Plane> p = std::make_shared<Plane>();
        WHEN("Calculating the local norm for different points") {
            const Intersection i = Intersection(1, p);
            const Tuple n1 = p->normal_at_local(Point(0, 0, 0), i);
            const Tuple n2 = p->normal_at_local(Point(10, 0, -10), i);
            const Tuple n3 = p->normal_at_local(Point(-5, 0, 150), i);
            THEN("It should hold") {
                REQUIRE(n1 == Vector(0, 1, 0));
                REQUIRE(n2 == Vector(0, 1, 0));
                REQUIRE(n3 == Vector(0, 1, 0));
            }
        }
    }
}


SCENARIO("Intersect with a ray parallel to the plane") {
    GIVEN("A plane and a ray") {
        const std::shared_ptr<Plane> p = std::make_shared<Plane>();
        const Ray r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
        WHEN("Obtaining the intersections") {
            std::vector<Intersection> xs;
            p->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.empty());
            }
        }
    }
}

SCENARIO("Intersect with a coplanar ray") {
    GIVEN("A plane and a ray") {
        const std::shared_ptr<Plane> p = std::make_shared<Plane>();
        const Ray r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
        WHEN("Obtaining the intersections") {
            std::vector<Intersection> xs;
            p->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.empty());
            }
        }
    }
}

SCENARIO("A ray intersecting a plane from above") {
    GIVEN("A plane") {
        const std::shared_ptr<Plane> p = std::make_shared<Plane>();
        const Ray r = Ray(Point(0, 1, 0), Vector(0, -1, 0));
        WHEN("Obtaining the intersections") {
            std::vector<Intersection> xs;
            p->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 1);
                REQUIRE(xs[0].get_distance() == 1);
                REQUIRE(xs[0].get_shape() == p);
            }
        }
    }
}

SCENARIO("A ray intersecting a plane from below") {
    GIVEN("A plane") {
        const std::shared_ptr<Plane> p = std::make_shared<Plane>();
        const Ray r = Ray(Point(0, -1, 0), Vector(0, 1, 0));
        WHEN("Obtaining the intersections") {
            std::vector<Intersection> xs;
            p->intersect(r, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 1);
                REQUIRE(xs[0].get_distance() == 1);
                REQUIRE(xs[0].get_shape() == p);
            }
        }
    }
}

SCENARIO("A plane has a bounding box") {
    GIVEN("An empty bounding box") {
        const std::shared_ptr<Plane> p = std::make_shared<Plane>();
        WHEN("Querying the bounding box") {
            const Bounds box = p->bounds;
            THEN("It holds") {
                const Tuple min = box.min();
                const Tuple max = box.max();
                REQUIRE((min.get_x() == -INF && min.get_y() == 0 && min.get_z() == -INF));
                REQUIRE((max.get_x() == INF && max.get_y() == 0 && max.get_z() == INF));
            }
        }
    }
}
