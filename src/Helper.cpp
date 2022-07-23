#include "Helper.hpp"

void check_axis(const float &origin, const float &direction, float *tmin, float *tmax, float minv, float maxv) {
    const float tmin_numerator = (minv - origin);
    const float tmax_numerator = (maxv - origin);

    float tmin_l;
    float tmax_l;

    if (abs(direction) >= EPSILON) {
        tmin_l = tmin_numerator / direction;
        tmax_l = tmax_numerator / direction;
    } else {
        tmin_l = tmin_numerator * INF;
        tmax_l = tmax_numerator * INF;
    }
    if (tmin_l > tmax_l)
        std::swap(tmin, tmax);

    *tmin = tmin_l;
    *tmax = tmax_l;
}

std::istream &sgetline(std::istream &is, std::string &t) {
    t.clear();
    std::istream::sentry se(is, true);
    std::streambuf *sb = is.rdbuf();
    if (se) {
        for (;;) {
            int c = sb->sbumpc();
            switch (c) {
                case '\n':
                    return is;
                case '\r':
                    if (sb->sgetc() == '\n')
                        sb->sbumpc();
                    return is;
                case std::streambuf::traits_type::eof():
                    // Also handle the case when the last line has no line ending
                    if (t.empty())
                        is.setstate(std::ios::eofbit);
                    return is;
                default:
                    t += static_cast<char>(c);
          }
        }
    }
    return is;
}

extern inline unsigned int BW_FLOAT_TO_UINT(float f);
