#include "Sphere.hpp"
#include "Ray.hpp"
#include "Light.hpp"
#include "World.hpp"

World::World() :
    objects(std::vector<ShapePtr>()),
    lights(std::vector<LightPtr>())
{}

World::World(std::initializer_list<ShapePtr> i_objects, std::initializer_list<LightPtr> i_lights) {
    for (auto o : i_objects)
        objects.push_back(o);
    for (auto o : i_lights)
        lights.push_back(o);
}

const std::vector<ShapePtr>& World::get_objects() const {
    return objects;
}

const std::vector<LightPtr>& World::get_lights() const {
    return lights;
}

Color World::shade_hit(const IntersectionComp &comps, uint8_t remaining) const {
    Color surface = Color();
    for (auto const &light : lights) {
        const float light_intensity = light->intensity_at(comps.over_point, *this);
        surface = surface + Lighting(comps.object->get_material(), comps.object, light, comps.over_point, comps.eyev, comps.normalv, light_intensity);
    }
    // TODO: How valid is this?
//    surface = surface * (1.0f / lights.size());

    const Color reflected = reflected_color(comps, remaining);
    const Color refracted = refracted_color(comps, remaining);

    // Fresnel/Schlick effect
    const Material material = comps.object->get_material();
    if (material.get_reflective() > 0.0f && material.get_transparency() > 0) {
        const float reflectance = Schlick(comps);
//        const float reflectance = Fresnel(comps);
        return surface + reflected * reflectance + refracted * (1.0f - reflectance);
    }
    return surface + reflected + refracted;
}

Color World::reflected_color(const IntersectionComp &comps, uint8_t remaining) const {
    const float reflective = comps.object->get_material().get_reflective();
    if (reflective == 0.0f || remaining < 1)
        return Color::black();
    
    const Ray reflect_ray = Ray(comps.over_point, comps.reflectv);
    return color_at(reflect_ray, remaining - 1) * reflective;
}

Color World::refracted_color(const IntersectionComp &comps, uint8_t remaining) const {    
    const float transparency = comps.object->get_material().get_transparency();
    if (transparency == 0.0f || remaining < 1)
        return Color::black();

    // Find the ratio of first index of refraction to the second
    const float n_ratio = comps.n1 / comps.n2;
    // cos(theta_i) is the same as the dot product of the two vectors
    const float cos_i = comps.eyev.dot(comps.normalv);
    // Find sin(theta_t)^2
    const float sin2_t = (n_ratio * n_ratio) * (1.0f - (cos_i * cos_i));
    // Test for total internal reflection
    if (sin2_t > 1.0f)
        return Color::black();

    // Find cos(theta_t)
    const float cos_t = sqrtf(1.0f - sin2_t);
    // Compute the direction of the refracted ray
    const Tuple direction = (comps.normalv * ((n_ratio * cos_i) - cos_t)) - (comps.eyev * n_ratio);
    // Create the refracted ray
    const Ray refract_ray = Ray(comps.under_point, direction);

    return color_at(refract_ray, remaining - 1) * transparency;
}

std::vector<LightPtr>& World::mod_lights() {
    return lights;
}

std::vector<ShapePtr>& World::mod_objects() {
    return objects;
}

Color World::color_at(const Ray &r, uint8_t remaining) const {
    std::vector<Intersection> xs;
    r.intersect(*this, xs);
    const auto hit = Hit(xs);
    if (hit.get_shape() == nullptr)
        return Color(0.0f, 0.0f, 0.0f);

    const IntersectionComp comps = r.prepare_computations(hit, xs);
    return shade_hit(comps, remaining);
}

void World::insert(const ShapePtr &s) {
    objects.push_back(s);
}

void World::insert(const LightPtr &l) {
    lights.push_back(l);
}

bool World::is_shadowed(const Tuple &p, const Tuple &light_p) const {
    const Tuple v = light_p - p;
    const float distance = v.magnitude();
    const Tuple direction = v.normalize();
    const Ray r = Ray(p, direction);

    std::vector<Intersection> xs;
    r.intersect(*this, xs);

    Intersection hit = Hit(xs);
    return (hit.get_shape() != nullptr && hit.get_shape()->get_material().get_shadow() && hit.get_distance() < distance);
}
