#ifndef Material_hpp
#define Material_hpp

#include "Helper.hpp"
#include "Color.hpp"
#include "Types.hpp"

class Material {
public:
    Material();
    Material(Color color, float ambient, float diffuse, float specular, float shininess, float reflective, float transparency, float refractive_index, bool shadow);
    const Color& get_color() const;
    float get_ambient() const;
    float get_diffuse() const;
    float get_specular() const;
    float get_shininess() const;
    float get_reflective() const;
    float get_transparency() const;
    float get_refractive_index() const;
    bool get_shadow() const;
    void set_color(const Color &c);
    void set_ambient(float n_ambient);
    void set_diffuse(float n_diffuse);
    void set_specular(float n_specular);
    void set_shininess(float n_shininess);
    void set_reflective(float n_reflective);
    void set_transparency(float n_transparency);
    void set_refractive_index(float n_refractive_index);
    void set_shadow(bool n_shadow);
    friend bool operator==(const Material &lhs, const Material &rhs);
    friend bool operator!=(const Material &lhs, const Material &rhs);
    Color color_at(const ShapeConstPtr &s, const Tuple &p) const;
    void set_pattern(const PatternPtr &p) {pattern = p;}
    const PatternPtr& get_pattern() const {return pattern;}
    PatternPtr& mod_pattern() {return pattern;};
private:
    PatternPtr pattern;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    float reflective;
    float transparency;
    float refractive_index;
    bool shadow;
};

Material bbox_m();

#endif /* Material_hpp */
