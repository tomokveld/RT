#include "Canvas.hpp"

#define PPM_LINE_MAX_WIDTH 70

Canvas::Canvas(uint32_t width, uint32_t height) : width{width}, height{height}, pixels{nullptr} {
    // TODO: Does this correctly zero initialize (using value initialization)?
    // TODO: Free memory
    pixels = new Color[width * height]();
}


void Canvas::write_pixel(uint32_t x, uint32_t y, Color const &c) {
    if (x >= width || y >= height)
        return;
    pixels[x + width * y] = c;
}

uint32_t Canvas::get_width() const {
    return width;
}

uint32_t Canvas::get_height() const {
    return height;
}

Color Canvas::get_pixel(uint32_t x, uint32_t y) const {
    assert(x < width);
    assert(y < height);
    return pixels[x + width * y];
}

std::string Canvas::canvas_to_ppm() {
    std::ostringstream oss;
    write_ppm_header(oss);
    write_ppm_pixels(oss);
    return oss.str();
}

void Canvas::write_ppm_header(std::ostream &os) const {
    // TODO: Replace with std::format
    os << "P3\n" << width << ' ' << height << "\n255" << std::endl;
}

void Canvas::write_ppm_pixels(std::ostream &os) const {
    for (uint32_t i = 0; i < height; i++) {
        write_ppm_row(os, i);
        os << std::endl;
    }
}

void Canvas::write_ppm_row(std::ostream &os, uint32_t row) const {
    uint32_t row_width = 0;
    for (uint32_t i = 0; i < width; i++) {
        row_width = write_ppm_pixel(os, row, i, row_width);
    }
}

uint32_t Canvas::write_ppm_pixel(std::ostream &os, uint32_t row, uint32_t col, uint32_t row_width) const {
    Color c = get_pixel(col, row);
    row_width = write_ppm_color_component(os, c.red(), row_width);
    row_width = write_ppm_color_component(os, c.green(), row_width);
    return write_ppm_color_component(os, c.blue(), row_width);;
}

uint32_t Canvas::write_ppm_color_component(std::ostream &os, float value, uint32_t row_width) const {
    std::string value_str = color_component_to_ppm_color(value);
    
    if ((row_width + value_str.size() + 1) > PPM_LINE_MAX_WIDTH) {
        os << std::endl;
        row_width = 0;
    }
    
    if (row_width > 0) {
        os << ' ';
        row_width++;
    }
    os << value_str;
    return row_width + (uint32_t)value_str.size();
}

std::string Canvas::color_component_to_ppm_color(float value) const {
    return std::to_string(std::clamp((int) round(value * 255.0f), 0, 255));
}

void Canvas::write_ppm(std::string filename) {
    std::ofstream file(filename);
    file << this->canvas_to_ppm();
    file.close();
}

Color* Canvas::get_pixels() {
    return pixels;
}

Canvas canvas_from_ppm(const std::string &fname) {
    std::ifstream ifs(fname);
    if (!ifs) {
        std::cout << "Cannot open file <" << fname << '>' << std::endl;
        exit(1);
    }
    if (ifs.peek() == -1) {
        std::cout << "Trying to read an empty file <" << fname << '>' << std::endl;
        exit(1);
    }
    return load_ppm(&ifs);
}

Canvas canvas_from_ppm_string(const std::string &s) {
    std::stringstream ifs(s);
    if (!ifs) {
        std::cout << "Cannot read string <" << s << '>' << std::endl;
        exit(1);
    }
    return load_ppm(&ifs);
}

Canvas load_ppm(std::istream *is) {
    std::string linebuf;
    uint32_t width, height, max_v;

    sgetline(*is, linebuf);
    if (linebuf[0] != 'P' || linebuf[1] != '3' || linebuf[2] != '\0') {
        std::cout << "Trying to read an unsupported fileformat" << std::endl;
        exit(1);
    }

    // TODO: Simplify
    while (is->peek() == '#')
        sgetline(*is, linebuf);

    *is >> width >> height;
    is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (is->peek() == '#')
        sgetline(*is, linebuf);
    *is >> max_v;
    is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Canvas canvas = Canvas(width, height);
    Color* c = canvas.get_pixels();

    size_t n_line = 1;
    int idx = 0;
    while (is->peek() != -1) {
        sgetline(*is, linebuf);
        n_line++;

        if (linebuf.size() > 0) {
            if (linebuf.back() == '\n')
                linebuf.pop_back();
        }

        if (linebuf.size() > 0) {
            if (linebuf.back() == '\r')
                linebuf.pop_back();
        }

        // Remove leading spaces
        linebuf.erase(0, linebuf.find_first_not_of(" \t"));

        // Skip empty lines
        if (linebuf[0] == '\0')
            continue;

        // Skip comments
        if (linebuf[0] == '#')
            continue;

        std::istringstream is(linebuf);
        int n;
        while (is >> n) {
            (*c)[idx] = (double) n / (double) max_v;
            idx++;
            if (idx > 2) {
                idx = 0;
                c++;
            }
        }
    }

    return canvas;
}
