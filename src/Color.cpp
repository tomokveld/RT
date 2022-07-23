#include "Color.hpp"

Color::Color() : Tuple() {}

Color::Color(const Tuple &t) : Tuple(t[0], t[1], t[2], t[3]) {}

Color::Color(float r, float g, float b): Tuple(r, g, b, 0.0f) {}

float Color::red() const {
    return this->x;
}

float Color::green() const {
    return this->y;
}

float Color::blue() const {
    return this->z;
}

Color operator*(const Color &lhs, const Color &rhs) {
    return Color(lhs.red() * rhs.red(),
                 lhs.green() * rhs.green(),
                 lhs.blue() * rhs.blue());
}
