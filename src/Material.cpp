#include "Material.hpp"
#include "Pattern.hpp"

Material::Material() :
    pattern(std::make_shared<PatternSolid>(Color(1.0f, 1.0f, 1.0f))),
    ambient(0.1f),
    diffuse(0.9f),
    specular(0.9f),
    shininess(200.0f),
    reflective(0.0f),
    transparency(0.0f),
    refractive_index(1.0f),
    shadow(true)
{}

Material::Material(Color color, float ambient, float diffuse, float specular, float shininess, float reflective, float transparency, float refractive_index, bool shadow)
    : pattern(std::make_shared<PatternSolid>(color)),
      ambient(ambient),
      diffuse(diffuse),
      specular(specular),
      shininess(shininess),
      reflective(reflective),
      transparency(transparency),
      refractive_index(refractive_index),
      shadow(shadow)
{}

Color Material::color_at(const ShapeConstPtr &s, const Tuple &p) const {
    return pattern->shape_color_at(s, p);
}

const Color& Material::get_color() const {
    static const Color c = pattern->color_at(Point(0.0f, 0.0f, 0.0f), nullptr);
    return c;
}

void Material::set_color(const Color &c) {
    pattern = std::make_shared<PatternSolid>(c);
}

void Material::set_shadow(bool n_shadow) {
    shadow = n_shadow;
}

float Material::get_reflective() const {
    return reflective;
}

bool Material::get_shadow() const {
    return shadow;
}

float Material::get_transparency() const {
    return transparency;
}

float Material::get_refractive_index() const {
    return refractive_index;
}

float Material::get_ambient() const {
    return ambient;
}

float Material::get_diffuse() const {
    return diffuse;
}

float Material::get_specular() const {
    return specular;
}

float Material::get_shininess() const {
    return shininess;
}

void Material::set_ambient(float n_ambient) {
    ambient = n_ambient;
}

void Material::set_diffuse(float n_diffuse) {
    diffuse = n_diffuse;
}

void Material::set_specular(float n_specular) {
    specular = n_specular;
}

void Material::set_shininess(float n_shininess) {
    shininess = n_shininess;
}

void Material::set_reflective(float n_reflective) {
    reflective = n_reflective;
}

void Material::set_transparency(float n_transparency) {
    transparency = n_transparency;
}
void Material::set_refractive_index(float n_refractive_index) {
    refractive_index = n_refractive_index;
}

bool operator==(const Material &lhs, const Material &rhs) {
    if (&lhs == &rhs)
        return true;
    // TODO: Add == for patterns
    return equal(lhs.ambient, rhs.ambient) &&
           equal(lhs.diffuse, rhs.diffuse) &&
           equal(lhs.specular, rhs.specular) &&
           equal(lhs.shininess, rhs.shininess) &&
           equal(lhs.reflective, rhs.reflective) &&
           equal(lhs.transparency, rhs.transparency) &&
           equal(lhs.refractive_index, rhs.refractive_index);
}

bool operator!=(const Material& lhs, const Material& rhs) {
    return !(lhs == rhs);
}

Material bbox_m() {
    return Material(Color(1.0f, 1.0f, 0.0f), 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.8f, 1.0f, false);
}

//Material::Material() :
//    pattern(std::make_shared<PatternSolid>(Color(1.0f, 1.0f, 1.0f))),
//    ambient(0.1f),
//    diffuse(0.9f),
//    specular(0.9f),
//    shininess(200.0f),
//    reflective(0.0f),
//    transparency(0.0f),
//    refractive_index(1.0f),
//    shadow(true)
//{}
