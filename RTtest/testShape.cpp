#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Group.hpp"
#include "Transformations.hpp"
#include "testHelper.hpp"

SCENARIO("Shape: A sphere's default transformation") {
    GIVEN("A sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        THEN("The transform of a sphere is the identity matrix") {
            REQUIRE(s->get_transform() == Matrix<4, 4>::identity());
        }
    }
}

SCENARIO("Shape: Changing a sphere's transformation") {
    GIVEN("A sphere and a transform") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        Matrix<4, 4> t = Transform::translation(2, 3, 4);
        WHEN("Changing the transform of a Sphere") {
            s->set_transform(t);
            THEN("It should have changed the transform") {
                REQUIRE(s->get_transform() == t);
            }
        }
    }
}

SCENARIO("Shape: Intersecting a scaled sphere with a ray") {
    GIVEN("A ray and a sphere") {
        Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Changing the transform and obtaining the intersections") {
            s->set_transform(Transform::scaling(2, 2, 2));
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 2);
                REQUIRE(xs[0].get_distance() == 3);
                REQUIRE(xs[1].get_distance() == 7);
            }
        }
    }
}

SCENARIO("Shape: Intersecting a translated sphere with a ray") {
    GIVEN("A ray and a sphere") {
        Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Changing the transform and obtaining the intersections") {
            s->set_transform(Transform::translation(5, 0, 0));
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 0);
            }
        }
    }
}

SCENARIO("Shape: The default transformation") {
    GIVEN("The test shape") {
        const std::shared_ptr<TestShape> s  = std::make_shared<TestShape>();
        THEN("The test shape has as default transform the identity matrix") {
            REQUIRE(s->get_transform() == Matrix<4, 4>::identity());
        }
    }
}

SCENARIO("Shape: Assigning a transformation") {
    GIVEN("The test shape") {
        const std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("Setting the transform") {
            s->set_transform(Transform::translation(2, 3, 4));
            THEN("The test shape has as default transform the identity matrix") {
                REQUIRE(s->get_transform() == Transform::translation(2, 3, 4));
            }
        }
    }
}

SCENARIO("Shape: The default material") {
    GIVEN("The test shape") {
        const std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("Getting the material") {
            const Material m = s->get_material();
            THEN("It should hold") {
                REQUIRE(m == Material());
            }
        }
    }
}

SCENARIO("Shape: Assigning a material") {
    GIVEN("The test shape") {
        const std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        Material m = s->get_material();
        m.set_ambient(1);
        WHEN("Setting the material") {
            s->set_material(m);
            THEN("It should hold") {
                REQUIRE(s->get_material() == m);
            }
        }
    }
}

SCENARIO("Shape: Intersecting a scaled shape with a ray") {
    GIVEN("The test shape") {
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("When setting the transform and insersecting") {
            s->set_transform(Transform::scaling(2, 2, 2));
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It holds") {
                REQUIRE(s->local_ray.get_origin() == Point(0, 0, -2.5));
                REQUIRE(s->local_ray.get_direction() == Vector(0, 0, 0.5));
            }
        }
    }
}

SCENARIO("Shape: Intersecting a translated shape with a ray") {
    GIVEN("The test shape") {
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("When setting the transform and insersecting") {
            s->set_transform(Transform::translation(5, 0, 0));
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It holds") {
                REQUIRE(s->local_ray.get_origin() == Point(-5, 0, -5));
                REQUIRE(s->local_ray.get_direction() == Vector(0, 0, 1));
            }
        }
    }
}

SCENARIO("Shape: Computing the normal on a translated shape") {
    GIVEN("The test shape") {
        std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("When setting the transform and getting the normal vector") {
            s->set_transform(Transform::translation(0, 1, 0));
            const Tuple n = s->normal_at(Point(0, 1.70711, -0.70711), Intersection(1, s));
            THEN("It holds") {
                REQUIRE(n == Vector(0, 0.70711, -0.70711));
            }
        }
    }
}

SCENARIO("Shape: Computing the normal on a transformed shape") {
    GIVEN("The test shape") {
        std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("When setting the transform and getting the normal vector") {
            s->set_transform(Transform::scaling(1, 0.5, 1) *
                             Transform::rotation_z(M_PI / 5));
            const Tuple n = s->normal_at(Point(0, sqrt(2)/2, -sqrt(2)/2), Intersection(1, s));
            THEN("It holds") {
                REQUIRE(n == Vector(0, 0.97014, -0.24254));
            }
        }
    }
}

