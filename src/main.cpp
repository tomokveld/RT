#include "Helper.hpp"
#include "Tuple.hpp"
#include "Color.hpp"
#include "Canvas.hpp"
#include "Matrix.hpp"
#include "Transformations.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Shape.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include "PointLight.hpp"
#include "AreaLight.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Plane.hpp"
#include "testHelper.hpp"
#include "Pattern.hpp"
#include "Group.hpp"

int main(int argc, const char * argv[]) {

    std::shared_ptr<Plane> floor = std::make_shared<Plane>();
    floor->mod_material().set_pattern(std::make_shared<PatternCheckers>(Color(0.4, 0.4, 0.4), Color(0.6, 0.6, 0.6)));

    std::shared_ptr<Sphere> sphere_1 = std::make_shared<Sphere>();
    sphere_1->set_transform(Transform::translation(0, 0, 0));
    sphere_1->mod_material().set_color(Color(1, 1, 0));
    sphere_1->mod_material().set_reflective(0.5);
    sphere_1->mod_material().set_specular(1);
    sphere_1->mod_material().set_shininess(20);

    std::shared_ptr<Sphere> sphere_2 = std::make_shared<Sphere>();
    sphere_2->set_transform(Transform::translation(1.65, 0, 0));
    sphere_2->mod_material().set_color(Color(0, 1, 1));
    sphere_2->mod_material().set_reflective(0.5);
    sphere_2->mod_material().set_specular(1);
    sphere_2->mod_material().set_shininess(20);

    std::shared_ptr<Sphere> sphere_3 = std::make_shared<Sphere>();
    sphere_3->set_transform(Transform::translation(0.825, -1.65, 0));
    sphere_3->mod_material().set_color(Color(1, 0, 1));
    sphere_3->mod_material().set_reflective(0.5);
    sphere_3->mod_material().set_specular(1);
    sphere_3->mod_material().set_shininess(20);

    std::shared_ptr<Group> g = std::make_shared<Group>();
    g->add_children(sphere_1, sphere_2, sphere_3);
    g->divide(1);

    World w = World();

    int scale = 1;
    w.insert(std::make_shared<PointLight>(Point(-3, -0.8, -1), Color(1, 1, 1)));
    w.insert(std::make_shared<PointLight>(Point(640 * scale, 480 * scale, -10000), Color(0.5, 0.5, 0.5)));
    w.insert(std::make_shared<AreaLight>(Point(0, 10, 10), Color(1, 1, 1), Vector(1, 0, 0), Vector(0, 1, 0), 16, 16));
//    w.insert(std::make_shared<AreaLight>(Point(-10, 4, 0), Color(1, 1, 1), Vector(1, 0, 0), Vector(0, 1, 0), 8, 8));
//    w.insert(floor);
    w.insert(g);

    Camera c = Camera(640 * scale, 480 * scale, 1);
    c.set_transform(Transform::view_transform(Point(0, 0, -8),
                                              Point(0, 0, 0),
                                              Vector(0, 1, 0)));
    Canvas canvas = c.render(w);
    canvas.write_ppm("/Users/tomb/git/raytracer/output/light.ppm");
}

