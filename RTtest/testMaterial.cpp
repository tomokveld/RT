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

SCENARIO("The default material") {
    GIVEN("A material") {
        const Material m = Material();
        THEN("It holds") {
            REQUIRE(m.get_color() == Color(1, 1, 1));
            REQUIRE(m.get_ambient() == Approx(0.1));
            REQUIRE(m.get_diffuse() == Approx(0.9));
            REQUIRE(m.get_specular() == Approx(0.9));
            REQUIRE(m.get_shininess() == Approx(200.0));
            REQUIRE(m.get_reflective() == Approx(0.0));
            REQUIRE(m.get_transparency() == Approx(0.0));
            REQUIRE(m.get_refractive_index() == Approx(1.0));
        }
    }
}
