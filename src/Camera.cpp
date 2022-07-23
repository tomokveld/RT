#include "Camera.hpp"

Camera::Camera(uint32_t hsize, uint32_t vsize, float fov) :
    hsize(hsize),
    vsize(vsize),
    fov(fov),
    transform(Matrix<4, 4>::identity()),
    transform_inv(Matrix<4, 4>::identity())
{
    const float half_view = std::tanf(fov / 2.0);
    const float aspect = (float) hsize / (float) vsize;
    if (aspect >= 1) {
        half_width = half_view;
        half_height = half_view / aspect;
    } else {
        half_width = half_view * aspect;
        half_height = half_view;
    }
    pixel_size = (half_width * 2.0) / (float) hsize;
}

float Camera::get_pixel_size() const {
    return pixel_size;
}

uint32_t Camera::get_hsize() const {
    return hsize;
}

uint32_t Camera::get_vsize() const {
    return vsize;
}

float Camera::get_fov() const {
    return fov;
}

const Matrix<4, 4>& Camera::get_transform() const {
    return transform;
}

void Camera::set_transform(const Matrix<4, 4> t) {
    transform = t;
    transform_inv = t.inverse();
}

Ray Camera::ray_for_pixel(uint32_t px, uint32_t py) const {
    // The offset from the edge of the canvas to the pixel's center
    const float x_offset = (px + 0.5f) * pixel_size;
    const float y_offset = (py + 0.5f) * pixel_size;

    // The untransformed coordinates of the pixel in world space
    const float world_x = half_width - x_offset;
    const float world_y = half_height - y_offset;

    // using the camera matrix, transform the canvas point and the origin,
    // and then compute the ray's direction vector.
    const Tuple pixel = transform_inv * Point(world_x, world_y, -1.0);
    const Tuple origin = transform_inv * Point(0, 0, 0);
    const Tuple direction = (pixel - origin).normalize();
    return Ray(origin, direction);
}

// TODO: Gamma correction: https://news.ycombinator.com/item?id=13563577
Canvas Camera::render(const World &w, uint32_t samples) const {
    Canvas image = Canvas(hsize, vsize);
    if (samples <= 1) {
        for (uint32_t y = 0; y < vsize; y++) {
            for (uint32_t x = 0; x < hsize; x++) {
                const Ray r = ray_for_pixel(x, y);
                Color c = w.color_at(r);

                // Using exposure and/or gamma correction causes washed out colors...
                // TODO: Exposure
//                float exposure = -0.66f;
//                c = Color(1.0f - expf(c[0] * exposure), 1.0f - expf(c[1] * exposure), 1.0f - expf(c[2] * exposure));

                // TODO: Gamma correction
                // 1
//                float invgamma = 0.45;
//                c = Color(powf(c[0], invgamma), powf(c[1], invgamma), powf(c[2], invgamma));
                // 2
//                c = Color(sqrt(c[0]), sqrt(c[1]), sqrt(c[2]));

                image.write_pixel(x, y, c);
            }
        }
    } else {
        auto scale = 1.0 / samples;
        for (uint32_t y = 0; y < vsize; y++) {
            for (uint32_t x = 0; x < hsize; x++) {
                Color c = Color(0, 0, 0);
                for (uint32_t s = 0; s <= samples; s++) {
                    auto u = (int)round(x + random_dbl());
                    auto v = (int)round(y + random_dbl());
                    const Ray r = ray_for_pixel(u, v);
                    c = c + w.color_at(r);
                }
                c = c * scale;
//                c = Color(sqrt(c[0]*scale), sqrt(c[1]*scale), sqrt(c[2]*scale));
                image.write_pixel(x, y, c);
            }
        }
    }
    return image;
}

//https://github.com/tylermorganwall/rayrender
//https://github.com/tylermorganwall/rayrender/blob/64bff27e71e905d2775509bf22b47bc11c5f70c9/src/sampler.cpp
//void StratifiedSample2D(vec2 *samp, int nx, int ny, random_gen &rng,
//                        bool jitter) {
//  Float dx = (Float)1 / nx, dy = (Float)1 / ny;
//  for (int y = 0; y < ny; ++y) {
//    for (int x = 0; x < nx; ++x) {
//      Float jx = jitter ? rng.unif_rand() : 0.5f;
//      Float jy = jitter ? rng.unif_rand() : 0.5f;
//      samp->e[0] = std::min((x + jx) * dx, OneMinusEpsilon);
//      samp->e[1] = std::min((y + jy) * dy, OneMinusEpsilon);
//      ++samp;
//    }
//  }
//}
