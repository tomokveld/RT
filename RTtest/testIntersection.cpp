#include "catch.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "testHelper.hpp"
#include "Triangle.hpp"

SCENARIO("Intersection: An intersection encapsulates t and object") {
    GIVEN("A sphere") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Creating an Intersection object") {
            const Intersection i = Intersection(3.5, s);
            THEN("It should hold") {
                REQUIRE(i.get_distance() == 3.5);
                REQUIRE(i.get_shape() == s);
            }
        }
    }
}

SCENARIO("Intersection: Aggregating intersections") {
    GIVEN("A sphere and two intersections") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Intersection i1 = Intersection(1, s);
        const Intersection i2 = Intersection(2, s);
        WHEN("Calling Intersections") {
            const std::vector<Intersection> xs = Intersections(i1, i2);
            THEN("It holds") {
                REQUIRE(xs.size() == 2);
                REQUIRE(xs[0].get_distance() == 1);
                REQUIRE(xs[1].get_distance() == 2);
            }
        }
    }
}

SCENARIO("Intersection: Intersect sets the object on the intersection") {
    GIVEN("A Ray and a sphere") {
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        WHEN("Trying to intersect the ray with the sphere") {
            std::vector<Intersection> xs;
            r.intersect(s, xs);
            THEN("It should hold") {
                REQUIRE(xs.size() == 2);
                REQUIRE(xs[0].get_shape() == s);
                REQUIRE(xs[1].get_shape() == s);
            }
        }
    }
}

SCENARIO("Intersection: The hit, when all intersections have positive t") {
    GIVEN("A sphere and some intersections") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Intersection i1 = Intersection(1, s);
        const Intersection i2 = Intersection(2, s);
        const std::vector<Intersection> xs = Intersections(i2, i1);
        WHEN("Hit") {
            const Intersection i = Hit(xs);
            THEN("It should hold") {
                REQUIRE(i == i1);
            }
        }
    }
}

SCENARIO("Intersection: The hit, when some intersections have negative t") {
    GIVEN("A sphere and some intersections") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Intersection i1 = Intersection(-1, s);
        const Intersection i2 = Intersection(1, s);
        const std::vector<Intersection> xs = Intersections(i2, i1);
        WHEN("Hit") {
            const Intersection i = Hit(xs);
            THEN("It should hold") {
                REQUIRE(i == i2);
            }
        }
    }
}

SCENARIO("Intersection: The hit, when all intersections have negative t") {
    GIVEN("A sphere and some intersections") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Intersection i1 = Intersection(-2, s);
        const Intersection i2 = Intersection(-1, s);
        const std::vector<Intersection> xs = Intersections(i2, i1);
        WHEN("Hit") {
            const Intersection i = Hit(xs);
            THEN("It should hold") {
                REQUIRE(i.get_distance() == 0);
                REQUIRE(i.get_shape() == nullptr);
            }
        }
    }
}

SCENARIO("Intersection: The hit is always the lowest nonnegative intersection") {
    GIVEN("A sphere and some intersections") {
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Intersection i1 = Intersection(5, s);
        const Intersection i2 = Intersection(7, s);
        const Intersection i3 = Intersection(-3, s);
        const Intersection i4 = Intersection(2, s);
        const std::vector<Intersection> xs = Intersections(i1, i2, i3, i4);
        WHEN("Hit") {
            const Intersection i = Hit(xs);
            THEN("It should hold") {
                REQUIRE(i == i4);
            }
        }
    }
}

SCENARIO("Precomputing the state of an intersection") {
    GIVEN("A ray, sphere, and a intersection") {
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Intersection i = Intersection(4, s);
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(i);
            THEN("It holds") {
                REQUIRE(comps.distance == i.get_distance());
                REQUIRE(comps.point == Point(0, 0, -1));
                REQUIRE(comps.eyev == Vector(0, 0, -1));
                REQUIRE(comps.normalv == Vector(0, 0, -1));
            }
        }
    }
}

SCENARIO("The hit, when an intersection occurs on the outside") {
    GIVEN("A ray, sphere, and a intersection") {
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Intersection i = Intersection(4, s);
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(i);
            THEN("It holds") {
                REQUIRE(comps.inside == false);
            }
        }
    }
}

SCENARIO("The hit, when an intersection occurs on the inside") {
    GIVEN("A ray, sphere, and a intersection") {
        const Ray r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
        const std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        const Intersection i = Intersection(1, s);
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(i);
            THEN("It holds") {
                REQUIRE(comps.point == Point(0, 0, 1));
                REQUIRE(comps.eyev == Vector(0, 0, -1));
                REQUIRE(comps.inside == true);
                REQUIRE(comps.normalv == Vector(0, 0, -1));
            }
        }
    }
}

SCENARIO("The hit should offset the point") {
    GIVEN("A ray, sphere, and a intersection") {
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
        s->set_transform(Transform::translation(0, 0, 1));
        const Intersection i = Intersection(5, s);
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(i);
            THEN("It holds") {
                REQUIRE(comps.over_point.get_z() < -EPSILON/2);
                REQUIRE(comps.point.get_z() > comps.over_point.get_z());
            }
        }
    }
}

SCENARIO("Precomputing the reflection vector") {
    GIVEN("A ray, plane, and a intersection") {
        const Ray r = Ray(Point(0, 1, -1), Vector(0, -sqrt(2)/2, sqrt(2)/2));
        const std::shared_ptr<Plane> s = std::make_shared<Plane>();
        const Intersection i = Intersection(sqrt(2), s);
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(i);
            THEN("It holds") {
                REQUIRE(comps.reflectv == Vector(0, sqrt(2)/2, sqrt(2)/2));
            }
        }
    }
}


