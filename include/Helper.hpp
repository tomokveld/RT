#ifndef Helper_hpp
#define Helper_hpp

#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>

#include "Tuple.hpp"

// WARNING: The value of epsilon may vary! In case of rendering cylinders/cones it may actually need to be smaller, i.e. 0.000001 rather than 0.00003 in parts with extreme slope.
// WARNING: While a SHADOW_BIAS of 0.0001 will let me pass all tests it adversely affects the visualization of certain effects, for instance PatternRing incorrectly renders and only appears correctly when dropping it down to 0.01. Additionally, it has to be higher when rendering cylinders/cones
constexpr float EPSILON = 0.00001 * 3;
//constexpr float EPSILON = 0.00001;
//constexpr float EPSILON = std::numeric_limits<float>::epsilon() * 0.5; // For actual use

//constexpr float SHADOW_BIAS = 0.00001;
//constexpr float SHADOW_BIAS = EPSILON;
constexpr float SHADOW_BIAS = 0.0006; // For actual use

constexpr uint8_t N_BOUNCE = 4;

constexpr float INF = std::numeric_limits<float>::infinity();

inline bool equal(float x, float y) {
    return fabs(x - y) < EPSILON;
}

// https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
inline double random_dbl() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

static constexpr float mEpsilon =
       std::numeric_limits<float>::epsilon() * 0.5;

inline constexpr float gamma(int n) {
    return (n * mEpsilon) / (1 - n * mEpsilon);
}

inline double diff_of_products(double a, double b, double c, double d) {
    double w = d * c;
    double e = fma(-d, c, w);
    double f = fma(a, b, -w);
    return f + e;
}

inline bool solve_quadratic(float a, float b, float c, float *t0, float *t1);
inline bool solve_quadratic(float a, float b, float c, float *t0, float *t1) {
    double discriminant = (double)b * (double)b - 4.0 * (double)a  * (double)c;
//    double discriminant = diff_of_products((double) b, (double) b, 4.0 * (double) a, (double) c); // *
//    double discriminant = fma(b, b, (c * a) * - 4); // *
    if (discriminant < 0.)
        return false;

    float root_discriminant = std::sqrt(discriminant);
//    double q = -0.5 * (b + copysign(root_discriminant, b)); // *
    float q;
    if (b < 0.0f)
        q = -0.5f * (b - root_discriminant);
    else
        q = -0.5f * (b + root_discriminant);
    *t0 = c / q;
    *t1 = q / a;

    if (*t0 > *t1)
        std::swap(*t0, *t1);
    return true;
}

template<typename T> struct TD;
//TD<decltype(outer)> td;

inline float fclamp(const float x, const float lo, const float hi) {
    return std::min(hi, std::max(x, lo));
}

inline float radians(float deg) {
    return (M_PI / 180.0) * deg;
}

inline float degrees(float rad) {
    return (180.0 / M_PI) * rad;
}

// AABB test
void check_axis(const float &origin, const float &direction, float *tmin, float *tmax, float minv=-1.0, float maxv=1.0);

enum Faces {left, front, right, back, up, down};

// As taken from: https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::istream &sgetline(std::istream &is, std::string &t);

#define IS_DIGIT(x) \
  (static_cast<unsigned int>((x) - '0') < static_cast<unsigned int>(10))
#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t'))
#define IS_NEW_LINE(x) (((x) == '\r') || ((x) == '\n') || ((x) == '\0'))

#define ISECT_NEAR 0.0001f
#define ISECT_FAR 10000.0f

// This function is borrowed from math.h in musl.
// BW = BitWise
inline unsigned int
BW_FLOAT_TO_UINT(float f) {
    union {
        float f;
        unsigned int u;
    } u;
    u.f = f;
    return u.u;
}

typedef struct {
    Tuple n0; float d0;
    Tuple n1; float d1;
    Tuple n2; float d2;
} isect_hh_data;

#endif /* Helper_hpp */

