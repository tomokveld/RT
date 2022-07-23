#include <stdio.h>
#include "catch.hpp"
#include "Helper.hpp"
#include "Matrix.hpp"

SCENARIO("Matrix: Constructing and inspecting a 4x4 matrix") {
    GIVEN("The following 4x4 matrix M") {
        Matrix<4, 4> M ({
            {1, 2, 3, 4},
            {5.5, 6.5, 7.5, 8.5},
            {9, 10, 11, 12},
            {13.5, 14.5, 15.5, 16.5}});
        THEN("The indices should equal") {
            REQUIRE(M[0][0] == 1);
            REQUIRE(M[0][3] == 4);
            REQUIRE(M[1][0] == 5.5);
            REQUIRE(M[1][2] == 7.5);
            REQUIRE(M[2][2] == 11);
            REQUIRE(M[3][0] == 13.5);
            REQUIRE(M[3][2] == 15.5);
        }
    }
}

SCENARIO("Matrix: A 2x2 matrix ought to be representable") {
    GIVEN("The following 2x2 matrix M") {
        Matrix<2, 2> M({
            {-3, 5},
            {1, -2}});
        THEN("The indices should equal") {
            REQUIRE(M[0][0] == -3);
            REQUIRE(M[0][1] == 5);
            REQUIRE(M[1][0] == 1);
            REQUIRE(M[1][1] == -2);
        }
    }
}

SCENARIO("Matrix: A 3x3 matrix ought to be representable") {
    GIVEN("The following 3x3 matrix M") {
        Matrix<3, 3> M({
            {-3, 5, 0},
            {1, -2, -7},
            {0, 1, 1}});
        THEN("The indices should equal") {
            REQUIRE(M[0][0] == -3);
            REQUIRE(M[1][1] == -2);
            REQUIRE(M[2][2] == 1);
        }
    }
}

SCENARIO("Matrix: Equality with identical matrices") {
    GIVEN("The following matrices A and B") {
        Matrix<4, 4> A({
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 8, 7, 6},
            {5, 4, 3, 2}});
        Matrix<4, 4> B({
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 8, 7, 6},
            {5, 4, 3, 2}});
        THEN("A == B") {
            REQUIRE(A == B);
        }
    }
}

SCENARIO("Matrix: Equality with different matrices") {
    GIVEN("The following matrices A and B") {
        Matrix<4, 4> A({
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 8, 7, 6},
            {5, 4, 3, 2}});
        Matrix<4, 4> B({
            {2, 3, 4, 5},
            {6, 7, 8, 9},
            {8, 7, 6, 5},
            {4, 3, 2, 1}});
        THEN("A != B") {
            REQUIRE(A != B);
        }
    }
}

SCENARIO("Matrix: Multiplying two matrices") {
    GIVEN("The following matrices A and B") {
        Matrix<4, 4> A({
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 8, 7, 6},
            {5, 4, 3, 2}});
        Matrix<4, 4> B({
            {-2, 1, 2, 3},
            {3, 2, 1, -1},
            {4, 3, 6, 5},
            {1, 2, 7, 8}});
        THEN("A * B is the following 4x4 matrix") {
            Matrix<4, 4> expected({
                {20, 22, 50, 48},
                {44, 54, 114, 108},
                {40, 58, 110, 102},
                {16, 26, 46, 42}});
            REQUIRE(A * B == expected);
        }
    }
}

SCENARIO("Matrix: A matrix multiplied by a tuple") {
    GIVEN("The following matrix A and tuple b") {
        Matrix<4, 4> A({
            {1, 2, 3, 4},
            {2, 4, 4, 2},
            {8, 6, 4, 1},
            {0, 0, 0, 1}});
        Tuple b = Tuple(1, 2, 3, 1);
        THEN("A * b equals") {
            REQUIRE(A * b == Tuple(18, 24, 33, 1));
        }
    }
}

SCENARIO("Matrix: A matrix multiplied by the identity matrix") {
    GIVEN("The following matrix A") {
        Matrix<4, 4> A({
            {0, 1, 2, 4},
            {1, 2, 4, 8},
            {2, 4, 8, 16},
            {4, 8, 16, 32}});
        THEN("A * I equals A") {
            REQUIRE(A * Matrix<4, 4>::identity() == A);
        }
    }
}

SCENARIO("Matrix: Multiplying the identity matrix by a tuple") {
    GIVEN("The following tuple a") {
        Tuple a = Tuple(1, 2, 3, 4);
        THEN("a * I equals a") {
            REQUIRE(Matrix<4, 4>::identity() * a == a);
        }
    }
}

SCENARIO("Matrix: Transposing a matrix in place") {
    GIVEN("The following matrix A") {
        Matrix<4, 4> A({
            {0, 9, 3, 0},
            {9, 8, 0, 8},
            {1, 8, 5, 3},
            {0, 0, 5, 8}});
        Matrix<4, 4> expected({
            {0, 9, 1, 0},
            {9, 8, 8, 0},
            {3, 0, 5, 5},
            {0, 8, 3, 8}});

        THEN("A transposed is the following matrix") {
            A.transpose();
            REQUIRE(A == expected);
        }
    }
}

