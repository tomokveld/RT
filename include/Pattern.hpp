#ifndef Pattern_hpp
#define Pattern_hpp

#include "Canvas.hpp"
#include "Color.hpp"
#include "Matrix.hpp"
#include "Types.hpp"

#include <math.h>

class Pattern : public std::enable_shared_from_this<Pattern> {
public:
    Pattern();
    Matrix<4, 4> const& get_transform() const;
    Matrix<4, 4> const& get_transform_inv() const;
    void set_transform(const Matrix<4, 4> &t);
    Color shape_color_at(const ShapeConstPtr &s, const Tuple &p) const;
    virtual Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const;
    virtual Color uv_color_at(float u, float v) const;
private:
    Matrix<4, 4> transform;
    Matrix<4, 4> transform_inv;
};

class PatternSolid : public Pattern {
public:
    PatternSolid(const Color &c);
    Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const override;
private:
    Color color;
};

class PatternBinary : public Pattern {
public:
    PatternBinary(PatternPtr a, PatternPtr b) : pattern_a(a), pattern_b(b) {}
protected:
    Color color_a(const Tuple &p, const ShapeConstPtr &s = nullptr) const;
    Color color_b(const Tuple &p, const ShapeConstPtr &s = nullptr) const;
private:
    PatternPtr pattern_a;
    PatternPtr pattern_b;
};

class PatternStripe : public PatternBinary {
public:
    PatternStripe(const Color &a, const Color &b) : PatternStripe(std::make_shared<PatternSolid>(a), std::make_shared<PatternSolid>(b)) {}
    PatternStripe(PatternPtr a, PatternPtr b) : PatternBinary(a, b) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const override;
};

class PatternGradient : public PatternBinary {
public:
    PatternGradient(const Color &a, const Color &b) : PatternGradient(std::make_shared<PatternSolid>(a), std::make_shared<PatternSolid>(b)) {}
    PatternGradient(PatternPtr a, PatternPtr b) : PatternBinary(a, b) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const override;
};

class PatternRing : public PatternBinary {
public:
    PatternRing(const Color &a, const Color &b) : PatternRing(std::make_shared<PatternSolid>(a), std::make_shared<PatternSolid>(b)) {}
    PatternRing(PatternPtr a, PatternPtr b) : PatternBinary(a, b) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const override;
};

class PatternCheckers : public PatternBinary {
public:
    PatternCheckers(const Color &a, const Color &b) : PatternCheckers(std::make_shared<PatternSolid>(a), std::make_shared<PatternSolid>(b)) {}
    PatternCheckers(PatternPtr a, PatternPtr b) : PatternBinary(a, b) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const override;
};

class PatternRadialGradient : public PatternBinary {
public:
    PatternRadialGradient(const Color &a, const Color &b) : PatternRadialGradient(std::make_shared<PatternSolid>(a), std::make_shared<PatternSolid>(b)) {}
    PatternRadialGradient(PatternPtr a, PatternPtr b) : PatternBinary(a, b) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const override;
};

class PatternBlend : public PatternBinary {
public:
    PatternBlend(const Color &a, const Color &b) : PatternBlend(std::make_shared<PatternSolid>(a), std::make_shared<PatternSolid>(b)) {}
    PatternBlend(PatternPtr a, PatternPtr b) : PatternBinary(a, b) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s = nullptr) const override;
};

// TODO: Most of the patterns I have use the PatternBinary subclass, however this is not directly applicable here given that it operates on the assumption of 3D points. It delays the transformation of the point with the inverse matrix until a color is picked. Whereas with the UV mapping the point is first transformed to UV space and then passed along to the function to decide on the color. It makes the most sense to transform the point before determining the UV mapping.
// TODO: Need to figure out a nicer way of doing this
class PatternUvCheckers : public Pattern {
public:
    PatternUvCheckers(int width, int height, Color color_a, Color color_b) : width_(width), height_(height), color_a_(color_a), color_b_(color_b) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s) const override;
    Color uv_color_at(float u, float v) const override;
private:
    int width_, height_;
    Color color_a_, color_b_;
};

class PatternUvAlignCheck : public Pattern {
public:
    PatternUvAlignCheck(const Color &main, const Color &ul, const Color &ur, const Color &bl, const Color &br) : main_(main), ul_(ul), ur_(ur), bl_(bl), br_(br) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s) const override;
    Color uv_color_at(float u, float v) const override;
private:
    Color main_, ul_, ur_, bl_, br_;
};

// TODO: Refactor
class PatternCubeMap : public Pattern {
public:
    PatternCubeMap(PatternPtr left, PatternPtr front, PatternPtr right, PatternPtr back, PatternPtr up, PatternPtr down) : faces{left, front, right, back, up, down} {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s) const override;
    Color uv_color_at(float u, float v) const override;
private:
    PatternPtr faces[6];
};

class PatternUVImage : public Pattern {
public:
    PatternUVImage(Canvas canvas) : canvas_(canvas) {}
    Color color_at(const Tuple &p, const ShapeConstPtr &s) const override;
    Color uv_color_at(float u, float v) const override;
private:
    Canvas canvas_;
};

#endif /* Pattern_hpp */
