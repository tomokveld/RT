#include "catch.hpp"
#include "Helper.hpp"
#include "Matrix.hpp"
#include "Transformations.hpp"

SCENARIO("Transformations: Multiplying by a translation matrix") {
    GIVEN("The transform matrix defined by (5, -3, 2) and a point p") {
        Matrix m = Transform::translation(5, -3, 2);
        Tuple p = Point(-3, 4, 5);
        THEN("Translating the point will move it to") {
            REQUIRE(m * p == Point(2, 1, 7));
        }
    }
}

SCENARIO("Transformations: Multiplying by the inverse of a translation matrix") {
    GIVEN("The inversed transform matrix defined by (5, -3, 2) and a point p") {
        Matrix m = Transform::translation(5, -3, 2).inverse();
        Tuple p = Point(-3, 4, 5);
        THEN("Translating the point will move it to") {
            REQUIRE(m * p == Point(-8, 7, 3));
        }
    }
}

SCENARIO("Transformations: Translation does not affect vectors") {
    GIVEN("The transform matrix m defined by (5, -3, 2) and a vector v") {
        Matrix m = Transform::translation(5, -3, 2).inverse();
        Tuple v = Vector(-3, 4, 5);
        THEN("Translating the point will move it to") {
            REQUIRE(m * v == Vector(-3, 4, 5));
        }
    }
}

SCENARIO("Transformations: A scaling matrix applied to a point") {
    GIVEN("A scaling matrix m and a point p") {
        Matrix m = Transform::scaling(2, 3, 4);
        Tuple p = Point(-4, 6, 8);
        THEN("Translating the point will move it to") {
            REQUIRE(m * p == Point(-8, 18, 32));
        }
    }
}

SCENARIO("Transformations: A scaling matrix applied to a vector") {
    GIVEN("A scaling matrix m and a vector v") {
        Matrix m = Transform::scaling(2, 3, 4);
        Tuple v = Vector(-4, 6, 8);
        THEN("Translating the point will move it to") {
            REQUIRE(m * v == Vector(-8, 18, 32));
        }
    }
}

SCENARIO("Transformations: Multiplication by the inverse of a scaling matrix") {
    GIVEN("A scaling matrix m and a vector v") {
        Matrix m = Transform::scaling(2, 3, 4);
        Matrix minv = m.inverse();
        Tuple v = Vector(-4, 6, 8);
        THEN("Translating the point will move it to") {
            REQUIRE(minv * v == Vector(-2, 2, 2));
        }
    }
}

SCENARIO("Transformations: Reflection is scaling by a negative value") {
    GIVEN("A scaling matrix m and a point p") {
        Matrix m = Transform::scaling(-1, 1, 1);
        Tuple p = Point(2, 3, 4);
        THEN("Translating the point will move it to") {
            REQUIRE(m * p == Point(-2, 3, 4));
        }
    }
}

SCENARIO("Transformations: Rotating a point around the x-axis") {
    GIVEN("A point and multiple rotation_x matrices") {
        Tuple p = Point(0, 1, 0);
        Matrix half_quarter = Transform::rotation_x(M_PI / 4.0);
        Matrix full_quarter = Transform::rotation_x(M_PI / 2.0);
        THEN("Rotating the point yields") {
            REQUIRE(half_quarter * p == Point(0, sqrt(2)/2, sqrt(2)/2));
            REQUIRE(full_quarter * p == Point(0, 0, 1));
        }
    }
}

SCENARIO("Transformations: The inverse of an x-rotation rotates in the opposite direction") {
    GIVEN("A point and a rotation_x matrix") {
        Tuple p = Point(0, 1, 0);
        Matrix half_quarter = Transform::rotation_x(M_PI / 4.0);
        THEN("Rotating the point using the inversed rotation_x matrix yields") {
            REQUIRE(half_quarter.inverse() * p == Point(0, sqrt(2)/2, -sqrt(2)/2));
        }
    }
}

SCENARIO("Transformations: Rotating a point around the y-axis") {
    GIVEN("A point and multiple rotation_y matrices") {
        Tuple p = Point(0, 0, 1);
        Matrix half_quarter = Transform::rotation_y(M_PI / 4.0);
        Matrix full_quarter = Transform::rotation_y(M_PI / 2.0);
        THEN("Rotating the point yields") {
            REQUIRE(half_quarter * p == Point(sqrt(2)/2, 0, sqrt(2)/2));
            REQUIRE(full_quarter * p == Point(1, 0, 0));
        }
    }
}

SCENARIO("Transformations: Rotating a point around the z-axis") {
    GIVEN("A point and multiple rotation_z matrices") {
        Tuple p = Point(0, 1, 0);
        Matrix half_quarter = Transform::rotation_z(M_PI / 4.0);
        Matrix full_quarter = Transform::rotation_z(M_PI / 2.0);
        THEN("Rotating the point yields") {
            REQUIRE(half_quarter * p == Point(-sqrt(2)/2, sqrt(2)/2, 0));
            REQUIRE(full_quarter * p == Point(-1, 0, 0));
        }
    }
}

SCENARIO("Transformations: A shearing transformation moves x in proportion to y") {
    GIVEN("A point and a shearing matrix") {
        Tuple p = Point(2, 3, 4);
        Matrix m = Transform::shearing(1, 0, 0, 0, 0, 0);
        THEN("Rotating the point yields") {
            REQUIRE(m * p == Point(5, 3, 4));
        }
    }
}

