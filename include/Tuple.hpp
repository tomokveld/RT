#ifndef Tuple_hpp
#define Tuple_hpp

#include <string>
#include <sstream>
#include <iostream>

class Tuple {
public:
    Tuple();
    Tuple(float x, float y, float z, float w);
    void debug() const;
    bool isPoint() const;
    bool isVector() const;
    float get_x() const;
    float get_y() const;
    float get_z() const;
    float get_w() const;
    float& mod_x();
    float& mod_y();
    float& mod_z();
    float& mod_w();    
    float operator[](size_t x) const;
    float& operator[](size_t x);
    float magnitude() const;
    float dot(const Tuple &rhs) const;
    float absdot(const Tuple &rhs) const;
    Tuple normalize() const;
    Tuple reflect(const Tuple normal) const;
    Tuple cross(const Tuple &rhs) const;
    Tuple min(const Tuple &rhs) const;
    Tuple max(const Tuple &rhs) const;
    int face_from_point() const;
    Tuple operator-(float val) const;
    Tuple operator+(float val) const;
    Tuple operator-() const;
    friend bool operator==(const Tuple &lhs, const Tuple &rhs);
    friend bool operator!=(const Tuple &lhs, const Tuple &rhs);
    friend Tuple operator+(const Tuple &lhs, const Tuple &rhs);
    friend Tuple operator-(const Tuple &lhs, const Tuple &rhs);
    friend Tuple operator*(const Tuple &lhs, const float scale);
    friend Tuple operator*(const Tuple &lhs, const Tuple &rhs);
    friend Tuple operator*(const float scale, const Tuple &rhs);
    friend Tuple operator/(const Tuple &lhs, const float scale);
    friend std::ostream& operator<<(std::ostream& os, const Tuple &t);
    std::string to_string(const Tuple &t);
protected:
    float x, y, z, w;
};

Tuple Point(float x, float y, float z);
Tuple Vector(float x, float y, float z);

#endif /* Tuple_hpp */
