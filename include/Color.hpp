#ifndef Color_hpp
#define Color_hpp

#include "Tuple.hpp"

class Color : public Tuple {
public:
    Color();
    Color(const Tuple &t);
    Color(float r, float g, float b);
    float red() const;
    float green() const;
    float blue() const;
    friend Color operator* (const Color& lhs, const Color& rhs);
    static const Color& white() {static Color c(1, 1, 1); return c;}
    static const Color& black() {static Color c(0, 0, 0); return c;}
};

#endif /* Color_hpp */
