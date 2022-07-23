#include "Tuple.hpp"
#include "Helper.hpp"
#include <stdexcept>
#include <iostream>


Tuple::Tuple() : x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f} { }
Tuple::Tuple(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} { }

bool Tuple::isPoint() const {
    return w == 1.0f;
}

bool Tuple::isVector() const {
    return w == 0.0f;
}

bool operator==(const Tuple &lhs, const Tuple &rhs) {
    if (&lhs == &rhs)
        return true;

    if (lhs.w != rhs.w)
        return false;

    return equal(lhs.x, rhs.x) && equal(lhs.y, rhs.y) && equal(lhs.z, rhs.z);
}

bool operator!=(const Tuple& lhs, const Tuple& rhs) {
    return !(lhs == rhs);
}

Tuple operator+(const Tuple& lhs, const Tuple& rhs) {
    return Tuple(lhs.x + rhs.x,
                 lhs.y + rhs.y,
                 lhs.z + rhs.z,
                 lhs.w + rhs.w);
}

Tuple operator-(const Tuple& lhs, const Tuple& rhs) {
    return Tuple(lhs.x - rhs.x,
                 lhs.y - rhs.y,
                 lhs.z - rhs.z,
                 lhs.w - rhs.w);
}

Tuple operator*(const Tuple &lhs, const float scale) {
    return Tuple(lhs.x * scale,
                 lhs.y * scale,
                 lhs.z * scale,
                 lhs.w * scale);
}

float Tuple::get_x() const {
    return x;
};

float Tuple::get_y() const {
    return y;
}

float Tuple::get_z() const {
    return z;
}

float Tuple::get_w() const {
    return w;
}

float& Tuple::mod_x() {
    return x;
}

float& Tuple::mod_y() {
    return y;
}

float& Tuple::mod_z() {
    return z;
}

float& Tuple::mod_w() {
    return w;
}

Tuple operator*(const float scale, const Tuple& rhs) {
    return rhs * scale;
}

Tuple operator/(const Tuple &lhs, const float scale) {
    return Tuple(lhs.x / scale, lhs.y / scale, lhs.z / scale, lhs.w / scale);
}

Tuple Tuple::operator-() const {
    return Tuple(-x, -y, -z, -w);
}

Tuple Point(float x, float y, float z) {
    return Tuple(x, y, z, 1.0f);
}

Tuple Vector(float x, float y, float z) {
    return Tuple(x, y, z, 0.0f);
}

float Tuple::magnitude() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

Tuple Tuple::normalize() const {
    return *this / this->magnitude();
}

float Tuple::dot(const Tuple& rhs) const {
    return x * rhs.x +
           y * rhs.y +
           z * rhs.z + w * rhs.w;
}

inline float Tuple::absdot(const Tuple &rhs) const {
    return abs(this->dot(rhs));
}

Tuple Tuple::cross(const Tuple &rhs) const {
    double v1x = x, v1y = y, v1z = z;
    double v2x = rhs.x, v2y = rhs.y, v2z = rhs.z;
    return Vector((v1y * v2z) - (v1z * v2y),
                  (v1z * v2x) - (v1x * v2z),
                  (v1x * v2y) - (v1y * v2x));
}

std::string to_string(const Tuple &t) {
    std::ostringstream ss;
    ss << t;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Tuple &t) {
    return os << '{' << t.get_x() << ',' << t.get_y() << ',' << t.get_z() << ',' << t.get_w() <<  '}';
}

void Tuple::debug() const {
    std::cout << *this << std::endl;
}

float Tuple::operator[](size_t x) const {
    return ((float*)(this))[x];
}

float& Tuple::operator[](size_t x) {
    return ((float*)(this))[x];
}

Tuple Tuple::reflect(const Tuple normal) const {
    return (*this) - normal * this->dot(normal) * 2.0f;
}

Tuple Tuple::min(const Tuple &rhs) const {
    return {std::min(x, rhs.x), std::min(y, rhs.y), std::min(z, rhs.z), std::min(w, rhs.w)};
}

Tuple Tuple::max(const Tuple &rhs) const {
    return {std::max(x, rhs.x), std::max(y, rhs.y), std::max(z, rhs.z), std::max(w, rhs.w)};
}

Tuple Tuple::operator-(float val) const {
    return {x - val, y - val, z - val, w};
}


Tuple Tuple::operator+(float val) const {
    return {x + val, y + val, z + val, w};
}


Tuple operator*(const Tuple &lhs, const Tuple &rhs) {
    return {lhs[0] * rhs[0],
            lhs[1] * rhs[1],
            lhs[2] * rhs[2],
            lhs[3] * rhs[3]};
}


int Tuple::face_from_point() const {
    float abs_x = abs(x);
    float abs_y = abs(y);
    float abs_z = abs(z);
    float coord = std::max({abs_x, abs_y, abs_z});

    if (coord == x)
        return Faces::right;
    else if (coord == -x)
        return Faces::left;
    else if (coord == y)
        return Faces::up;
    else if (coord == -y)
        return Faces::down;
    else if (coord == z)
        return Faces::front;
    else
        return Faces::back;
}
