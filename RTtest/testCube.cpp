#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Cube.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Material.hpp"
#include "Pattern.hpp"
#include "testHelper.hpp"


SCENARIO("A ray intersects a cube") {
    std::tuple<Tuple, Tuple, int, int> p[] = {{Point(5, 0.5, 0), Vector(-1, 0, 0), 4, 6}, // +x
                                              {Point(-5, 0.5, 0), Vector(1, 0, 0), 4, 6}, // -x
                                              {Point(0.5, 5, 0), Vector(0, -1, 0), 4, 6}, // +y
                                              {Point(0.5, -5, 0), Vector(0, 1, 0), 4, 6}, // -y
                                              {Point(0.5, 0, 5), Vector(0, 0, -1), 4, 6}, // +z
                                              {Point(0.5, 0, -5), Vector(0, 0, 1), 4, 6}, // -z
                                              {Point(0, 0.5, 0), Vector(0, 0, 1), -1, 1}}; // inside
    for (auto& [origin, direction, t1, t2] : p) {
        const std::shared_ptr<Cube> c = std::make_shared<Cube>();
        const Ray r = Ray(origin, direction);
        std::vector<Intersection> xs;
        c->intersect(r, xs);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].get_distance() == t1);
        REQUIRE(xs[1].get_distance() == t2);
    }
}

SCENARIO("A ray misses a cube") {
    std::tuple<Tuple, Tuple> p[] = {{Point(-2, 0, 0), Vector(0.2673, 0.5345, 0.8018)},
                                    {Point(0, -2, 0), Vector(0.8018, 0.2673, 0.5345)},
                                    {Point(0, 0, -2), Vector(0.5345, 0.8018, 0.2673)},
                                    {Point(2, 0, 2), Vector(0, 0, -1)},
                                    {Point(0, 2, 2), Vector(0, -1, 0)},
                                    {Point(2, 2, 0), Vector(-1, 0, 0)},
                                    {Point(0, 0, 2), Vector(0, 0, 1)}};
    for (auto& [origin, direction] : p) {
        const std::shared_ptr<Cube> c = std::make_shared<Cube>();
        const Ray r = Ray(origin, direction);
        std::vector<Intersection> xs;
        c->intersect(r, xs);
        REQUIRE(xs.size() == 0);
    }
}

SCENARIO("The normal on the surface of a cube") {
    std::tuple<Tuple, Tuple> p[] = {{Point(1, 0.5, -0.8), Vector(1, 0, 0)},
                                    {Point(-1, -0.2, 0.9), Vector(-1, 0, 0)},
                                    {Point(-0.4, 1, -0.1), Vector(0, 1, 0) },
                                    {Point(0.3, -1, -0.7), Vector(0, -1, 0)},
                                    {Point(-0.6, 0.3, 1), Vector(0, 0, 1)},
                                    {Point(0.4, 0.4, -1), Vector(0, 0, -1)},
                                    {Point(1, 1, 1), Vector(1, 0, 0)},
                                    {Point(-1, -1, -1), Vector(-1, 0, 0)}};
    for (auto& [point_, normal_] : p) {
        const std::shared_ptr<Cube> c = std::make_shared<Cube>();
        const Tuple point = point_;
        const Tuple normal = c->normal_at_local(point, Intersection(1, c));
        REQUIRE(normal == normal_);
    }
}

SCENARIO("A cube has a bounding box") {
    GIVEN("A cube") {
        std::shared_ptr<Cube> s = std::make_shared<Cube>();
        WHEN("Querying the bounding box") {
            const Bounds box = s->bounds;
            THEN("It should hold") {
                REQUIRE(box.min() == Point(-1, -1, -1));
                REQUIRE(box.max() == Point(1, 1, 1));
            }
        }
    }
}