SCENARIO("Matrix: Transposing a matrix") {
    GIVEN("The following matrix A") {
        Matrix<4, 4> A({
            {0, 9, 3, 0},
            {9, 8, 0, 8},
            {1, 8, 5, 3},
            {0, 0, 5, 8}});
        Matrix<4, 4> expected({
            {0, 9, 1, 0},
            {9, 8, 8, 0},
            {3, 0, 5, 5},
            {0, 8, 3, 8}});
        THEN("A transposed is the following matrix") {
            REQUIRE(A.transposed() == expected);
        }
    }
}


SCENARIO("Matrix: Transposing the identity matrix") {
    GIVEN("The identity matrix") {
        Matrix A = Matrix<4, 4>::identity();
        THEN("The identity matrix transposed should be the same matrix") {
            REQUIRE(A.transposed() == A);
        }
    }
}

SCENARIO("Matrix: Calculating the determinant of a 2x2 matrix") {
    GIVEN("The following 2x2 matrix A") {
        Matrix<2,2> A({
            {1, 5},
            {-3, 2}});
        THEN("The determinant of A is 17") {
            REQUIRE(A.determinant() == 17);
        }
    }
}

SCENARIO("Matrix: A submatrix of a 3x3 matrix is a 2x2 matrix") {
    GIVEN("The following 3x3 matrix A") {
        Matrix<3, 3> A({
            {1, 5, 0},
            {-3, 2, 7},
            {0, 6, -3}});
        Matrix<2, 2> expected({
            {-3, 2},
            {0, 6}});
        THEN("Submatrix (0, 2) is the following 2x2 matrix") {
            REQUIRE(A.submatrix(0, 2) == expected);
        }
    }
}


SCENARIO("Matrix: A submatrix of a 4x4 matrix is a 3x3 matrix") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {-6, 1, 1, 6},
            {-8, 5, 8, 6},
            {-1, 0, 8, 2},
            {-7, 1, -1, 1}});
        Matrix<3, 3> expected({
            {-6, 1, 6},
            {-8, 8, 6},
            {-7, -1, 1}});
        THEN("Submatrix (2, 1) is the following 3x3 matrix") {
            REQUIRE(A.submatrix(2, 1) == expected);
        }
    }
}


SCENARIO("Matrix: Calculating a minor of a 3x3 matrix") {
    GIVEN("The following 3x3 matrix A") {
        Matrix<3, 3> A({
            {3, 5, 0},
            {2, -1, -7},
            {6, -1, 5}});
        Matrix B = A.submatrix(1, 0);
        THEN("Determinant of B = 25 and minor of A on (1, 0) is 25") {
            REQUIRE(B.determinant() == 25);
            REQUIRE(A.minor(1, 0) == 25);
        }
    }
}

SCENARIO("Matrix: Calculating a cofactor of a 3x3 matrix") {
    GIVEN("The following 3x3 matrix A") {
        Matrix<3, 3> A({
            {3, 5, 0},
            {2, -1, -7},
            {6, -1, 5}});
        THEN("It must hold") {
            REQUIRE(A.minor(0, 0) == -12);
            REQUIRE(A.cofactor(0, 0) == -12);
            REQUIRE(A.minor(1, 0) == 25);
            REQUIRE(A.cofactor(1, 0) == -25);
        }
    }
}


SCENARIO("Matrix: Calculating the determinant of a 3x3 matrix") {
    GIVEN("The following 3x3 matrix A") {
        Matrix<3, 3> A({
            {1, 2, 6},
            {-5, 8, -4},
            {2, 6, 4}});
        THEN("It must hold") {
            REQUIRE(A.cofactor(0, 0) == 56);
            REQUIRE(A.cofactor(0, 1) == 12);
            REQUIRE(A.cofactor(0, 2) == -46);
            REQUIRE(A.determinant() == -196);
        }
    }
}

SCENARIO("Matrix: Calculating the determinant of a 4x4 matrix") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {-2, -8, 3, 5},
            {-3, 1, 7, 3},
            {1, 2, -9, 6},
            {-6, 7, 7, -9}});
        THEN("It must hold") {
            REQUIRE(A.cofactor(0, 0) == 690);
            REQUIRE(A.cofactor(0, 1) == 447);
            REQUIRE(A.cofactor(0, 2) == 210);
            REQUIRE(A.cofactor(0, 3) == 51);
            REQUIRE(A.determinant() == -4071);
        }
    }
}

SCENARIO("Matrix: Testing an invertible matrix for invertibility") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {6, 4, 4, 4},
            {5, 5, 7, 6},
            {4, -9, 3, -7},
            {9, 1, 7, -6}});
        THEN("If the determinant is non-zero the matrix is invertible") {
            REQUIRE(A.determinant() == -2120);
            REQUIRE(A.invertible() == true);
        }
    }
}

