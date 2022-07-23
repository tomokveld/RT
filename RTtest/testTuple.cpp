#include "catch.hpp"
#include "Tuple.hpp"
#include "Helper.hpp"

SCENARIO("An empty tuple is a vector") {
    GIVEN("An empty tuple") {
        Tuple t;
        THEN ("Is a vector") {
            REQUIRE(t.isPoint() == false);
            REQUIRE(t.isVector() == true);
        }
        THEN ("All values are zero") {
            REQUIRE(t == Tuple(0, 0, 0, 0));
        }
    }
}

TEST_CASE("Tuple: A tuple with w=1.0 is a point")  {
    Tuple t{0, 0, 0, 1};
    REQUIRE(t.isPoint());
    REQUIRE(!t.isVector());
}

TEST_CASE("Tuple: A tuple with w=0 is a vector")  {
    Tuple t{0, 0, 0, 0};
    REQUIRE(!t.isPoint());
    REQUIRE(t.isVector());
}

TEST_CASE("Tuple: point() creates tuples with w=1") {
    Tuple t = Point(0, 0, 0);
    REQUIRE(t.isPoint());
    REQUIRE(!t.isVector());
}

TEST_CASE("Tuple: vector() creates tuples with w=0") {
    Tuple t = Vector(0, 0, 0);
    REQUIRE(t.isVector());
    REQUIRE(!t.isPoint());
}

TEST_CASE("Tuple: == to test equivalence with the same tuple") {
    Tuple t = Vector(0, 0, 0);
    REQUIRE(t == t);
}

TEST_CASE("Tuple: == to test equivalence with a different tuple") {
    Tuple t = Vector(0, 0, 0);
    Tuple r = Vector(0, 0, 0);
    REQUIRE(t == r);
}

TEST_CASE("Tuple: == to test equivalence with different tuples") {
    Tuple t = Vector(15.32, 23.2, 32.2);
    Tuple r = Vector(15.32, 23.2, 32.2);
    REQUIRE(t == r);
}

//TEST_CASE("Tuple: != to test non-equivalence just outside epsilon margin") {
//    Tuple t = Vector(15, 20, 20);
//    Tuple r = Vector(15.000011, 20, 20);
//    REQUIRE(t != r);
//}

TEST_CASE("Tuple: != to test non-equivalence with tuple and point") {
    Tuple t = Vector(15.32, 23.2, 32.2);
    Tuple r = Point(15.32, 23.2, 32.2);
    REQUIRE(t != r);
}

TEST_CASE("Tuple: + Adding two tuples") {
    Tuple t = Tuple(3, -2, 5, 1);
    Tuple r = Tuple(-2, 3, 1, 0);
    REQUIRE(t+r == Tuple(1, 1, 6, 1));
}

TEST_CASE("Tuple: + Adding two tuples other direction") {
    Tuple t = Tuple(3, -2, 5, 1);
    Tuple r = Tuple(-2, 3, 1, 0);
    REQUIRE(r+t == Tuple(1, 1, 6, 1));
}

TEST_CASE("Tuple: - Subtracting two points") {
    Tuple t = Point(3, 2, 1);
    Tuple r = Point(5, 6, 7);
    REQUIRE(t-r == Vector(-2, -4, -6));
}

TEST_CASE("Tuple: - Subtracting a vector from a point") {
    Tuple t = Point(3, 2, 1);
    Tuple r = Vector(5, 6, 7);
    REQUIRE(t-r == Point(-2, -4, -6));
}

TEST_CASE("Tuple: - Subtracting two vectors") {
    Tuple t = Vector(3, 2, 1);
    Tuple r = Vector(5, 6, 7);
    REQUIRE(t-r == Vector(-2, -4, -6));
}

TEST_CASE("Tuple: - Subtracting a vector from the zero vector") {
    Tuple t = Vector(0, 0, 0);
    Tuple r = Vector(1, -2, 3);
    REQUIRE(t-r == Vector(-1, 2, -3));
}

TEST_CASE("Tuple: - Negating a tuple") {
    Tuple t = Tuple(1, -2, 3, -4);
    Tuple mt = -t;
    REQUIRE(-mt == t);
}

TEST_CASE("Tuple: - Negating a vector and adding result should equal the zero vector {0,0,0}") {
    Tuple t = Vector(1, 2, 3);
    Tuple p = -t;
    REQUIRE((p + t) == Vector(0, 0, 0));
}

