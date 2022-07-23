#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Light.hpp"
#include "PointLight.hpp"
#include "AreaLight.hpp"
#include "Pattern.hpp"
#include "testHelper.hpp"
#include "Sequence.hpp"

SCENARIO("A point light has a position and intensity") {
    GIVEN("An color and a position") {
        const Color intensity = Color(1, 1, 1);
        const Tuple position = Point(0, 0, 0);
        WHEN("A PointLight is initialized") {
            std::shared_ptr<PointLight> light = std::make_shared<PointLight>(position, intensity);
            THEN("It should hold") {
                REQUIRE(light->position() == position);
                REQUIRE(light->intensity() == intensity);
            }
        }
    }
}

SCENARIO("Lighting with the eye between the light and the surface") {
    GIVEN("The constituents for Lighting") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Material m = Material();
        const Tuple position = Point(0, 0, 0);
        const Tuple eyev = Vector(0, 0, -1);
        const Tuple normalv = Vector(0, 0, -1);
        const std::shared_ptr<PointLight> light = std::make_shared<PointLight>(Point(0, 0, -10), Color(1, 1, 1));
        WHEN("Calling Lighting") {
            const Color result = Lighting(m, s, light, position, eyev, normalv);
            THEN("It holds") {
                REQUIRE(result == Color(1.9, 1.9, 1.9));
            }
        }
    }
}

SCENARIO("Lighting with the eye between light and surface, eye offset 45 degrees") {
    GIVEN("The constituents for Lighting") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Material m = Material();
        const Tuple position = Point(0, 0, 0);
        const Tuple eyev = Vector(0, sqrt(2)/2.0, -sqrt(2)/2.0);
        const Tuple normalv = Vector(0, 0, -1);
        const std::shared_ptr<PointLight> light = std::make_shared<PointLight>(Point(0, 0, -10), Color(1, 1, 1));
        WHEN("Calling Lighting") {
            Color result = Lighting(m, s, light, position, eyev, normalv);
            THEN("It holds") {
                REQUIRE(result == Color(1.0, 1.0, 1.0));
            }
        }
    }
}

SCENARIO("Lighting with eye opposite surface, light offset 45 degrees") {
    GIVEN("The constituents for Lighting") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Material m = Material();
        const Tuple position = Point(0, 0, 0);
        const Tuple eyev = Vector(0, 0, -1);
        const Tuple normalv = Vector(0, 0, -1);
        const std::shared_ptr<PointLight> light = std::make_shared<PointLight>(Point(0, 10, -10), Color(1, 1, 1));
        WHEN("Calling Lighting") {
            Color result = Lighting(m, s, light, position, eyev, normalv);
            THEN("It holds") {
                REQUIRE(result == Color(0.7364, 0.7364, 0.7364));
            }
        }
    }
}

SCENARIO("Lighting with eye in the path of the reflection vector") {
    GIVEN("The constituents for Lighting") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Material m = Material();
        const Tuple position = Point(0, 0, 0);
        const Tuple eyev = Vector(0, -sqrt(2)/2, -sqrt(2)/2);
        const Tuple normalv = Vector(0, 0, -1);
        const std::shared_ptr<PointLight> light = std::make_shared<PointLight>(Point(0, 10, -10), Color(1, 1, 1));
        WHEN("Calling Lighting") {
            Color result = Lighting(m, s, light, position, eyev, normalv);
            THEN("It holds") {
                // TODO: The value in the book is slightly different here.
                REQUIRE(result == Color(1.63639, 1.63639, 1.63639));
            }
        }
    }
}

SCENARIO("Lighting with the light behind the surface") {
    GIVEN("The constituents for Lighting") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Material m = Material();
        const Tuple position = Point(0, 0, 0);
        const Tuple eyev = Vector(0, 0, -1);
        const Tuple normalv = Vector(0, 0, -1);
        const std::shared_ptr<PointLight> light = std::make_shared<PointLight>(Point(0, 0, 10), Color(1, 1, 1));
        WHEN("Calling Lighting") {
            Color result = Lighting(m, s, light, position, eyev, normalv);
            THEN("It holds") {
                REQUIRE(result == Color(0.1, 0.1, 0.1));
            }
        }
    }
}

SCENARIO("Lighting with the surface in shadow") {
    GIVEN("The constituents for Lighting") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Material m = Material();
        const Tuple position = Point(0, 0, 0);
        const Tuple eyev = Vector(0, 0, -1);
        const Tuple normalv = Vector(0, 0, -1);
        const std::shared_ptr<PointLight> light = std::make_shared<PointLight>(Point(0, 0, -10), Color(1, 1, 1));
        const float light_intensity = 0.0;
        WHEN("Calling Lighting") {
            const Color result = Lighting(m, s, light, position, eyev, normalv, light_intensity);
            THEN("It holds") {
                REQUIRE(result == Color(0.1, 0.1, 0.1));
            }
        }
    }
}