SCENARIO("A shape has a parent attribute") {
    GIVEN("The test shape") {
        const std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        THEN("It holds") {
            REQUIRE(s->get_parent() == nullptr);
        }
    }
}

SCENARIO("Converting a point from world to object space") {
    GIVEN("Multiple groups and a sphere") {
        const std::shared_ptr<Group> g1 = std::make_shared<Group>();
        g1->set_transform(Transform::rotation_y(M_PI_2));
        const std::shared_ptr<Group> g2 = std::make_shared<Group>();
        g2->set_transform(Transform::scaling(2, 2, 2));
        g1->add_child(g2);
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::translation(5, 0, 0));
        g2->add_child(s);
        WHEN("Calling world_to_object") {
            const Tuple p = s->world_to_object(Point(-2, 0, -10));
            THEN("It holds") {
                REQUIRE(p == Point(0, 0, -1));
            }
        }
    }
}

SCENARIO("Converting a normal from object to world space") {
    GIVEN("Multiple groups and a sphere") {
        const std::shared_ptr<Group> g1 = std::make_shared<Group>();
        g1->set_transform(Transform::rotation_y(M_PI_2));
        const std::shared_ptr<Group> g2 = std::make_shared<Group>();
        g2->set_transform(Transform::scaling(1, 2, 3));
        g1->add_child(g2);
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::translation(5, 0, 0));
        g2->add_child(s);
        WHEN("Calling normal_to_world") {
            const Tuple n = s->normal_to_world(Vector(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3));
            THEN("It holds") {
                REQUIRE(n == Vector(0.285714, 0.428571, -0.857143));
            }
        }
    }
}

SCENARIO("Finding the normal on a child object") {
    GIVEN("Multiple groups and a sphere") {
        const std::shared_ptr<Group> g1 = std::make_shared<Group>();
        g1->set_transform(Transform::rotation_y(M_PI_2));
        const std::shared_ptr<Group> g2 = std::make_shared<Group>();
        g2->set_transform(Transform::scaling(1, 2, 3));
        g1->add_child(g2);
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::translation(5, 0, 0));
        g2->add_child(s);
        WHEN("Calling normal_at") {
            const Tuple n = s->normal_at(Point(1.7321, 1.1547, -5.5774), Intersection(1, s));
            THEN("It holds") {
                REQUIRE(n == Vector(0.285704, 0.428543, -0.857161));
            }
        }
    }
}

SCENARIO("Test shape has (arbitrary) bounds") {
    GIVEN("The test shape") {
        const std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("Obtaining the bounds of s") {
            const Bounds box = s->bounds;
            THEN("It holds") {
                REQUIRE(box.min() == Point(-1, -1, -1));
                REQUIRE(box.max() == Point(1, 1, 1));
            }
        }
    }
}

SCENARIO("Querying a shape's bounding box in its parent's space") {
    GIVEN("A transformed sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::translation(1, -3, 5) * Transform::scaling(0.5, 2, 4));
        WHEN("Obtaining the bounds of s") {
            const Bounds box = s->bounds_transform;
            THEN("It holds") {
                REQUIRE(box.min() == Point(0.5, -5, 1));
                REQUIRE(box.max() == Point(1.5, -1, 9));
            }
        }
    }
}

SCENARIO("A group has a bounding box that contains its children") {
    GIVEN("A number of shapes in the same group") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::translation(2, 5, -3) * Transform::scaling(2, 2, 2));
        const std::shared_ptr<Cylinder> c = std::make_shared<Cylinder>(-2, 2, false);
        c->set_transform(Transform::translation(-4, -1, 4) * Transform::scaling(0.5, 1, 0.5));
        const std::shared_ptr<Group> shape = std::make_shared<Group>();
        shape->add_children(s, c);
        WHEN("Obtaining the bounds of s") {
            const Bounds box = shape->bounds;
            THEN("It holds") {
                REQUIRE(box.min() == Point(-4.5, -3, -5));
                REQUIRE(box.max() == Point(4, 7, 4.5));
            }
        }
    }
}

SCENARIO("Subdividing a primitive does nothing") {
    GIVEN("The test shape") {
        std::shared_ptr<TestShape> s = std::make_shared<TestShape>();
        WHEN("Dividing") {
            s->divide(1);
            THEN("It holds") {
                REQUIRE(std::dynamic_pointer_cast<TestShape>(s));
            }
        }
    }
}
