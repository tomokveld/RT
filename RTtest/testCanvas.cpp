#include "catch.hpp"
#include "Canvas.hpp"
#include "Color.hpp"

SCENARIO("Creating a new canvas") {
    GIVEN ("An empty 3x4 Canvas and a black color") {
        uint32_t w = 3;
        uint32_t h = 4;
        Canvas c = Canvas(w, h);
        Color black = Color(0, 0, 0);
        THEN ("The canvas must be 3x4") {
            REQUIRE(c.get_width() == w);
            REQUIRE(c.get_height() == h);
        }
        THEN ("All colors in the canvas must be black") {
            for (int i = 0; i < c.get_width(); i++) {
                for (int j = 0; j < c.get_height(); j++) {
                    REQUIRE(c.get_pixel(i, j) == black);
                }
            }
        }
    }
}

SCENARIO("Writing to a canvas") {
    GIVEN ("An empty canvas and a red color") {
        Canvas c = Canvas(15, 20);
        Color red = Color(1, 0, 0);
        WHEN ("Write the red color to (2, 3)") {
            c.write_pixel(2, 3, red);
            THEN ("The pixel (2, 3) is now red") {
                REQUIRE(c.get_pixel(2, 3) == red);
            }
        }
    }
}

SCENARIO("Writing outside of the canvas is safe") {
    GIVEN ("An empty 3x3 canvas") {
        Canvas c = Canvas(3, 3);
        WHEN ("Write a pixel outside of canvas") {
            c.write_pixel(3, -20, Color(1,1,1));
        }
    }
}

SCENARIO("Constructing the PPM header") {
    GIVEN ("An empty canvas") {
        Canvas c = Canvas(5, 3);
        WHEN ("Call canvas_to_ppm") {
            std::string const ppm = c.canvas_to_ppm();
            THEN ("The header is P3\n5 3\n255") {
                REQUIRE(ppm.substr(0, 11) == "P3\n5 3\n255\n");
            }
        }
    }
}

SCENARIO("Constructing the PPM pixel data") {
    constexpr char const* expected =
        "P3\n"
        "5 3\n"
        "255\n"
        "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";
    
    GIVEN ("An empty canvas and three colors") {
        Canvas c = Canvas(5, 3);
        Color c1 = Color(1.5, 0, 0);
        Color c2 = Color(0, 0.5, 0);
        Color c3 = Color(-0.5, 0, 1);
        WHEN ("The pixels are written and canvas_to_ppm is called") {
            c.write_pixel(0, 0, c1);
            c.write_pixel(2, 1, c2);
            c.write_pixel(4, 2, c3);
            std::string const ppm = c.canvas_to_ppm();
            THEN ("The written pixels should equal") {
                REQUIRE(ppm == expected);
            }
        }
    }
}

SCENARIO("Splitting long lines in PPM files") {
    constexpr char const* expected =
        "P3\n"
        "10 2\n"
        "255\n"
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
        "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
        "153 255 204 153 255 204 153 255 204 153 255 204 153\n";
    
    GIVEN ("A 10x2 canvas") {
        Canvas c = Canvas(10, 2);
        WHEN ("Every pixel is set to (1, 0.8, 0.6) and the canvas is written to PPM") {
            for (int i = 0; i < c.get_width(); i++) {
                for (int j = 0; j < c.get_height(); j++) {
                    c.write_pixel(i, j, Color(1, 0.8, 0.6));
                }
            }
            std::string const ppm = c.canvas_to_ppm();
            THEN ("The written pixels should equal") {
                REQUIRE(ppm == expected);
            }
        }
    }
}

SCENARIO("PPM files are terminated by a newline character") {
    GIVEN ("A 5x3 canvas") {
        Canvas c = Canvas(5, 3);
        WHEN ("Write the canvas to PPM") {
            std::string const ppm = c.canvas_to_ppm();
            THEN("PPM ends with a newline character") {
                REQUIRE(ppm.back() == '\n');
            }
        }
    }
}

SCENARIO("Reading a PPM returns a canvas of the right size") {
    GIVEN("A file (here represented as a string") {
        std::string s = "P3\n10 2\n255\n0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n";
        WHEN("Parsing the string") {
            Canvas canvas = canvas_from_ppm_string(s);
            THEN("It holds") {
                REQUIRE(canvas.get_width() == 10);
                REQUIRE(canvas.get_height() == 2);
            }
        }
    }
}

SCENARIO("Reading pixel data from a PPM file") {
    std::string s = "P3\n4 3\n255\n255 127 0  0 127 255  127 255 0  255 255 255\n0 0 0  255 0 0  0 255 0  0 0 255\n255 255 0  0 255 255  255 0 255  127 127 127\n";
    Canvas canvas = canvas_from_ppm_string(s);
    std::tuple<int, int, Color> p[] = {{0, 0, Color(1, 0.498039, 0)},
                                        {1, 0, Color(0, 0.498039, 1)},
                                        {2, 0, Color(0.498039, 1, 0)},
                                        {3, 0, Color(1, 1, 1)},
                                        {0, 1, Color(0, 0, 0)},
                                        {1, 1, Color(1, 0, 0)},
                                        {2, 1, Color(0, 1, 0)},
                                        {3, 1, Color(0, 0, 1)},
                                        {0, 2, Color(1, 1, 0)},
                                        {1, 2, Color(0, 1, 1)},
                                        {2, 2, Color(1, 0, 1)},
                                        {3, 2, Color(0.498039, 0.498039, 0.498039)}};
    for (auto &[x, y, color] : p) {
        REQUIRE(canvas.get_pixel(x, y) == color);
    }
}

SCENARIO("PPM parsing ignores comment lines") {
    GIVEN("A file (here represented as a string") {
        std::string s = "P3\n# this is a comment\n2 1\n# this, too\n255\n# another comment\n255 255 255\n# oh, no, comments in the pixel data!\n255 0 255\n";
        WHEN("Parsing the string") {
            Canvas canvas = canvas_from_ppm_string(s);
            THEN("It holds") {
                REQUIRE(canvas.get_pixel(0, 0) == Color(1, 1, 1));
                REQUIRE(canvas.get_pixel(1, 0) == Color(1, 0, 1));
            }
        }
    }
}

SCENARIO("PPM parsing allows an RGB triple to span lines") {
    GIVEN("A file (here represented as a string") {
        std::string s = "P3\n1 1\n255\n51\n153\n\n204\n";
        WHEN("Parsing the string") {
            Canvas canvas = canvas_from_ppm_string(s);
            THEN("It holds") {
                REQUIRE(canvas.get_pixel(0, 0) == Color(0.2, 0.6, 0.8));
            }
        }
    }
}

SCENARIO("PPM parsing respects the scale setting") {
    GIVEN("A file (here represented as a string") {
        std::string s = "P3\n2 2\n100\n100 100 100  50 50 50\n75 50 25  0 0 0\n";
        WHEN("Parsing the string") {
            Canvas canvas = canvas_from_ppm_string(s);
            THEN("It holds") {
                REQUIRE(canvas.get_pixel(0, 1) == Color(0.75, 0.5, 0.25));
            }
        }
    }
}
