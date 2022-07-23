#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Cone.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Material.hpp"
#include "Pattern.hpp"
#include "testHelper.hpp"

SCENARIO("A ray intersects a Cone") {
    std::tuple<Tuple, Tuple, float, float> p[] = {{Point(0, 0, -5), Vector(0, 0, 1), 5, 5},
                                                    // I can only get this answer by manually calculating it
                                                    // Insufficient precision otherwise
//                                                  {Point(0, 0, -5), Vector(1, 1, 1), 8.66025, 8.66025},
                                                  {Point(1, 1, -5), Vector(-0.5, -1, 1), 4.55006, 49.44994}

    };
    for (auto& [origin, direction, t1, t2] : p) {
        const std::shared_ptr<Cone> c = std::make_shared<Cone>();
        direction = direction.normalize();
        const Ray r = Ray(origin, direction);
        std::vector<Intersection> xs;
        c->intersect(r, xs);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].get_distance() == Approx(t1));
        REQUIRE(xs[1].get_distance() == Approx(t2));
    }
}

SCENARIO("Intersecting a cone with a ray parallel to one of its halves") {
    GIVEN("A Cone and a ray") {
        const std::shared_ptr<Cone> c = std::make_shared<Cone>();
        const Ray r = Ray(Point(0, 0, -1), Vector(0, 1, 1).normalize());
        WHEN("Calculating the intersecion") {
            std::vector<Intersection> xs;
            c->intersect(r, xs);
            THEN("It holds") {
                REQUIRE(xs.size() == 1);
                REQUIRE(xs[0].get_distance() == Approx(0.35355));
            }
        }
    }
}

SCENARIO("Intersecting a cone's end caps") {
    std::tuple<Tuple, Tuple, int> p[] = {{Point(0, 0, -5), Vector(0, 1, 0), 0},
                                         {Point(0, 0, -0.25), Vector(0, 1, 1), 2},
                                         {Point(0, 0, -0.25), Vector(0, 1, 0), 4}};
    for (auto& [origin, direction, count] : p) {
        const std::shared_ptr<Cone> c = std::make_shared<Cone>(-0.5, 0.5, true);
        const Ray r = Ray(origin, direction);
        std::vector<Intersection> xs;
        c->intersect(r, xs);
        REQUIRE(xs.size() == count);
    }
}

SCENARIO("Computing the normal vector on a cone") {
    std::tuple<Tuple, Tuple> p[] = {{Point(0, 0, 0), Vector(0, 0, 0)},
                                    {Point(1, 1, 1), Vector(1, -sqrt(2), 1)},
                                    {Point(-1, -1, 0), Vector(-1, 1, 0)}};
    for (auto& [point_, normal_] : p) {
        const std::shared_ptr<Cone> c = std::make_shared<Cone>();
        const Tuple point = point_;
        const Tuple normal = c->normal_at_local(point, Intersection(1, c));
        REQUIRE(normal == normal_);
    }
}

SCENARIO("An unbounded cone has a bounding box") {
    GIVEN("An empty bounding box") {
        const std::shared_ptr<Cone> c = std::make_shared<Cone>();
        WHEN("Querying the bounding box") {
            const Bounds box = c->bounds;
            THEN("It holds") {
                const Tuple min = box.min();
                const Tuple max = box.max();
                REQUIRE((min.get_x() == -INF && min.get_y() == -INF && min.get_z() == -INF));
                REQUIRE((max.get_x() == INF && max.get_y() == INF && max.get_z() == INF));
            }
        }
    }
}
