#include "catch.hpp"
#include "Color.hpp"
#include "Helper.hpp"

TEST_CASE("Color: Colors are (red, green, blue) tuples")  {
    Color c = Color(-0.5, 0.4, 1.7);
    REQUIRE(c.red() == Approx(-0.5).epsilon(EPSILON));
    REQUIRE(c.green() == Approx(0.4).epsilon(EPSILON));
    REQUIRE(c.blue() == Approx(1.7).epsilon(EPSILON));
}

TEST_CASE("Color: Adding colors") {
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);
    REQUIRE(c1 + c2 == Color(1.6, 0.7, 1.0));
}

TEST_CASE("Color: Subtracting colors") {
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);
    REQUIRE(c1 - c2 == Color(0.2, 0.5, 0.5));
}

TEST_CASE("Color: Multiplying a color by a scalar") {
    Color c = Color(0.2, 0.3, 0.4);
    REQUIRE(c * 2 == Color(0.4, 0.6, 0.8));
}

TEST_CASE("Color: Multiplying colors") {
    Color c1 = Color(1, 0.2, 0.4);
    Color c2 = Color(0.9, 1, 0.1);
    REQUIRE(c1 * c2 == Color(0.9, 0.2, 0.04));
}
