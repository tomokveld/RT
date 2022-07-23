#ifndef Shape_hpp
#define Shape_hpp

#include "Helper.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Bounds.hpp"

#include <vector>
#include <memory>

class Shape : public std::enable_shared_from_this<Shape> {
public:
    Shape();
    void set_transform(const Matrix<4, 4> &t);
    Matrix<4, 4> const& get_transform() const;
    Matrix<4, 4> const& get_transform_inv() const;
    ShapePtr get_parent();
    void set_parent(ShapePtr parent_n);
    void set_material(const Material &m);
    const Material& get_material() const;
    void set_bounds(const Bounds &b);
    Bounds& mod_bounds();
    const Bounds& get_bounds() const;
    Material& mod_material();
    Tuple normal_at(const Tuple &p, const Intersection &i) const;
    Tuple world_to_object(const Tuple &world_p) const;
    Tuple normal_to_world(const Tuple &n) const;
    virtual void intersect(const Ray &r, std::vector<Intersection> &xs) const = 0;
    virtual Tuple normal_at_local(const Tuple &p, const Intersection &i) const = 0;
    virtual bool operator==(const Shape &rhs) const = 0;
    virtual void divide(int threshold);
    virtual bool includes(const ShapePtr &s) const;
    virtual void UVMappedPoint(const Tuple &p, float *u, float *v) const;
    Bounds bounds_transform;
    Bounds bounds;
protected:
    bool is_equal(const Shape &rhs) const;
private:
    Matrix<4, 4> transform;
    Matrix<4, 4> transform_inv;
    Material material;
    std::shared_ptr<Shape> parent;
};


#endif /* Shape_hpp */
