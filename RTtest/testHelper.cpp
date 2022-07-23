#include "testHelper.hpp"

TestShape::TestShape() : local_ray(Ray(Point(0, 0, 0), Vector(0, 0, 0))) {
    bounds = Bounds(Point(-1, -1, -1), Point(1, 1, 1));
    bounds_transform = bounds;
};

void TestShape::intersect(const Ray &r, std::vector<Intersection> &xs) const {
    const_cast<TestShape*>(this)->local_ray = r;
}

Tuple TestShape::normal_at_local(const Tuple &p, const Intersection &i) const {
    return Vector(p[0], p[1], p[2]);
}

bool TestShape::operator==(const Shape &rhs) const {
    const TestShape *rhs_test_shape = dynamic_cast<const TestShape*>(&rhs);
    return rhs_test_shape && Shape::operator==(rhs);
}

TestPattern::TestPattern() {};

Color TestPattern::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    return Color(p.get_x(), p.get_y(), p.get_z());
}

World default_world() {
    const LightPtr light = std::make_shared<PointLight>(Point(-10, 10, -10), Color(1, 1, 1));

    const std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>();
    Material m = Material();
    s1->mod_material().set_color(Color(0.8, 1.0, 0.6));
    s1->mod_material().set_diffuse(0.7);
    s1->mod_material().set_specular(0.2);
    
    const std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>();
    s2->set_transform(Transform::scaling(0.5, 0.5, 0.5));
    
    return World({s1, s2},{light});
}

ShapePtr glass_sphere() {
    std::shared_ptr<Sphere> s = std::make_shared<Sphere>();
    s->mod_material().set_transparency(1.0);
    s->mod_material().set_refractive_index(1.5);
    return s;
}


ShapePtr MappedCube() {
    std::shared_ptr<Cube> c = std::make_shared<Cube>();
    
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
    
    c->mod_material().set_pattern(std::make_shared<PatternCubeMap>(left, front, right, back, up, down));
    
    return c;
}
 