SCENARIO("Transformations: A shearing transformation moves x in proportion to z") {
    GIVEN("A point and a shearing matrix") {
        Tuple p = Point(2, 3, 4);
        Matrix m = Transform::shearing(0, 1, 0, 0, 0, 0);
        THEN("Rotating the point yields") {
            REQUIRE(m * p == Point(6, 3, 4));
        }
    }
}


SCENARIO("Transformations: A shearing transformation moves y in proportion to x") {
    GIVEN("A point and a shearing matrix") {
        Tuple p = Point(2, 3, 4);
        Matrix m = Transform::shearing(0, 0, 1, 0, 0, 0);
        THEN("Rotating the point yields") {
            REQUIRE(m * p == Point(2, 5, 4));
        }
    }
}

SCENARIO("Transformations: A shearing transformation moves y in proportion to z") {
    GIVEN("A point and a shearing matrix") {
        Tuple p = Point(2, 3, 4);
        Matrix m = Transform::shearing(0, 0, 0, 1, 0, 0);
        THEN("Rotating the point yields") {
            REQUIRE(m * p == Point(2, 7, 4));
        }
    }
}


SCENARIO("Transformations: A shearing transformation moves z in proportion to x") {
    GIVEN("A point and a shearing matrix") {
        Tuple p = Point(2, 3, 4);
        Matrix m = Transform::shearing(0, 0, 0, 0, 1, 0);
        THEN("Rotating the point yields") {
            REQUIRE(m * p == Point(2, 3, 6));
        }
    }
}

SCENARIO("Transformations: A shearing transformation moves z in proportion to y") {
    GIVEN("A point and a shearing matrix") {
        Tuple p = Point(2, 3, 4);
        Matrix m = Transform::shearing(0, 0, 0, 0, 0, 1);
        THEN("Rotating the point yields") {
            REQUIRE(m * p == Point(2, 3, 7));
        }
    }
}

SCENARIO("Transformations: Individual transformations are applied in sequence") {
    GIVEN("A point and multiple transformation matrices") {
        Tuple p = Point(1, 0, 1);
        Matrix A = Transform::rotation_x(M_PI / 2);
        Matrix B = Transform::scaling(5, 5, 5);
        Matrix C = Transform::translation(10, 5, 7);
        WHEN("Applying the transforms") {
            Tuple p2 = A * p;
            Tuple p3 = B * p2;
            Tuple p4 = C * p3;
            THEN("The points should equal") {
                REQUIRE(p2 == Point(1, -1, 0));
                REQUIRE(p3 == Point(5, -5, 0));
                REQUIRE(p4 == Point(15, 0, 7));
            }
        }
    }
}

SCENARIO("Transformations: Chained transformations must be applied in reverse order") {
    GIVEN("A point and multiple transformation matrices") {
        Tuple p = Point(1, 0, 1);
        Matrix A = Transform::rotation_x(M_PI / 2);
        Matrix B = Transform::scaling(5, 5, 5);
        Matrix C = Transform::translation(10, 5, 7);
        WHEN("Combining the transforms") {
            Matrix T = C * B * A;
            THEN("The points should equal") {
                REQUIRE(T * p == Point(15, 0, 7));
            }
        }
    }
}

SCENARIO("Transformations: The transformation matrix for the default orientation") {
    GIVEN("Two points and a vector") {
        const Tuple from = Point(0, 0, 0);
        const Tuple to = Point(0, 0, -1);
        const Tuple up = Vector(0, 1, 0);
        WHEN("Calling view_transform") {
            Matrix<4, 4> t = Transform::view_transform(from, to, up);
            THEN("It holds") {
                REQUIRE(t == Matrix<4, 4>::identity());
            }
        }
    }
}

SCENARIO("Transformations: A view transformation matrix looking in positive z direction") {
    GIVEN("Two points and a vector") {
        const Tuple from = Point(0, 0, 0);
        const Tuple to = Point(0, 0, 1);
        const Tuple up = Vector(0, 1, 0);
        WHEN("Calling view_transform") {
            Matrix<4, 4> t = Transform::view_transform(from, to, up);
            THEN("It holds") {
                REQUIRE(t == Transform::scaling(-1, 1, -1));
            }
        }
    }
}

SCENARIO("Transformations: The view transformation moves the world") {
    GIVEN("Two points and a vector") {
        const Tuple from = Point(0, 0, 8);
        const Tuple to = Point(0, 0, 0);
        const Tuple up = Vector(0, 1, 0);
        WHEN("Calling view_transform") {
            Matrix<4, 4> t = Transform::view_transform(from, to, up);
            THEN("It holds") {
                REQUIRE(t == Transform::translation(0, 0, -8));
            }
        }
    }
}

SCENARIO("Transformations: An arbitrary view transformation") {
    GIVEN("Two points and a vector") {
        const Tuple from = Point(1, 3, 2);
        const Tuple to = Point(4, -2, 8);
        const Tuple up = Vector(1, 1, 0);
        WHEN("Calling view_transform") {
            Matrix<4, 4> t = Transform::view_transform(from, to, up);
            THEN("It holds") {
                REQUIRE(t == Matrix<4, 4>({
                                {-0.50709, 0.50709, 0.67612, -2.36643},
                                {0.76772, 0.60609, 0.12122, -2.82843},
                                {-0.35857, 0.59761, -0.71714, 0.00000},
                                {0.00000, 0.00000, 0.00000, 1.00000}}));
            }
        }
    }
}