TEST_CASE("Tuple: - Multiplying a tuple by a scalar") {
    Tuple t = Tuple(1, -2, 3, -4);
    REQUIRE(t * 3.5 == Tuple(3.5, -7, 10.5, -14));
}

TEST_CASE("Tuple: - Multiplying a tuple by a fraction") {
    Tuple t = Tuple(1, -2, 3, -4);
    REQUIRE(t * 0.5 == Tuple(0.5, -1, 1.5, -2));
}

TEST_CASE("Tuple: - Dividing a tuple by a scalar") {
    Tuple t = Tuple(1, -2, 3, -4);
    REQUIRE(t / 2 == Tuple(0.5, -1, 1.5, -2));
}

TEST_CASE("Tuple: - Compute the magnitude of vector (1, 0, 0)") {
    Tuple t = Vector(1, 0, 0);
    REQUIRE(t.magnitude() == 1);
}

TEST_CASE("Tuple: - Compute the magnitude of vector (0, 1, 0)") {
    Tuple t = Vector(0, 1, 0);
    REQUIRE(t.magnitude() == 1);
}

TEST_CASE("Tuple: - Compute the magnitude of vector (0, 0, 1)") {
    Tuple t = Vector(0, 0, 1);
    REQUIRE(t.magnitude() == 1);
}

TEST_CASE("Tuple: - Compute the magnitude of vector (1, 2, 3)") {
    Tuple t = Vector(1, 2, 3);
    Approx r = Approx(std::sqrt(14)).epsilon(EPSILON);
    REQUIRE(t.magnitude() == r);
}

TEST_CASE("Tuple: - Compute the magnitude of vector (-1, -2, -3)") {
    Tuple t = Vector(-1, -2, -3);
    Approx r = Approx(std::sqrt(14)).epsilon(EPSILON);
    REQUIRE(t.magnitude() == r);
}

TEST_CASE("Tuple: - Normalizing vector(4, 0, 0) gives (1, 0, 0)") {
    Tuple t = Vector(4, 0, 0);
    REQUIRE(t.normalize() == Vector(1, 0, 0));
}

TEST_CASE("Tuple: - Normalizing vector(1, 2, 3)") {
    Tuple t = Vector(1, 2, 3);
    REQUIRE(t.normalize() == t / t.magnitude());
}

TEST_CASE("Tuple: - The magnitude of a normalized vector") {
    Tuple t = Vector(1, 2, 3);
    Approx p = Approx(t.normalize().magnitude()).epsilon(EPSILON);
    REQUIRE(p == 1);
}

TEST_CASE("Tuple: - The dot product of two tuples") {
    Tuple v = Vector(1, 2, 3);
    Tuple r = Vector(2, 3, 4);
    REQUIRE(v.dot(r) == 20);
}

TEST_CASE("Tuple: - The cross product of two vectors I") {
    Tuple v = Vector(1, 2, 3);
    Tuple r = Vector(2, 3, 4);
    REQUIRE(v.cross(r) == Vector(-1, 2, -1));
}

TEST_CASE("Tuple: - The cross product of two vectors II") {
    Tuple v = Vector(1, 2, 3);
    Tuple r = Vector(2, 3, 4);
    REQUIRE(r.cross(v) == Vector(1, -2, 1));
}

SCENARIO("Tuple: Reflecting a vector approaching at 45 degrees") {
    GIVEN("Two vectors v and n") {
        const Tuple v = Vector(1, -1, 0);
        const Tuple n = Vector(0, 1, 0);
        WHEN("The vector v is reflected with n") {
            const Tuple r = v.reflect(n);
            THEN("It must hold") {
                REQUIRE(r == Vector(1, 1, 0));
            }
        }
    }
}

SCENARIO("Tuple: Reflecting a vector off a slanted surface") {
    GIVEN("Two vectors v and n") {
        const Tuple v = Vector(0, -1, 0);
        const Tuple n = Vector(sqrt(2)/2, sqrt(2)/2, 0);
        WHEN("The vector v is reflected with n") {
            const Tuple r = v.reflect(n);
            THEN("It must hold") {
                REQUIRE(r == Vector(1, 0, 0));
            }
        }
    }
}
