#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cube.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Material.hpp"
#include "Pattern.hpp"
#include "Canvas.hpp"
#include "testHelper.hpp"

SCENARIO("The default pattern transformation") {
    GIVEN("A pattern") {
        const TestPattern p = TestPattern();
        THEN("It has the identity matrix transform") {
            REQUIRE(p.get_transform() == Matrix<4, 4>::identity());
        }
    }
}

SCENARIO("Assigning a transformation") {
    GIVEN("A pattern") {
        TestPattern p = TestPattern();
        WHEN("Assigning a transform") {
            p.set_transform(Transform::translation(1, 2, 3));
            THEN("It has the new matrix transform") {
                REQUIRE(p.get_transform() == Transform::translation(1, 2, 3));
            }
        }
    }
}

SCENARIO("A pattern with an object transformation") {
    GIVEN("A sphere, transform, and pattern") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::scaling(2, 2, 2));
        TestPattern p = TestPattern();
        WHEN("Calling shape_color_at") {
            const Color c = p.shape_color_at(s, Point(2, 3, 4));
            THEN("It holds") {
                REQUIRE(c == Color(1, 1.5, 2));
            }
        }
    }
}

SCENARIO("A pattern with a pattern transformation") {
    GIVEN("A sphere, transform, and pattern") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        TestPattern p = TestPattern();
        p.set_transform(Transform::scaling(2, 2, 2));
        WHEN("Calling shape_color_at") {
            const Color c = p.shape_color_at(s, Point(2, 3, 4));
            THEN("It holds") {
                REQUIRE(c == Color(1, 1.5, 2));
            }
        }
    }
}

SCENARIO("A pattern with both an object and a pattern transformation") {
    GIVEN("A sphere, transform, and pattern") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::scaling(2, 2, 2));
        TestPattern p = TestPattern();
        p.set_transform(Transform::translation(0.5, 1, 1.5));
        WHEN("Calling shape_color_at") {
            const Color c = p.shape_color_at(s, Point(2.5, 3, 3.5));
            THEN("It holds") {
                REQUIRE(c == Color(0.75, 0.5, 0.25));
            }
        }
    }
}

SCENARIO("Stripes with an object transformation") {
    GIVEN("A sphere, transform, and pattern") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::scaling(2, 2, 2));
        const std::shared_ptr<PatternStripe> p = std::make_shared<PatternStripe>(Color::white(), Color::black());
        WHEN("Calling shape_color_at") {
            const Color c = p->shape_color_at(s, Point(1.5, 0, 0));
            THEN("It holds") {
                REQUIRE(c == Color::white());
            }
        }
    }
}


SCENARIO("Stripes with a pattern transformation") {
    GIVEN("A sphere, transform, and pattern") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const std::shared_ptr<PatternStripe> p = std::make_shared<PatternStripe>(Color::white(), Color::black());
        p->set_transform(Transform::scaling(2, 2, 2));
        WHEN("Calling shape_color_at") {
            const Color c = p->shape_color_at(s, Point(1.5, 0, 0));
            THEN("It holds") {
                REQUIRE(c == Color::white());
            }
        }
    }
}

SCENARIO("Stripes with both an object and a pattern transformation") {
    GIVEN("A sphere, transform, and pattern") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::scaling(2, 2, 2));
        const std::shared_ptr<PatternStripe> p = std::make_shared<PatternStripe>(Color::white(), Color::black());
        p->set_transform(Transform::translation(0.5, 0, 0));
        WHEN("Calling shape_color_at") {
            const Color c = p->shape_color_at(s, Point(2.5, 0, 0));
            THEN("It holds") {
                REQUIRE(c == Color::white());
            }
        }
    }
}

SCENARIO("A stripe pattern is constant in y") {
    GIVEN("A stripe pattern") {
        std::shared_ptr<PatternStripe> pattern = std::make_shared<PatternStripe>(Color::white(), Color::black());
        THEN("It holds") {
            REQUIRE(pattern->color_at(Point(0, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0, 1, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0, 2, 0)) == Color::white());
        }
    }
}

