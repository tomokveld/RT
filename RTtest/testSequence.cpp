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
#include "CSG.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"
#include "Sequence.hpp"
#include "Light.hpp"
#include "AreaLight.hpp"

SCENARIO("A number generator returns a cyclic sequence of numbers") {
    GIVEN("A sequence") {
        Sequence gen = Sequence({0.1f, 0.5f, 1.0f});
        THEN("It should hold") {
            REQUIRE(gen() == 0.1f);
            REQUIRE(gen() == 0.5f);
            REQUIRE(gen() == 1.0f);
            REQUIRE(gen() == 0.1f);
        }
    }
}