SCENARIO("Lighting with a pattern applied") {
    GIVEN("The constituents for Lighting") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        Material m = Material();
        m.set_pattern(std::make_shared<PatternStripe>(Color(1, 1, 1), Color(0, 0, 0)));
        m.set_ambient(1);
        m.set_diffuse(0);
        m.set_specular(0);
        const Tuple eyev = Vector(0, 0, -1);
        const Tuple normalv = Vector(0, 0, -1);
        const std::shared_ptr<PointLight> light = std::make_shared<PointLight>(Point(0, 0, -10), Color(1, 1, 1));
        WHEN("Calling Lighting") {
            const Color c1 = Lighting(m, s, light, Point(0.9, 0, 0), eyev, normalv, false);
            const Color c2 = Lighting(m, s, light, Point(1.1, 0, 0), eyev, normalv, false);
            THEN("It holds") {
                REQUIRE(c1 == Color(1, 1, 1));
                REQUIRE(c2 == Color(0, 0, 0));
            }
        }
    }
}

SCENARIO("Creating an area light") {
    GIVEN("The constituents for Lighting") {
        const Tuple position = Point(0, 0, 0);
        const Tuple v1 = Vector(2, 0, 0);
        const Tuple v2 = Vector(0, 0, 1);
        WHEN("Creating an arealight") {
            const std::shared_ptr<AreaLight> light = std::make_shared<AreaLight>(position, Color(1, 1, 1), v1, v2, 4, 2);
            THEN("It holds") {
                REQUIRE(light->uvec() == Vector(0.5, 0, 0));
                REQUIRE(light->usteps() == 4);
                REQUIRE(light->vvec() == Vector(0, 0, 0.5));
                REQUIRE(light->vsteps() == 2);
                REQUIRE(light->n_samples() == 8);
                REQUIRE(light->position() == Point(1, 0, 0.5));
            }
        }
    }
}

SCENARIO("Finding a single point on an area light") {
    std::tuple<int, int, Tuple> p[] = {{0, 0, Point(0.25, 0, 0.25)},
                                        {1, 0, Point(0.75, 0, 0.25)},
                                        {0, 1, Point(0.25, 0, 0.75)},
                                        {2, 0, Point(1.25, 0, 0.25)},
                                        {3, 1, Point(1.75, 0, 0.75)}};
    Tuple position = Point(0, 0, 0);
    Tuple v1 = Vector(2, 0, 0);
    Tuple v2 = Vector(0, 0, 1);
    const std::shared_ptr<AreaLight> light = std::make_shared<AreaLight>(position, Color(1, 1, 1), v1, v2, 4, 2);
    for (auto &[u, v, result] : p) {
        REQUIRE(light->point_on_light(u, v) == result);
    }
}

SCENARIO("The area light intensity function") {
    std::tuple<Tuple, float> p[] = {{Point(0, 0, 2), 0.0},
                                        {Point(1, -1, 2), 0.25},
                                        {Point(1.5, 0, 2), 0.5},
                                        {Point(1.25, 1.25, 3), 0.75},
                                        {Point(0, 0, -2), 1.0},};
    World w = default_world();
    Tuple position = Point(-0.5, -0.5, -5);
    Tuple v1 = Vector(1, 0, 0);
    Tuple v2 = Vector(0, 1, 0);
    const std::shared_ptr<AreaLight> light = std::make_shared<AreaLight>(position, Color(1, 1, 1), v1, v2, 2, 2);
    for (auto &[point, result] : p) {
        REQUIRE(light->intensity_at(point, w) == result);
    }
}

SCENARIO("Finding a single point on a jittered area light") {
    std::tuple<int, int, Tuple> p[] = {{0, 0, Point(0.15, 0, 0.35)},
                                       {1, 0, Point(0.65, 0, 0.35)},
                                       {0, 1, Point(0.15, 0, 0.85)},
                                       {2, 0, Point(1.15, 0, 0.35)},
                                       {3, 1, Point(1.65, 0, 0.85)}};
    Tuple position = Point(0, 0, 0);
    Tuple v1 = Vector(2, 0, 0);
    Tuple v2 = Vector(0, 0, 1);
    const std::shared_ptr<AreaLight> light = std::make_shared<AreaLight>(position, Color(1, 1, 1), v1, v2, 4, 2);
    light->set_jitter(Sequence({0.3, 0.7}));
    for (auto& [u, v, res] : p) {
        REQUIRE(light->point_on_light(u, v) == res);
    }
}

SCENARIO("The area light with jittered samples") {
    std::tuple<Tuple, float> p[] = {{Point(0, 0, 2), 0.0},
                                    {Point(1, -1, 2), 0.5},
                                    {Point(1.5, 0, 2), 0.75},
                                    {Point(1.25, 1.25, 3), 0.75},
                                    {Point(0, 0, -2), 1.0}};
    World w = default_world();
    Tuple position = Point(-0.5, -0.5, -5);
    Tuple v1 = Vector(1, 0, 0);
    Tuple v2 = Vector(0, 1, 0);
    for (auto& [point, res] : p) {
        const std::shared_ptr<AreaLight> light = std::make_shared<AreaLight>(position, Color(1, 1, 1), v1, v2, 2, 2);
        light->set_jitter(Sequence({0.7, 0.3, 0.9, 0.1, 0.5}));
        REQUIRE(light->intensity_at(point, w) == res);
    }
}
