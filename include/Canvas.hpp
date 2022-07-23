#ifndef Canvas_hpp
#define Canvas_hpp

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <math.h>

#include "Color.hpp"
#include "Helper.hpp"

class Canvas {
public:
    Canvas(uint32_t width, uint32_t height);
    Color get_pixel(uint32_t x, uint32_t y) const;
    void write_pixel(uint32_t x, uint32_t y, Color const &c);
    Color* get_pixels();
    std::string canvas_to_ppm();
    void write_ppm(std::string filename);
    uint32_t get_width() const;
    uint32_t get_height() const;
protected:
    void write_ppm_header(std::ostream &os) const;
    void write_ppm_pixels(std::ostream &os) const;
    void write_ppm_row(std::ostream &os, uint32_t row) const;
    uint32_t write_ppm_pixel(std::ostream &os, uint32_t row, uint32_t col, uint32_t row_width) const;
    uint32_t write_ppm_color_component(std::ostream &os, float value, uint32_t row_width) const;
    std::string color_component_to_ppm_color(float value) const;
private:
    uint32_t width, height;
    Color* pixels;
};

Canvas canvas_from_ppm(const std::string &fname);
Canvas canvas_from_ppm_string(const std::string &s);
Canvas load_ppm(std::istream *is);

#endif /* Canvas_hpp */
