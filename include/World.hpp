#ifndef World_hpp
#define World_hpp

#include "Tuple.hpp"
#include "Color.hpp"
#include "Intersection.hpp"
#include "Transformations.hpp"

#include <vector>

class Ray;

class World {
public:
    World();
    World(std::initializer_list<ShapePtr> i_objects, std::initializer_list<LightPtr> i_lights);
    const std::vector<ShapePtr>& get_objects() const;
    const std::vector<LightPtr>& get_lights() const;
    std::vector<ShapePtr>& mod_objects();
    std::vector<LightPtr>& mod_lights();
    Color shade_hit(const IntersectionComp &comps, uint8_t remaining=N_BOUNCE) const;
    Color color_at(const Ray &r, uint8_t remaining=N_BOUNCE) const;
    Color reflected_color(const IntersectionComp &comps, uint8_t remaining=N_BOUNCE) const;
    Color refracted_color(const IntersectionComp &comps, uint8_t remaining=N_BOUNCE) const;
    bool is_shadowed(const Tuple &p, const Tuple &light_p) const;
    void insert(const ShapePtr &s);
    void insert(const LightPtr &l);
private:
    std::vector<ShapePtr> objects;
    std::vector<LightPtr> lights;
};

#endif /* World_hpp */

