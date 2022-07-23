#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Material.hpp"
#include "PointLight.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "testHelper.hpp"

SCENARIO("Constructing a camera") {
    GIVEN("A horizontal, vertical pixel size and a fov") {
        const uint32_t hsize = 160;
        const uint32_t vsize = 120;
        const float fov = M_PI_2;
        WHEN("Creating a new camera") {
            const Camera c = Camera(hsize, vsize, fov);
            THEN("It holds") {
                REQUIRE(c.get_hsize() == hsize);
                REQUIRE(c.get_vsize() == vsize);
                REQUIRE(c.get_fov() == fov);
                REQUIRE(c.get_transform() == Matrix<4, 4>::identity());
            }
        }
    }
}

SCENARIO("The pixel size for a horizontal canvas") {
    GIVEN("A camera") {
        const Camera c = Camera(200, 125, M_PI_2);
        THEN("It holds") {
            REQUIRE(c.get_pixel_size() == Approx(0.01).epsilon(EPSILON));
        }
    }
}

SCENARIO("The pixel size for a vertical canvas") {
    GIVEN("A camera") {
        const Camera c = Camera(125, 200, M_PI_2);
        THEN("It holds") {
            REQUIRE(c.get_pixel_size() == Approx(0.01).epsilon(EPSILON));
        }
    }
}

SCENARIO("Constructing a ray through the center of the canvas") {
    GIVEN("A camera") {
        const Camera c = Camera(201, 101, M_PI_2);
        WHEN("Create a ray using ray_for_pixel") {
            const Ray r = c.ray_for_pixel(100, 50);
            THEN("It holds") {
                REQUIRE(r.get_origin() == Point(0, 0, 0));
                REQUIRE(r.get_direction() == Vector(0, 0, -1));
            }
        }
    }
}

SCENARIO("Constructing a ray through a corner of the canvas") {
    GIVEN("A camera") {
        const Camera c = Camera(201, 101, M_PI_2);
        WHEN("Create a ray using ray_for_pixel") {
            const Ray r = c.ray_for_pixel(0, 0);
            THEN("It holds") {
                REQUIRE(r.get_origin() == Point(0, 0, 0));
                REQUIRE(r.get_direction() == Vector(0.66519, 0.33259, -0.66851));
            }
        }
    }
}

SCENARIO("Constructing a ray when the camera is transformed") {
    GIVEN("A camera") {
        Camera c = Camera(201, 101, M_PI_2);
        c.set_transform(Transform::rotation_y(M_PI_4) * Transform::translation(0, -2, 5));
        WHEN("Create a ray using ray_for_pixel") {
            const Ray r = c.ray_for_pixel(100, 50);
            THEN("It holds") {
                REQUIRE(r.get_origin() == Point(0, 2, -5));
                REQUIRE(r.get_direction() == Vector(sqrt(2)/2, 0, -sqrt(2)/2));
            }
        }
    }
}

SCENARIO("Rendering a world with a camera") {
    GIVEN("The default world, a camera, and tuples used to define a transform") {
        const World w = default_world();
        Camera c = Camera(11, 11, M_PI_2);
        const Tuple from = Point(0, 0, -5);
        const Tuple to = Point(0, 0, 0);
        const Tuple up = Point(0, 1, 0);
        c.set_transform(Transform::view_transform(from, to, up));
        WHEN("An image is rendered") {
            const Canvas image = c.render(w);
            THEN("It holds") {
                REQUIRE(image.get_pixel(5, 5) == Color(0.38066, 0.47583, 0.2855));
            }
        }
    }
}
