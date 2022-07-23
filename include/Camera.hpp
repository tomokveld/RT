#ifndef Camera_hpp
#define Camera_hpp

#include "Canvas.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "Transformations.hpp"

class Ray;

class Camera {
public:
    Camera();
    Camera(uint32_t hsize, uint32_t vsize, float fov);
    uint32_t get_hsize() const;
    uint32_t get_vsize() const;
    float get_fov() const;
    float get_pixel_size() const;
    const Matrix<4, 4>& get_transform() const;
    Ray ray_for_pixel(uint32_t px, uint32_t py) const;
    void set_transform(const Matrix<4, 4> t);
    Canvas render(const World &w, uint32_t samples=1) const;
private:
    Matrix<4, 4> transform;
    Matrix<4, 4> transform_inv;
    uint32_t hsize;
    uint32_t vsize;
    float fov;
    float half_width;
    float half_height;
    float pixel_size;
};

#endif /* Camera_hpp */
