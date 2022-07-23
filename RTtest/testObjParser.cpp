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
#include "ObjParser.hpp"

SCENARIO("Ignoring unrecognized lines") {
    GIVEN("A file (here represented as a string") {
        std::string s = "There was a young lady named Bright\nwho traveled much faster than light.\nShe set out one day\nin a relative way,\nand came back the previous night.\n";
        ObjParser parser = ObjParser();
        parser.parse_from_string(s);
        THEN("It holds") {
        }
    }
}

SCENARIO("Vertex records") {
    GIVEN("A file (here represented as a string") {
        std::string s = "v -1 1 0\nv -1.0000 0.5000 0.0000\nv 1 0 0\nv 1 1 0\n";
        ObjParser parser = ObjParser();
        parser.parse_from_string(s);
        THEN("It holds") {
            REQUIRE(parser.vertex(1) == Point(-1, 1, 0));
            REQUIRE(parser.vertex(2) == Point(-1, 0.5, 0));
            REQUIRE(parser.vertex(3) == Point(1, 0, 0));
            REQUIRE(parser.vertex(4) == Point(1, 1, 0));
        }
    }
}

SCENARIO("Parsing triangle faces") {
    GIVEN("A file (here represented as a string") {
        std::string s = "v -1 1 0\nv -1 0 0\nv 1 0 0\nv 1 1 0\n\nf 1 2 3\nf 1 3 4\n";
        WHEN("Parsing the string") {
            ObjParser parser = ObjParser();
            parser.parse_from_string(s);
            std::shared_ptr<Group> g = parser.default_group();
            std::shared_ptr<Triangle> t1 = std::dynamic_pointer_cast<Triangle>(g->operator[](0));
            std::shared_ptr<Triangle> t2 = std::dynamic_pointer_cast<Triangle>(g->operator[](1));
            THEN("It holds") {
                REQUIRE(t1->p1() == parser.vertex(1));
                REQUIRE(t1->p2() == parser.vertex(2));
                REQUIRE(t1->p3() == parser.vertex(3));
                REQUIRE(t2->p1() == parser.vertex(1));
                REQUIRE(t2->p2() == parser.vertex(3));
                REQUIRE(t2->p3() == parser.vertex(4));
            }
        }
    }
}

SCENARIO("Triangulating polygons") {
    GIVEN("A file (here represented as a string") {
        std::string s = "v -1 1 0\nv -1 0 0\nv 1 0 0\nv 1 1 0\nv 0 2 0\n\nf 1 2 3 4 5\n";
        WHEN("Parsing the string") {
            ObjParser parser = ObjParser();
            parser.parse_from_string(s);
            std::shared_ptr<Group> g = parser.default_group();
            std::shared_ptr<Triangle> t1 = std::dynamic_pointer_cast<Triangle>(g->operator[](0));
            std::shared_ptr<Triangle> t2 = std::dynamic_pointer_cast<Triangle>(g->operator[](1));
            std::shared_ptr<Triangle> t3 = std::dynamic_pointer_cast<Triangle>(g->operator[](2));
            THEN("It holds") {
                REQUIRE(t1->p1() == parser.vertex(1));
                REQUIRE(t1->p2() == parser.vertex(2));
                REQUIRE(t1->p3() == parser.vertex(3));
                REQUIRE(t2->p1() == parser.vertex(1));
                REQUIRE(t2->p2() == parser.vertex(3));
                REQUIRE(t2->p3() == parser.vertex(4));
                REQUIRE(t3->p1() == parser.vertex(1));
                REQUIRE(t3->p2() == parser.vertex(4));
                REQUIRE(t3->p3() == parser.vertex(5));
            }
        }
    }
}

SCENARIO("Triangles in groups") {
    GIVEN("A file (here represented as a string") {
        std::string s = "v -1 1 0\nv -1 0 0\nv 1 0 0\nv 1 1 0\n\ng FirstGroup\nf 1 2 3\ng SecondGroup\nf 1 3 4\n";
        WHEN("Parsing the string") {
            ObjParser parser = ObjParser();
            parser.parse_from_string(s);
            std::shared_ptr<Group> g1 = parser.get_group("FirstGroup");
            std::shared_ptr<Group> g2 = parser.get_group("SecondGroup");
            std::shared_ptr<Triangle> t1 = std::dynamic_pointer_cast<Triangle>(g1->operator[](0));
            std::shared_ptr<Triangle> t2 = std::dynamic_pointer_cast<Triangle>(g2->operator[](0));
            THEN("It holds") {
                REQUIRE(t1->p1() == parser.vertex(1));
                REQUIRE(t1->p2() == parser.vertex(2));
                REQUIRE(t1->p3() == parser.vertex(3));
                REQUIRE(t2->p1() == parser.vertex(1));
                REQUIRE(t2->p2() == parser.vertex(3));
                REQUIRE(t2->p3() == parser.vertex(4));
            }
        }
    }
}

SCENARIO("Converting an OBJ file to a group") {
    GIVEN("A file (here represented as a string") {
        std::string s = "v -1 1 0\nv -1 0 0\nv 1 0 0\nv 1 1 0\n\ng FirstGroup\nf 1 2 3\ng SecondGroup\nf 1 3 4\n";
        WHEN("Parsing the string") {
            ObjParser parser = ObjParser();
            parser.parse_from_string(s);
            std::shared_ptr<Group> g = parser.obj_to_group();
            THEN("It holds") {
                REQUIRE(g->count() == 2);
            }
        }
    }
}

SCENARIO("Vertex normal records") {
    GIVEN("A file (here represented as a string") {
        std::string s = "vn 0 0 1\nvn 0.707 0 -0.707\nvn 1 2 3\n";
        WHEN("Parsing the string") {
            ObjParser parser = ObjParser();
            parser.parse_from_string(s);
            THEN("It holds") {
                REQUIRE(parser.normal(1) == Vector(0, 0, 1));
                REQUIRE(parser.normal(2) == Vector(0.707, 0, -0.707));
                REQUIRE(parser.normal(3) == Vector(1, 2, 3));
            }
        }
    }
}

SCENARIO("Faces with normals") {
    GIVEN("A file (here represented as a string") {
        std::string s = "v 0 1 0\nv -1 0 0\nv 1 0 0\nvn -1 0 0\nvn 1 0 0\nvn 0 1 0\nf 1//3 2//1 3//2\nf 1/0/3 2/102/1 3/14/2";
        WHEN("Parsing the string") {
            ObjParser parser = ObjParser();
            parser.parse_from_string(s);
            std::shared_ptr<Group> g = parser.default_group();
            std::shared_ptr<SmoothTriangle> t1 = std::dynamic_pointer_cast<SmoothTriangle>(g->operator[](0));
            std::shared_ptr<SmoothTriangle> t2 = std::dynamic_pointer_cast<SmoothTriangle>(g->operator[](1));
            THEN("It holds") {
                REQUIRE(t1->p1() == parser.vertex(1));
                REQUIRE(t1->p2() == parser.vertex(2));
                REQUIRE(t1->p3() == parser.vertex(3));
                REQUIRE(t1->n1() == parser.normal(3));
                REQUIRE(t1->n2() == parser.normal(1));
                REQUIRE(t1->n3() == parser.normal(2));
            }
        }
    }
}
