#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Cylinder.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Material.hpp"
#include "Pattern.hpp"
#include "testHelper.hpp"

SCENARIO("A ray misses a cylinder") {
    std::tuple<Tuple, Tuple> p[] = {{Point(1, 0, 0), Vector(0, 1, 0)},
                                    {Point(0, 0, 0), Vector(0, 1, 0)},
                                    {Point(0, 0, -5), Vector(1, 1, 1)}};
    for (auto& [origin, direction] : p) {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>();
        direction = direction.normalize();
        const Ray r = Ray(origin, direction);
        std::vector<Intersection> xs;
        c->intersect(r, xs);
        REQUIRE(xs.size() == 0);
    }
}

SCENARIO("A ray intersects a cylinder") {
    std::tuple<Tuple, Tuple, float, float> p[] = {{Point(1, 0, -5), Vector(0, 0, 1), 5, 5},
                                                  {Point(0, 0, -5), Vector(0, 0, 1), 4, 6},
                                                  {Point(0.5, 0, -5), Vector(0.1, 1, 1), 6.80798, 7.08872}};
    for (auto& [origin, direction, t1, t2] : p) {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>();
        direction = direction.normalize();
        const Ray r = Ray(origin, direction);
        std::vector<Intersection> xs;
        c->intersect(r, xs);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].get_distance() == Approx(t1));
        REQUIRE(xs[1].get_distance() == Approx(t2));
    }
}

SCENARIO("The normal on the surface of a cylinder") {
    std::tuple<Tuple, Tuple> p[] = {{Point(1, 0, 0), Vector(1, 0, 0)},
                                    {Point(0, 5, -1), Vector(0, 0, -1)},
                                    {Point(0, -2, 1), Vector(0, 0, 1)},
                                    {Point(-1, 1, 0), Vector(-1, 0, 0)}};
    for (auto& [point_, normal_] : p) {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>();
        const Tuple point = point_;
        const Tuple normal = c->normal_at_local(point, Intersection(1, c));
        REQUIRE(normal == normal_);
    }
}

SCENARIO("The default minimum and maximum for a cylinder") {
    GIVEN("A cylinder") {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>();
        THEN("It holds") {
            REQUIRE(c->get_min() == -INF);
            REQUIRE(c->get_max() == INF);
        }
    }
}

SCENARIO("Creating a constrained cylinder") {
    GIVEN("A cylinder") {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>(1, 2, false);
        THEN("It holds") {
            REQUIRE(c->get_min() == 1);
            REQUIRE(c->get_max() == 2);
        }
    }
}

SCENARIO("Intersecting a constrained cylinder") {
    std::tuple<Tuple, Tuple, int> p[] = {{Point(0, 1.5, 0), Vector(0.1, 1, 0), 0},
                                         {Point(0, 3, -5), Vector(0, 0, 1), 0},
                                         {Point(0, 0, -5), Vector(0, 0, 1), 0},
                                         {Point(0, 2, -5), Vector(0, 0, 1), 0},
                                         {Point(0, 1, -5), Vector(0, 0, 1), 0},
                                         {Point(0, 1.5, -2), Vector(0, 0, 1), 2}};
    for (auto& [origin, direction, count] : p) {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>(1, 2, false);
        const Ray r = Ray(origin, direction);
        std::vector<Intersection> xs;
        c->intersect(r, xs);
        REQUIRE(xs.size() == count);
    }
}

SCENARIO("The default closed value for a cylinder") {
    GIVEN("A cylinder") {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>();
        THEN("It holds") {
            REQUIRE(!c->get_capped());
        }
    }
}

SCENARIO("Intersecting the caps of a closed cylinder") {
    std::tuple<Tuple, Tuple, int> p[] = {{Point(0, 3, 0), Vector(0, -1, 0), 2},
                                         {Point(0, 3, -2), Vector(0, -1, 2), 2},
                                         {Point(0, 4, -2), Vector(0, -1, 1), 2},
                                         {Point(0, 0, -2), Vector(0, 1, 2), 2},
                                         {Point(0, -1, -2), Vector(0, 1, 1), 2}};
    for (auto& [origin, direction, count] : p) {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>(1, 2, true);
        const Ray r = Ray(origin, direction);
        std::vector<Intersection> xs;
        c->intersect(r, xs);
        REQUIRE(xs.size() == count);
    }
}

SCENARIO("The normal vector on a cylinder's end caps") {
    std::tuple<Tuple, Tuple> p[] = {{Point(0, 1, 0), Vector(0, -1, 0)},
                                    {Point(0.5, 1, 0), Vector(0, -1, 0)},
                                    {Point(0, 1, 0.5), Vector(0, -1, 0)},
                                    {Point(0, 2, 0), Vector(0, 1, 0)},
                                    {Point(0.5, 2, 0), Vector(0, 1, 0)},
                                    {Point(0, 2, 0.5), Vector(0, 1, 0)}};
    for (auto& [point_, normal_] : p) {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>(1, 2, true);
        const Tuple point = point_;
        const Tuple normal = c->normal_at_local(point, Intersection(1, c));
        REQUIRE(normal == normal_);
    }
}

SCENARIO("An unbounded cylinder has a bounding box") {
    GIVEN("An empty bounding box") {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>();
        WHEN("Querying the bounding box") {
            const Bounds box = c->bounds;
            THEN("It holds") {
                const Tuple min = box.min();
                const Tuple max = box.max();
                REQUIRE((min.get_x() == -1 && min.get_y() == -INF && min.get_z() == -1));
                REQUIRE((max.get_x() == 1 && max.get_y() == INF && max.get_z() == 1));
            }
        }
    }
}

SCENARIO("A bounded cylinder has a bounding box") {
    GIVEN("An empty bounding box") {
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>(-5, 3, false);
        WHEN("Querying the bounding box") {
            const Bounds box = c->bounds;
            THEN("It holds") {
                REQUIRE(box.min() == Point(-1, -5, -1));
                REQUIRE(box.max() == Point(1, 3, 1));
            }
        }
    }
}
