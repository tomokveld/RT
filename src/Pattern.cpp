#include "Pattern.hpp"
#include "Shape.hpp"

Pattern::Pattern() :
    transform(Matrix<4, 4>::identity()),
    transform_inv(Matrix<4, 4>::identity())
{}

Matrix<4, 4> const& Pattern::get_transform() const {
    return transform;
}

Matrix<4, 4> const& Pattern::get_transform_inv() const {
    return transform_inv;
}

Color Pattern::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    return Color::black();
}

void Pattern::set_transform(const Matrix<4, 4> &t) {
    transform = t;
    transform_inv = t.inverse();
}

Color Pattern::shape_color_at(const ShapeConstPtr &s, const Tuple &p) const {
    const Tuple o_p = s->world_to_object(p);
    const Tuple p_p = transform_inv * o_p;
    return color_at(p_p, s);
}

Color Pattern::uv_color_at(float u, float v) const {
    return Color::black();  
}

PatternSolid::PatternSolid(const Color &c) : color(c) {}

Color PatternSolid::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    return color;
}

Color PatternBinary::color_a(const Tuple &p, const ShapeConstPtr &s) const {
    return pattern_a->color_at(pattern_a->get_transform_inv() * p, s);
}

Color PatternBinary::color_b(const Tuple &p, const ShapeConstPtr &s) const {
    return pattern_b->color_at(pattern_b->get_transform_inv() * p, s);
}

Color PatternStripe::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    if ((int) std::floor(p.get_x()) % 2)
        return color_b(p, s);
    return color_a(p, s);
}

Color PatternGradient::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    const Tuple distance = color_b(p, s) - color_a(p, s);
    const float fraction = std::clamp(p.get_x(), 0.0f, 1.0f); // TODO: This actually looks better...
//    const float fraction = (p.get_x() + 1.0) * 0.5; // Alternative, not satisfactory
//    const float fraction = p.get_x() - std::floor(p.get_x()); // Gives incorrect gradient
    return color_a(p, s) + distance * fraction;
}

Color PatternRing::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    // TODO: The ring pattern does not correctly render on spheres.
    const float x2 = p.get_x() * p.get_x();
    const float z2 = p.get_z() * p.get_z();
//    const float magnitude = std::sqrtf(x2 * z2); // NOTE: Multiplying here gives an interesting visual effect. Adding and not squaring also produces interesting results
    const float magnitude = std::sqrt(x2 + z2);
    if (((int) std::floor(magnitude)) % 2 == 0)
        return color_a(p, s);
    else
        return color_b(p, s);
}

Color PatternCheckers::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    const int x = floor(p.get_x());
    const int y = floor(p.get_y());
    const int z = floor(p.get_z());
    if ((x + y + z) % 2 == 0)
        return color_a(p, s);
    else
        return color_b(p, s);
}

Color PatternRadialGradient::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    const float magnitude = (p - Point(0, 0, 0)).magnitude();
    const Tuple distance = color_b(p, s) - color_a(p, s);
    return color_a(p, s) + distance * magnitude;
}

Color PatternBlend::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    return (color_a(p, s) + color_b(p, s)) * 0.5f;
}


Color PatternUvCheckers::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    // TODO: Find something nicer
    if (s == nullptr)
        return Color::black();
    float u, v;
    s->UVMappedPoint(get_transform_inv() * p, &u, &v);
//    s->UVMappedPoint(get_transform() * p, &u, &v);
    return uv_color_at(u, v);
}

Color PatternUvCheckers::uv_color_at(float u, float v) const {
    int u2 = (int) floor(u * width_);
    int v2 = (int) floor(v * height_);

    if ((u2 + v2) % 2 == 0)
        return color_a_;
    else
        return color_b_;
}


Color PatternUvAlignCheck::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    // TODO: Find something nicer
    if (s == nullptr)
        return Color::black();
    
    float u, v;
    s->UVMappedPoint(get_transform_inv() * p, &u, &v);
//    s->UVMappedPoint(get_transform() * p, &u, &v);
    return uv_color_at(u, v);
}

Color PatternUvAlignCheck::uv_color_at(float u, float v) const {
    if (v > 0.8f) {
        if (u < 0.2f)
            return ul_;
        if (u > 0.8f)
            return ur_;
    } else if (v < 0.2f) {
        if (u < 0.2f)
            return bl_;
        if (u > 0.8f)
            return br_;
    }
    return main_;
}

Color PatternCubeMap::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    // TODO: Find something nicer
    if (s == nullptr)
        return Color::black();

    float u, v;
    s->UVMappedPoint(get_transform_inv() * p, &u, &v);
    // TODO: face_from_point is now called twice, also once in UVMappedPoint...
    int face = (get_transform_inv() * p).face_from_point();
    return faces[face]->uv_color_at(u, v);
}

Color PatternCubeMap::uv_color_at(float u, float v) const {
    std::cout << "Should never be called!";
    return Color::black();
}

Color PatternUVImage::color_at(const Tuple &p, const ShapeConstPtr &s) const {
    // TODO: Find something nicer
    if (s == nullptr)
        return Color::black();
    float u, v;
    s->UVMappedPoint(get_transform_inv() * p, &u, &v);
    return uv_color_at(u, v);
}

Color PatternUVImage::uv_color_at(float u, float v) const {
    v = 1.0f - v;
    uint32_t x = (uint32_t) round(u * (double) (canvas_.get_width() - 1));
    uint32_t y = (uint32_t) round(v * (double) (canvas_.get_height() - 1));
    return canvas_.get_pixel(x, y);
}
