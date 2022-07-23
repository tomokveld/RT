#include "catch.hpp"
#include "Helper.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"
#include "Material.hpp"
#include "testHelper.hpp"

SCENARIO("Sphere: The normal on a sphere at a point on the x axis") {
    GIVEN("A sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Normal_at given (1, 0, 0)") {
            const Tuple n = s->normal_at(Point(1, 0, 0), Intersection(1, s));
            THEN("It should hold") {
                REQUIRE(n == Vector(1, 0, 0));
            }
        }
    }
}

SCENARIO("Sphere: The normal on a sphere at a point on the y axis") {
    GIVEN("A sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Normal_at given (0, 1, 0)") {
            const Tuple n = s->normal_at(Point(0, 1, 0), Intersection(1, s));
            THEN("It should hold") {
                REQUIRE(n == Vector(0, 1, 0));
            }
        }
    }
}

SCENARIO("Sphere: The normal on a sphere at a point on the z axis") {
    GIVEN("A sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Normal_at given (0, 0, 1)") {
            const Tuple n = s->normal_at(Point(0, 0, 1), Intersection(1, s));
            THEN("It should hold") {
                REQUIRE(n == Vector(0, 0, 1));
            }
        }
    }
}

SCENARIO("Sphere: The normal on a sphere at a nonaxial point") {
    GIVEN("A sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Normal_at given (sqrt(3)/3, sqrt(3)/3, sqrt(3)/3)") {
            const Tuple n = s->normal_at(Point(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3), Intersection(1, s));
            THEN("It should hold") {
                REQUIRE(n == Vector(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3));
            }
        }
    }
}

SCENARIO("Sphere: The normal is a normalized vector") {
    GIVEN("A sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Normal_at given (sqrt(3)/3, sqrt(3)/3, sqrt(3)/3)") {
            const Tuple n = s->normal_at(Point(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3), Intersection(1, s));
            THEN("It should hold") {
                REQUIRE(n == n.normalize());
            }
        }
    }
}

SCENARIO("Sphere: Computing the normal on a translated sphere") {
    GIVEN("A sphere with a translate") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::translation(0, 1, 0));
        WHEN("Normal_at given (0, 1.70711, -0.70711)") {
            const Tuple n = s->normal_at(Point(0, 1.70711, -0.70711), Intersection(1, s));
            THEN("It should hold") {
                REQUIRE(n == Vector(0, 0.70711, -0.70711));
            }
        }
    }
}

SCENARIO("Sphere: Computing the normal on a transformed sphere") {
    GIVEN("A sphere with a transform applied") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        Matrix<4, 4> m = Transform::scaling(1, 0.5, 1) * Transform::rotation_z(M_PI / 5);
        s->set_transform(m);
        WHEN("Normal_at given (0, sqrt(2)/2, -sqrt(2)/2)") {
            const Tuple n = s->normal_at(Point(0, sqrt(2)/2, -sqrt(2)/2), Intersection(1, s));
            THEN("It should hold") {
                REQUIRE(n == Vector(0, 0.97014, -0.24254));
            }
        }
    }
}

SCENARIO("Sphere: A sphere has a default material") {
    GIVEN("A sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("The material of the sphere") {
            Material m = s->get_material();
            THEN("It holds") {
                REQUIRE(m == Material());
            }
        }
    }
}

SCENARIO("Sphere: A sphere may be assigned a material") {
    GIVEN("A sphere and a material") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        Material m = Material();
        m.set_ambient(1);
        WHEN("Changing the material of the sphere") {
            s->set_material(m);
            THEN("It holds") {
                REQUIRE(s->get_material() == m);
            }
        }
    }
}

SCENARIO("Sphere: A sphere is a Shape") {
    GIVEN("A sphere") {
         std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        THEN("It should hold") {
            assert(std::dynamic_pointer_cast<Shape>(s));
            REQUIRE(std::is_base_of<Shape, Sphere>::value);
        }
    }
}

SCENARIO("Sphere: A helper for producing a sphere with a glassy material") {
    GIVEN("A glass sphere") {
        std::shared_ptr<Shape> s = glass_sphere();
        THEN("It should hold") {
            REQUIRE(s->get_transform() == Matrix<4, 4>::identity());
            REQUIRE(s->get_material().get_transparency() == 1.0);
            REQUIRE(s->get_material().get_refractive_index() == 1.5);
        }
    }
}

SCENARIO("Sphere: A sphere has a bounding box") {
    GIVEN("A sphere") {
        std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Querying the bounding box") {
            const Bounds box = s->bounds;
            THEN("It should hold") {
                REQUIRE(box.min() == Point(-1, -1, -1));
                REQUIRE(box.max() == Point(1, 1, 1));
            }
        }
    }
}