SCENARIO("Matrix: Testing an noninvertible matrix for invertibility") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {-4, 2, -2, -3},
            {9, 6, 2, 6},
            {0, -5, 1, -5},
            {0, 0, 0, 0}});
        THEN("If the determinant is zero the matrix is not invertible") {
            REQUIRE(A.determinant() == 0);
            REQUIRE(A.invertible() == false);
        }
    }
}


SCENARIO("Matrix: Calculating the inverse of a matrix") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {-5, 2, 6, -8},
            {1, -5, 1, 8},
            {7, 7, -6, -7},
            {1, -3, 7, 4}});
        Matrix<4, 4> expected({
            { 0.21805, 0.45113, 0.24060, -0.04511},
            {-0.80827, -1.45677, -0.44361, 0.52068},
            {-0.07895, -0.22368, -0.05263, 0.19737},
            {-0.52256, -0.81391, -0.30075, 0.30639}});
        AND_WHEN("A is invertible") {
            REQUIRE(A.invertible() == true);
            Matrix B = A.inverse();
            THEN("It must hold") {
                REQUIRE(A.determinant() == 532);
                REQUIRE(A.cofactor(2, 3) == -160);
                REQUIRE(B[3][2] == Approx(-160.0 / 532.0).epsilon(EPSILON));
                REQUIRE(A.cofactor(3, 2) == 105);
                REQUIRE(B[2][3] == Approx(105.0 / 532.0).epsilon(EPSILON));
                REQUIRE(B == expected);
            }
        }
    }
}

SCENARIO("Matrix: Calculating the inverse of another matrix") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {8, -5, 9, 2},
            {7, 5, 6, 1},
            {-6, 0, 9, 6},
            {-3, 0, -9, -4}});
        Matrix<4, 4> expected({
            {-0.15385, -0.15385, -0.28205, -0.53846},
            {-0.07692, 0.12308, 0.02564, 0.03077},
            {0.35897, 0.35897, 0.43590, 0.92308},
            {-0.69231, -0.69231, -0.76923, -1.92308}});
        THEN("It must hold") {
            REQUIRE(A.inverse() == expected);
        }
    }
}

SCENARIO("Matrix: Calculating the inverse of a third matrix") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {9, 3, 0, 9},
            {-5, -2, -6, -3},
            {-4, 9, 6, 4},
            {-7, 6, 6, 2}});
        Matrix<4, 4> expected({
            {-0.04074, -0.07778, 0.14444, -0.22222},
            {-0.07778, 0.03333, 0.36667, -0.33333},
            {-0.02901, -0.14630, -0.10926, 0.12963},
            {0.17778, 0.06667, -0.26667, 0.33333}});
        THEN("It must hold") {
            REQUIRE(A.inverse() == expected);
        }
    }
}

SCENARIO("Matrix: Multiplying a product by its inverse") {
    GIVEN("The following 4x4 matrices A and B") {
        Matrix<4, 4> A({
            {3, -9, 7, 3},
            {3, -8, 2, -9},
            {-4, 4, 4, 1},
            {-6, 5, -1, 1}});
        Matrix<4, 4> B({
            {8, 2, 2, 2},
            {3, -1, 7, 0},
            {7, 0, 5, 4},
            {6, -2, 0, 5}});
        Matrix C = A * B;
        THEN("If you multiply some matrix A by another matrix B, producing C, you can multiply C by the inverse of B to get A again") {
            REQUIRE(C * B.inverse() == A);
        }
    }
}

SCENARIO("Matrix: Inverting the identity matrix is itself") {
    GIVEN("The 4x4 identity matrix A") {
        Matrix A = Matrix<4, 4>::identity();
        THEN("A inverted is equal to A") {
            REQUIRE(A.inverse() == A);
        }
    }
}

SCENARIO("Matrix: Multiplying a matrix with its inverse is the identity matrix") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {3, -9, 7, 3},
            {3, -8, 2, -9},
            {-4, 4, 4, 1},
            {-6, 5, -1, 1}});
        THEN("A multiplied by its inverse is the 4x4 identity matrix") {
            REQUIRE(A.inverse() * A == Matrix<4, 4>::identity());
        }
    }
}

SCENARIO("Matrix: The inverse of the tranpose is equal to the tranpose of the inverse") {
    GIVEN("The following 4x4 matrix A") {
        Matrix<4, 4> A({
            {3, -9, 7, 3},
            {3, -8, 2, -9},
            {-4, 4, 4, 1},
            {-6, 5, -1, 1}});
        THEN("It holds") {
            REQUIRE(A.inverse().transposed() == A.transposed().inverse());
        }
    }
}

SCENARIO("Matrix: A near identity matrix multiplied by a tuple") {
    GIVEN("The following matrix A and tuple b") {
        Matrix<4, 4> A({
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 4, 0},
            {0, 0, 0, 1}});
        Tuple b = Tuple(1, 2, 3, 4);
        THEN("A * b equals") {
            REQUIRE(A * b == Tuple(1, 2, 12, 4));
        }
    }
}
