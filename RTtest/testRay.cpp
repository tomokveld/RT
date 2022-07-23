#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"

SCENARIO("Ray: Creating and querying a ray") {
    GIVEN("An origin point and a direction vector") {
        Tuple origin = Point(1, 2, 3);
        Tuple direction = Vector(4, 5, 6);
        WHEN("Creating a Ray object") {
            Ray r = Ray(origin, direction);
            THEN("It holds") {
                REQUIRE(r.get_direction() == direction);
                REQUIRE(r.get_origin() == origin);
            }
        }
    }
}

SCENARIO("Ray: Computing a point from a distance") {
    GIVEN("A Ray") {
        Ray r = Ray(Point(2, 3, 4), Vector(1, 0, 0));
        THEN("Calling distance for different values of t yields a position where the origin moves to") {
            REQUIRE(r.position(0) == Point(2, 3, 4));
            REQUIRE(r.position(1) == Point(3, 3, 4));
            REQUIRE(r.position(-1) == Point(1, 3, 4));
            REQUIRE(r.position(2.5) == Point(4.5, 3, 4));
        }
    }
}

SCENARIO("Ray: A ray intersects a sphere at two points") {
    GIVEN("A Ray and a sphere") {
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Ray intersects the sphere") {
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 2);
                REQUIRE(xs[0].get_distance() == 4.0);
                REQUIRE(xs[1].get_distance() == 6.0);
            }
        }
    }
}

SCENARIO("Ray: A ray intersects a sphere at a tangent") {
    GIVEN("A Ray and a sphere") {
        const Ray r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Ray intersects the sphere") {
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 2);
                REQUIRE(xs[0].get_distance() == 5.0);
                REQUIRE(xs[1].get_distance() == 5.0);
            }
        }
    }
}

SCENARIO("Ray: A ray misses a sphere") {
    GIVEN("A Ray and a sphere") {
        const Ray r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Trying to intersect the ray with the sphere") {
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 0);
            }
        }
    }
}

SCENARIO("Ray: A ray originates inside a sphere") {
    GIVEN("A Ray and a sphere") {
        const Ray r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Trying to intersect the ray with the sphere") {
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 2);
                REQUIRE(xs[0].get_distance() == -1.0);
                REQUIRE(xs[1].get_distance() == 1.0);
            }
        }
    }
}

SCENARIO("Ray: A sphere is behind a ray") {
    GIVEN("A Ray and a sphere") {
        const Ray r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Trying to intersect the ray with the sphere") {
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 2);
                REQUIRE(xs[0].get_distance() == -6.0);
                REQUIRE(xs[1].get_distance() == -4.0);
            }
        }
    }
}

SCENARIO("Ray: Translating a ray") {
    GIVEN("A Ray and a transformation") {
        const Ray r = Ray(Point(1, 2, 3), Vector(0, 1, 0));
        Matrix<4, 4> m = Transform::translation(3, 4, 5);
        WHEN("Translating the ray") {
            Ray r2 = m * r;
            THEN("It holds") {
                REQUIRE(r2.get_origin() == Point(4, 6, 8));
                REQUIRE(r2.get_direction() == Vector(0, 1, 0));
            }
        }
    }
}

SCENARIO("Ray: Scaling a ray") {
    GIVEN("A Ray and a transformation") {
        const Ray r = Ray(Point(1, 2, 3), Vector(0, 1, 0));
        Matrix<4, 4> m = Transform::scaling(2, 3, 4);
        WHEN("Translating the ray") {
            Ray r2 = m * r;
            THEN("It holds") {
                REQUIRE(r2.get_origin() == Point(2, 6, 12));
                REQUIRE(r2.get_direction() == Vector(0, 3, 0));
            }
        }
    }
}