SCENARIO("Finding n1 and n2 at various intersections") {
    GIVEN("Three glass spheres") {
        std::shared_ptr<Shape> A = glass_sphere();
        A->set_transform(Transform::scaling(2, 2, 2));
        A->mod_material().set_refractive_index(1.5);
        
        std::shared_ptr<Shape> B = glass_sphere();
        B->set_transform(Transform::translation(0, 0, -0.25));
        B->mod_material().set_refractive_index(2.0);
        
        std::shared_ptr<Shape> C = glass_sphere();
        C->set_transform(Transform::translation(0, 0, 0.25));
        C->mod_material().set_refractive_index(2.5);
        
        const Ray r = Ray(Point(0, 0, -4), Vector(0, 0, -1));
        
        std::vector<Intersection> xs = Intersections(Intersection(2, A),
                                                     Intersection(2.75, B),
                                                     Intersection(3.25, C),
                                                     Intersection(4.75, B),
                                                     Intersection(5.25, C),
                                                     Intersection(6, A));
        WHEN("Calculating the comps") {
            // TODO: clean this up
            IntersectionComp comps_0 = r.prepare_computations(xs[0], xs);
            IntersectionComp comps_1 = r.prepare_computations(xs[1], xs);
            IntersectionComp comps_2 = r.prepare_computations(xs[2], xs);
            IntersectionComp comps_3 = r.prepare_computations(xs[3], xs);
            IntersectionComp comps_4 = r.prepare_computations(xs[4], xs);
            IntersectionComp comps_5 = r.prepare_computations(xs[5], xs);
            THEN("It holds") {
                REQUIRE(comps_0.n1 == 1.0);
                REQUIRE(comps_0.n2 == 1.5);
                
                REQUIRE(comps_1.n1 == 1.5);
                REQUIRE(comps_1.n2 == 2.0);
                
                REQUIRE(comps_2.n1 == 2.0);
                REQUIRE(comps_2.n2 == 2.5);
                
                REQUIRE(comps_3.n1 == 2.5);
                REQUIRE(comps_3.n2 == 2.5);
                
                REQUIRE(comps_4.n1 == 2.5);
                REQUIRE(comps_4.n2 == 1.5);
                
                REQUIRE(comps_5.n1 == 1.5);
                REQUIRE(comps_5.n2 == 1.0);
            }
        }
    }
}

SCENARIO("The under point is offset below the surface") {
    GIVEN("A ray, sphere, and a intersection") {
        const Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
        std::shared_ptr<Shape> s = glass_sphere();
        s->set_transform(Transform::translation(0, 0, 1));
        const Intersection i = Intersection(5, s);
        std::vector<Intersection> xs = Intersections(i);
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(i, xs);
            THEN("It holds") {
                REQUIRE(comps.under_point.get_z() > EPSILON/2);
                REQUIRE(comps.point.get_z() < comps.under_point.get_z());
            }
        }
    }
}

SCENARIO("The Schlick approximation under total internal reflection") {
    GIVEN("A ray, sphere, and a intersection") {
        std::shared_ptr<Shape> s = glass_sphere();
        const Ray r = Ray(Point(0, 0, sqrtf(2)/2), Vector(0, 1, 0));
        std::vector<Intersection> xs = Intersections(Intersection(-sqrtf(2)/2, s),
                                                     Intersection(sqrtf(2)/2, s));
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(xs[1], xs);
            const float reflectance = Schlick(comps);
            THEN("It holds") {
                REQUIRE(reflectance == 1.0);
            }
        }
    }
}

SCENARIO("The Schlick approximation with a perpendicular viewing angle") {
    GIVEN("A ray, sphere, and a intersection") {
        std::shared_ptr<Shape> s = glass_sphere();
        const Ray r = Ray(Point(0, 0, 0), Vector(0, 1, 0));
        std::vector<Intersection> xs = Intersections(Intersection(-1, s),
                                                     Intersection(1, s));
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(xs[1], xs);
            const float reflectance = Schlick(comps);
            THEN("It holds") {
                REQUIRE(reflectance == Approx(0.04));
            }
        }
    }
}

SCENARIO("The Schlick approximation with small angle and n2 > n1") {
    GIVEN("A ray, sphere, and a intersection") {
        std::shared_ptr<Shape> s = glass_sphere();
        const Ray r = Ray(Point(0, 0.99, -2), Vector(0, 0, 1));
        std::vector<Intersection> xs = Intersections(Intersection(1.8589, s));
        WHEN("When pre-computing") {
            IntersectionComp comps = r.prepare_computations(xs[0], xs);
            const float reflectance = Schlick(comps);
            THEN("It holds") {
                REQUIRE(reflectance == Approx(0.48873));
            }
        }
    }
}

SCENARIO("An intersection can encapsulate `u` and `v`") {
    GIVEN("A triangle") {
        const std::shared_ptr<Triangle> s = std::make_shared<Triangle>(Point(0, 1, 0),
                                                                       Point(-1, 0, 0),
                                                                       Point(1, 0, 0));
        WHEN("Creating an intersection") {
            const Intersection i = Intersection(3.5, s, 0.2, 0.4);
            THEN("It should hold") {
                REQUIRE(i.u() == Approx(0.2));
                REQUIRE(i.v() == Approx(0.4));
            }
        }
    }
}