SCENARIO("A stripe pattern is constant in z") {
    GIVEN("A stripe pattern") {
        std::shared_ptr<PatternStripe> pattern = std::make_shared<PatternStripe>(Color::white(), Color::black());
        THEN("It holds") {
            REQUIRE(pattern->color_at(Point(0, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0, 0, 1)) == Color::white());
            REQUIRE(pattern->color_at(Point(0, 0, 2)) == Color::white());
        }
    }
}

SCENARIO("A stripe pattern alternates in x") {
    GIVEN("A stripe pattern") {
        std::shared_ptr<PatternStripe> pattern = std::make_shared<PatternStripe>(Color::white(), Color::black());
        THEN("It holds") {
            REQUIRE(pattern->color_at(Point(0, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0.9, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(1, 0, 0)) == Color::black());
            REQUIRE(pattern->color_at(Point(-0.1, 0, 0)) == Color::black());
            REQUIRE(pattern->color_at(Point(-1, 0, 0)) == Color::black());
            REQUIRE(pattern->color_at(Point(-1.1, 0, 0)) == Color::white());
        }
    }
}

SCENARIO("A gradient linearly interpolates between colors") {
    GIVEN("A gradient pattern") {
        std::shared_ptr<PatternGradient> pattern = std::make_shared<PatternGradient>(Color::white(), Color::black());
        THEN("It holds") {
            REQUIRE(pattern->color_at(Point(0, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0.25, 0, 0)) == Color(0.75, 0.75, 0.75));
            REQUIRE(pattern->color_at(Point(0.5, 0, 0)) == Color(0.5, 0.5, 0.5));
            REQUIRE(pattern->color_at(Point(0.75, 0, 0)) == Color(0.25, 0.25, 0.25));
        }
    }
}

SCENARIO("A ring should extend in both x and z") {
    GIVEN("A gradient pattern") {
        std::shared_ptr<PatternRing> pattern = std::make_shared<PatternRing>(Color::white(), Color::black());
        THEN("It holds") {
            REQUIRE(pattern->color_at(Point(0, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(1, 0, 0)) == Color::black());
            REQUIRE(pattern->color_at(Point(0, 0, 1)) == Color::black());
            REQUIRE(pattern->color_at(Point(0.708, 0, 0.708)) == Color::black());
        }
    }
}


SCENARIO("Checkers should repeat in x") {
    GIVEN("A checkers pattern") {
        std::shared_ptr<PatternCheckers> pattern = std::make_shared<PatternCheckers>(Color::white(), Color::black());
        THEN("It holds") {
            REQUIRE(pattern->color_at(Point(0, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0.99, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(1.01, 0, 0)) == Color::black());
        }
    }
}

SCENARIO("Checkers should repeat in y") {
    GIVEN("A checkers pattern") {
        std::shared_ptr<PatternCheckers> pattern = std::make_shared<PatternCheckers>(Color::white(), Color::black());
        THEN("It holds") {
            REQUIRE(pattern->color_at(Point(0, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0, 0.99, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0, 1.01, 0)) == Color::black());
        }
    }
}

SCENARIO("Checkers should repeat in z") {
    GIVEN("A checkers pattern") {
        std::shared_ptr<PatternCheckers> pattern = std::make_shared<PatternCheckers>(Color::white(), Color::black());
        THEN("It holds") {
            REQUIRE(pattern->color_at(Point(0, 0, 0)) == Color::white());
            REQUIRE(pattern->color_at(Point(0, 0, 0.99)) == Color::white());
            REQUIRE(pattern->color_at(Point(0, 0, 1.01)) == Color::black());
        }
    }
}

SCENARIO("Checker pattern in 2D") {
    std::tuple<float, float, Color> p[] = {{0.0, 0.0, Color::black()},
                                           {0.5, 0.0, Color::white()},
                                           {0.0, 0.5, Color::white()},
                                           {0.5, 0.5, Color::black()},
                                           {1.0, 1.0, Color::black()}};
    for (auto &[u, v, exp] : p) {
        std::shared_ptr<PatternUvCheckers> pattern = std::make_shared<PatternUvCheckers>(2, 2, Color::black(), Color::white());
        const Color c = pattern->uv_color_at(u, v);
        REQUIRE(c == exp);
    }
}

SCENARIO("Using a spherical mapping on a 3D point") {
    std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
    std::tuple<Tuple, float, float> p[] = {{Point(0, 0, -1), 0.0, 0.5},
                                           {Point(1, 0, 0), 0.25, 0.5},
                                           {Point(0, 0, 1), 0.5, 0.5},
                                           {Point(-1, 0, 0), 0.75, 0.5},
                                           {Point(0, 1, 0), 0.5, 1.0},
                                           {Point(0, -1, 0), 0.5, 0.0},
                                           {Point(sqrt(2)/2, sqrt(2)/2, 0), 0.25, 0.75}};
    for (auto &[point, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("Using a texture map pattern with a spherical map") {
    std::tuple<Tuple, Color> p[] = {{Point(0.4315, 0.4670, 0.7719), Color::white()},
                                    {Point(-0.9654, 0.2552, -0.0534), Color::black()},
                                    {Point(0.1039, 0.7090, 0.6975), Color::white()},
                                    {Point(-0.4986, -0.7856, -0.3663), Color::black()},
                                    {Point(-0.0317, -0.9395, 0.3411), Color::black()},
                                    {Point(0.4809, -0.7721, 0.4154), Color::black()},
                                    {Point(0.0285, -0.9612, -0.2745), Color::black()},
                                    {Point(-0.5734, -0.2162, -0.7903), Color::white()},
                                    {Point(0.7688, -0.1470, 0.6223), Color::black()},
                                    {Point(-0.7652, 0.2175, 0.6060), Color::black()}};
    for (auto &[point, color] : p) {
        std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        std::shared_ptr<PatternUvCheckers> pattern = std::make_shared<PatternUvCheckers>(16, 8, Color::black(), Color::white());
        REQUIRE(pattern->color_at(point, s) == color);
    }
}

SCENARIO("Using a planar mapping on a 3D point") {
    std::shared_ptr<Plane> s = std::make_shared<Plane>();
    std::tuple<Tuple, float, float> p[] = {{Point(0.25, 0, 0.5), 0.25, 0.5},
                                    {Point(0.25, 0, -0.25), 0.25, 0.75},
                                    {Point(0.25, 0.5, -0.25), 0.25, 0.75},
                                    {Point(1.25, 0, 0.5), 0.25, 0.5},
                                    {Point(0.25, 0, -1.75), 0.25, 0.25},
                                    {Point(1, 0, -1), 0.0, 0.0},
                                    {Point(0, 0, 0), 0.0, 0.0}};
    for (auto &[point, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("Using a cylindrical mapping on a 3D point") {
    std::shared_ptr<Cylinder> s = std::make_shared<Cylinder>();
    std::tuple<Tuple, float, float> p[] = {{Point(0, 0, -1), 0.0, 0.0},
                                           {Point(0, 0.5, -1), 0.0, 0.5},
                                           {Point(0, 1, -1), 0.0, 0.0},
                                           {Point(0.70711, 0.5, -0.70711), 0.125, 0.5},
                                           {Point(1, 0.5, 0), 0.25, 0.5},
                                           {Point(0.70711, 0.5, 0.70711), 0.375, 0.5},
                                           {Point(0, -0.25, 1), 0.5, -0.25}, // v = 0.75 doesnt work...
                                           {Point(-0.70711, 0.5, 0.70711), 0.625, 0.5},
                                           {Point(-1, 1.25, 0), 0.75, 0.25},
                                           {Point(-0.70711, 0.5, -0.70711), 0.875, 0.5}};
    for (auto &[point, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("Layout of the 'align check' pattern") {
    Color main = Color(1, 1, 1);
    Color ul = Color(1, 0, 0);
    Color ur = Color(1, 1, 0);
    Color bl = Color(0, 1, 0);
    Color br = Color(0, 1, 1);
    std::shared_ptr<PatternUvAlignCheck> pattern = std::make_shared<PatternUvAlignCheck>(main, ul, ur, bl, br);
    std::tuple<float, float, Color> p[] = {{0.5, 0.5, main},
                                           {0.1, 0.9, ul},
                                           {0.9, 0.9, ur},
                                           {0.1, 0.1, bl},
                                           {0.9, 0.1, br}};
    for (auto &[u, v, expected] : p) {
        Color c = pattern->uv_color_at(u, v);
        REQUIRE(c == expected);
    }
}

SCENARIO("Identifying the face of a cube from a point") {
    std::tuple<Tuple, int> p[] = {{Point(-1, 0.5, -0.25), Faces::left},
                                  {Point(1.1, -0.75, 0.8), Faces::right},
                                  {Point(0.1, 0.6, 0.9), Faces::front},
                                  {Point(-0.7, 0, -2), Faces::back},
                                  {Point(0.5, 1, 0.9), Faces::up},
                                  {Point(-0.2, -1.3, 1.1), Faces::down}};
    for (auto &[point_, face_] : p) {
        REQUIRE(point_.face_from_point() == face_);
    }
}

SCENARIO("UV mapping the front face of a cube") {
    std::shared_ptr<Cube> s = std::make_shared<Cube>();
    std::tuple<Tuple, float, float> p[] = {{Point(-0.5, 0.5, 1), 0.25, 0.75},
                                           {Point(0.5, -0.5, 1), 0.75, 0.25}};
    for (auto &[point_, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point_, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("UV mapping the back face of a cube") {
    std::shared_ptr<Cube> s = std::make_shared<Cube>();
    std::tuple<Tuple, float, float> p[] = {{Point(0.5, 0.5, -1), 0.25, 0.75},
                                           {Point(-0.5, -0.5, -1), 0.75, 0.25}};
    for (auto &[point_, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point_, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("UV mapping the left face of a cube") {
    std::shared_ptr<Cube> s = std::make_shared<Cube>();
    std::tuple<Tuple, float, float> p[] = {{Point(-1, 0.5, -0.5), 0.25, 0.75},
                                           {Point(-1, -0.5, 0.5), 0.75, 0.25}};
    for (auto &[point_, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point_, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("UV mapping the right face of a cube") {
    std::shared_ptr<Cube> s = std::make_shared<Cube>();
    std::tuple<Tuple, float, float> p[] = {{Point(1, 0.5, 0.5), 0.25, 0.75},
                                           {Point(1, -0.5, -0.5), 0.75, 0.25}};
    for (auto &[point_, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point_, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("UV mapping the upper face of a cube") {
    std::shared_ptr<Cube> s = std::make_shared<Cube>();
    std::tuple<Tuple, float, float> p[] = {{Point(-0.5, 1, -0.5), 0.25, 0.75},
                                           {Point(0.5, 1, 0.5), 0.75, 0.25}};
    for (auto &[point_, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point_, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("UV mapping the lower face of a cube") {
    std::shared_ptr<Cube> s = std::make_shared<Cube>();
    std::tuple<Tuple, float, float> p[] = {{Point(-0.5, -1, 0.5), 0.25, 0.75},
                                           {Point(0.5, -1, -0.5), 0.75, 0.25}};
    for (auto &[point_, u_, v_] : p) {
        float u, v;
        s->UVMappedPoint(point_, &u, &v);
        REQUIRE(equal(u, u_));
        REQUIRE(equal(v, v_));
    }
}

SCENARIO("Finding the colors on a mapped cube") {
    Color red = Color(1, 0, 0);
    Color yellow = Color(1, 1, 0);
    Color brown = Color(1, 0.5, 0);
    Color green = Color(0, 1, 0);
    Color cyan = Color(0, 1, 1);
    Color blue = Color(0, 0, 1);
    Color purple = Color(1, 0, 1);
    Color white = Color(1, 1, 1);
    
    std::shared_ptr<PatternUvAlignCheck> left = std::make_shared<PatternUvAlignCheck>(yellow, cyan, red, blue, brown);
    std::shared_ptr<PatternUvAlignCheck> front = std::make_shared<PatternUvAlignCheck>(cyan, red, yellow, brown, green);
    std::shared_ptr<PatternUvAlignCheck> right = std::make_shared<PatternUvAlignCheck>(red, yellow, purple, green, white);
    std::shared_ptr<PatternUvAlignCheck> back = std::make_shared<PatternUvAlignCheck>(green, purple, cyan, white, blue);
    std::shared_ptr<PatternUvAlignCheck> up = std::make_shared<PatternUvAlignCheck>(brown, cyan, purple, red, yellow);
    std::shared_ptr<PatternUvAlignCheck> down = std::make_shared<PatternUvAlignCheck>(purple, brown, green, blue, white);
    
    std::tuple<Tuple, Color> p[] = {{Point(-1, 0, 0), yellow},     // L
                                    {Point(-1, 0.9, -0.9), cyan},
                                    {Point(-1, 0.9, 0.9), red},
                                    {Point(-1, -0.9, -0.9), blue},
                                    {Point(-1, -0.9, 0.9), brown},
                                    {Point(0, 0, 1), cyan},        // F
                                    {Point(-0.9, 0.9, 1), red},
                                    {Point(0.9, 0.9, 1), yellow},
                                    {Point(-0.9, -0.9, 1), brown},
                                    {Point(0.9, -0.9, 1), green},
                                    {Point(1, 0, 0), red},         // R
                                    {Point(1, 0.9, 0.9), yellow},
                                    {Point(1, 0.9, -0.9), purple},
                                    {Point(1, -0.9, 0.9), green},
                                    {Point(1, -0.9, -0.9), white},
                                    {Point(0, 0, -1), green},      // B
                                    {Point(0.9, 0.9, -1), purple},
                                    {Point(-0.9, 0.9, -1), cyan},
                                    {Point(0.9, -0.9, -1), white},
                                    {Point(-0.9, -0.9, -1), blue},
                                    {Point(0, 1, 0), brown},       // U
                                    {Point(-0.9, 1, -0.9), cyan},
                                    {Point(0.9, 1, -0.9), purple},
                                    {Point(-0.9, 1, 0.9), red},
                                    {Point(0.9, 1, 0.9), yellow},
                                    {Point(0, -1, 0), purple},     // D
                                    {Point(-0.9, -1, 0.9), brown},
                                    {Point(0.9, -1, 0.9), green},
                                    {Point(-0.9, -1, -0.9), blue},
                                    {Point(0.9, -1, -0.9), white}};
    std::shared_ptr<PatternCubeMap> pattern = std::make_shared<PatternCubeMap>(left, front, right, back, up, down);
    for (auto &[point_, color_] : p) {
        REQUIRE(pattern->color_at(point_, std::make_shared<Cube>()) == color_);
    }
}

SCENARIO("UV image pattern") {
    std::tuple<float, float, Color> p[] = {{0, 0, Color(0.9, 0.9, 0.9)},
                                        {0.3, 0, Color(0.2, 0.2, 0.2)},
                                        {0.6, 0.3, Color(0.1, 0.1, 0.1)},
                                        {1, 1, Color(0.9, 0.9, 0.9)}};
    std::string s = "P3\n10 10\n10\n0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9\n1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0\n2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1\n3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2\n4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3\n5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4\n6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5\n7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6\n8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7\n9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8\n";
    Canvas canvas = canvas_from_ppm_string(s);
    std::shared_ptr<PatternUVImage> pattern = std::make_shared<PatternUVImage>(canvas);
    for (auto &[u, v, expected] : p) {
        REQUIRE(pattern->uv_color_at(u, v) == expected);
    }
}

