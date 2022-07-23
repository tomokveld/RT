#ifndef Light_hpp
#define Light_hpp

#include "Tuple.hpp"
#include "Color.hpp"
#include "Material.hpp"

class World;

class Light : public std::enable_shared_from_this<Light> {
public:
    Light(Tuple position, Color intensity);
    virtual float intensity_at(const Tuple &p, const World &w) const;
    Tuple position() const;
    Color intensity() const;
    virtual bool operator==(const Light &rhs) const = 0;
    size_t n_samples() const;
    std::vector<Tuple> sample_cache_;
protected:
    size_t n_samples_;
    Tuple position_;
    Color intensity_;
    bool is_equal(const Light &rhs) const;
private:
};

//bool operator==(const PointLight &lhs, const PointLight &rhs) {
//    return lhs.intensity() == rhs.intensity() && lhs.position() == rhs.position();
//}

Color Lighting(const Material &m,
               const ShapeConstPtr &s,
               const LightPtr &light,
               const Tuple &p,
               const Tuple &eyev,
               const Tuple &normalv,
               float light_intensity=1.0);

#endif /* Light_hpp */
