#include "ObjParser.hpp"

// Tries to parse a floating point number located at s.
//
// s_end should be a location in the string where reading should absolutely
// stop. For example at the end of the string, to prevent buffer overflows.
//
// Parses the following EBNF grammar:
//   sign    = "+" | "-" ;
//   END     = ? anything not in digit ?
//   digit   = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
//   integer = [sign] , digit , {digit} ;
//   decimal = integer , ["." , integer] ;
//   float   = ( decimal , END ) | ( decimal , ("E" | "e") , integer , END ) ;
//
//  Valid strings are for example:
//   -0  +3.1417e+2  -0.0E-3  1.0324  -1.41   11e2
//
// If the parsing is a success, result is set to the parsed value and true
// is returned.
//
// The function is greedy and will parse until any of the following happens:
//  - a non-conforming character is encountered.
//  - s_end is reached.
//
// The following situations triggers a failure:
//  - s >= s_end.
//  - parse failure.
//
static bool try_parse_double(const char *s, const char *s_end, double *result) {
    if (s >= s_end) {
        return false;
    }
    double mantissa = 0.0;
    // This exponent is base 2 rather than 10.
    // However the exponent we parse is supposed to be one of ten,
    // thus we must take care to convert the exponent/and or the
    // mantissa to a * 2^E, where a is the mantissa and E is the
    // exponent.
    // To get the final double we will use ldexp, it requires the
    // exponent to be in base 2.
    int exponent = 0;

    // NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
    // TO JUMP OVER DEFINITIONS.
    char sign = '+';
    char exp_sign = '+';
    char const *curr = s;

    // How many characters were read in a loop.
    int read = 0;
    // Tells whether a loop terminated due to reaching s_end.
    bool end_not_reached = false;
    bool leading_decimal_dots = false;

  /*
          BEGIN PARSING.
  */

    // Find out what sign we've got.
    if (*curr == '+' || *curr == '-') {
        sign = *curr;
        curr++;
        if ((curr != s_end) && (*curr == '.')) {
            // accept. Somethig like `.7e+2`, `-.5234`
            leading_decimal_dots = true;
        }
    } else if (IS_DIGIT(*curr)) { /* Pass through. */
    } else if (*curr == '.') {
        // accept. Somethig like `.7e+2`, `-.5234`
        leading_decimal_dots = true;
    } else {
      goto fail;
    }

    // Read the integer part.
    end_not_reached = (curr != s_end);
    if (!leading_decimal_dots) {
        while (end_not_reached && IS_DIGIT(*curr)) {
            mantissa *= 10;
            mantissa += static_cast<int>(*curr - 0x30);
            curr++;
            read++;
            end_not_reached = (curr != s_end);
        }
    // We must make sure we actually got something.
        if (read == 0) goto fail;
    }

    // We allow numbers of form "#", "###" etc.
    if (!end_not_reached) goto assemble;

  // Read the decimal part.
    if (*curr == '.') {
        curr++;
        read = 1;
        end_not_reached = (curr != s_end);
        while (end_not_reached && IS_DIGIT(*curr)) {
            static const double pow_lut[] = {
                1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001,
            };
            const int lut_entries = sizeof pow_lut / sizeof pow_lut[0];
          // NOTE: Don't use powf here, it will absolutely murder precision.
          mantissa += static_cast<int>(*curr - 0x30) *
                    (read < lut_entries ? pow_lut[read] : std::pow(10.0, -read));
          read++;
          curr++;
          end_not_reached = (curr != s_end);
      }
    } else if (*curr == 'e' || *curr == 'E') {
    } else {
        goto assemble;
    }

  if (!end_not_reached) goto assemble;

  // Read the exponent part.
    if (*curr == 'e' || *curr == 'E') {
        curr++;
        // Figure out if a sign is present and if it is.
        end_not_reached = (curr != s_end);
        if (end_not_reached && (*curr == '+' || *curr == '-')) {
            exp_sign = *curr;
            curr++;
        } else if (IS_DIGIT(*curr)) { /* Pass through. */
        } else {
            // Empty E is not allowed.
            goto fail;
        }

        read = 0;
        end_not_reached = (curr != s_end);
        while (end_not_reached && IS_DIGIT(*curr)) {
            // To avoid annoying MSVC's min/max macro definiton,
            // Use hardcoded int max value
            if (exponent > (2147483647/10)) { // 2147483647 = std::numeric_limits<int>::max()
                // Integer overflow
                goto fail;
            }
            exponent *= 10;
            exponent += static_cast<int>(*curr - 0x30);
            curr++;
            read++;
            end_not_reached = (curr != s_end);
        }
        exponent *= (exp_sign == '+' ? 1 : -1);
        if (read == 0) goto fail;
    }

    assemble:
        *result = (sign == '+' ? 1 : -1) *
                  (exponent ? std::ldexp(mantissa * std::pow(5.0, exponent), exponent)
                      : mantissa);
        return true;
    fail:
        return false;
}

static inline double parse_double(const char **token, double default_value = 0.0) {
    (*token) += strspn((*token), " \t");
    const char *end = (*token) + strcspn((*token), " \t\r");
    double val = default_value;
    try_parse_double((*token), end, &val);
    (*token) = end;
    return val;
}

static vertex_index_t parse_triple(const char **token) {
    vertex_index_t vi(static_cast<int>(0));  // 0 is an invalid index in OBJ

    vi.v_idx = atoi((*token));
    (*token) += strcspn((*token), "/ \t\r");
    if ((*token)[0] != '/')
        return vi;
    (*token)++;

    // i//k
    if ((*token)[0] == '/') {
        (*token)++;
        vi.vn_idx = atoi((*token));
        (*token) += strcspn((*token), "/ \t\r");
        return vi;
    }

    // i/j/k or i/j
    vi.vt_idx = atoi((*token));
    (*token) += strcspn((*token), "/ \t\r");
    if ((*token)[0] != '/')
        return vi;

    // i/j/k
    (*token)++;  // skip '/'
    vi.vn_idx = atoi((*token));
    (*token) += strcspn((*token), "/ \t\r");
    return vi;
}

ObjGroup::ObjGroup(std::string name) : name_(name) {}

const std::string ObjGroup::name() const {
    return name_;
}

std::shared_ptr<Group> ObjGroup::to_group() const {
    std::shared_ptr<Group> g = std::make_shared<Group>();
    for (auto face : faces_)
        g->add_child(face);
    return g;
}

void ObjGroup::add_face(ShapePtr face) {
    faces_.push_back(face);
}

ObjParser::ObjParser() : valid_(false) {}

std::shared_ptr<Group> ObjParser::default_group() const {
    return groups_[0]->to_group();
}

bool ObjParser::parse_from_file(const std::string &fname) {
    valid_ = load_obj(fname);
    return valid_;
}

bool ObjParser::parse_from_string(const std::string &s) {
    std::stringstream errss;
    std::stringstream ifs(s);
    if (!ifs) {
//        std::cout << "Cannot open file <" << fname << '>' << std::endl;
//        errss << "Cannot open file <" << fname << '>' << std::endl;
        return false;
    }
    return load_obj(&ifs);
}

const bool ObjParser::valid() const {
    return valid_;
}

const Tuple ObjParser::vertex(size_t v) const {
    assert(v != 0);
    if (v > 0) {
        std::vector<Tuple>::size_type zero_indexed = v - 1;
        assert(zero_indexed < vertices_.size());
        return vertices_[zero_indexed];
    } else {
        std::vector<Tuple>::size_type zero_indexed_from_last = v + 1;
        assert(abs((int) zero_indexed_from_last) < vertices_.size());
        return vertices_[vertices_.size() - 1 + zero_indexed_from_last];
    }
}

const Tuple ObjParser::normal(size_t vn) const {
    assert(vn != 0);
    if (vn > 0) {
        std::vector<Tuple>::size_type idx = vn - 1;
        assert(idx < normals_.size());
        return normals_[idx];
    } else {
        assert(abs((int) vn) <= normals_.size());
        return normals_[normals_.size() + vn];
    }
}

// TODO: add_vertex and add_normal are nearly identical
void ObjParser::add_vertex(const char **token) {
    double x, y, z;
    x = parse_double(token, 0.0);
    y = parse_double(token, 0.0);
    z = parse_double(token, 0.0);
    vertices_.push_back(Point(x, y, z));
}

void ObjParser::add_normal(const char **token) {
    double x, y, z;
    x = parse_double(token, 0.0);
    y = parse_double(token, 0.0);
    z = parse_double(token, 0.0);
    normals_.push_back(Vector(x, y, z));
}

void ObjParser::add_face(const char *token) {
    std::vector<vertex_index_t> fv;
    fv.reserve(3);
    while (!IS_NEW_LINE(token[0])) {
        vertex_index_t vi = parse_triple(&token);
        fv.push_back(vi);
        size_t n = strspn(token, " \t\r");
        token += n;
    }

    // Triangulation
    for (std::vector<vertex_index_t>::size_type i = 1; i < fv.size() - 1; ++i) {
        int vn0 = fv[0].vn_idx;
        int vni = fv[i].vn_idx;
        int vni1 = fv[i + 1].vn_idx;
        if (vn0 == 0 && vni == 0 && vni1 == 0) {
            std::shared_ptr<Triangle> face = std::make_shared<Triangle>(vertex(fv[0].v_idx),
                                                                        vertex(fv[i].v_idx),
                                                                        vertex(fv[i + 1].v_idx));
            groups_.back()->add_face(face);
        } else {
            std::shared_ptr<SmoothTriangle> face = std::make_shared<SmoothTriangle>(vertex(fv[0].v_idx),
                                                                                    vertex(fv[i].v_idx),
                                                                                    vertex(fv[i + 1].v_idx),
                                                                                    normal(vn0),
                                                                                    normal(vni),
                                                                                    normal(vni1));
            groups_.back()->add_face(face);
        }
    }
}

static inline std::string parse_string(const char **token) {
    std::string s;
    (*token) += strspn((*token), " \t");
    size_t e = strcspn((*token), " \t\r");
    s = std::string((*token), &(*token)[e]);
    (*token) += e;
    return s;
}

bool ObjParser::load_obj(const std::string &fname) {
    std::stringstream errss;
    std::ifstream ifs(fname);
    if (!ifs) {
        std::cout << "Cannot open file <" << fname << '>' << std::endl;
        errss << "Cannot open file <" << fname << '>' << std::endl;
        return false;
    }
    return load_obj(&ifs);
}

void ObjParser::add_group(std::string name) {
    auto group = new ObjGroup(name);
    groups_.push_back(group);
}

std::shared_ptr<Group> ObjParser::get_group(std::string name) const {
    for (auto group : groups_) {
        if (name == group->name())
            return group->to_group();
    }
    assert(false);
    return default_group();
}

std::shared_ptr<Group> ObjParser::obj_to_group() const {
    std::shared_ptr<Group> top_group = std::make_shared<Group>();
    for (auto g : groups_) {
        auto group = g->to_group();
        if (group->count() > 0) {
            top_group->add_child(group);
        }
    }
    return top_group;
}

bool ObjParser::load_obj(std::istream *is) {
    groups_.push_back(new ObjGroup(""));
    n_line_ = 0;
    std::string name;
    std::string linebuf;
    while (is->peek() != -1) {
        sgetline(*is, linebuf);
        n_line_++;

        if (linebuf.size() > 0) {
            if (linebuf.back() == '\n')
                linebuf.pop_back();
        }

        if (linebuf.size() > 0) {
            if (linebuf.back() == '\r')
                linebuf.pop_back();
        }

        // Skip empty lines
        if (linebuf.empty())
            continue;

        const char *token = linebuf.c_str();

        // Skip leading space
        token += strspn(token, " \t");
        assert(token);

        // Skip empty line
        if (token[0] == '\0')
            continue;

        // Skip comments
        if (token[0] == '#')
            continue;

        // Vertex
        if (token[0] == 'v' && IS_SPACE(token[1])) {
            token += 2;
            add_vertex(&token);
            continue;
        }

        // Vertex normal
        if (token[0] == 'v' && token[1] == 'n' && IS_SPACE(token[2])) {
            token += 3;
            add_normal(&token);
            continue;
        }

        // Face
        if (token[0] == 'f' && IS_SPACE(token[1])) {
            token += 2;
            token += strspn(token, " \t");
            add_face(token);
            continue;
        }

        // Group
        if (token[0] == 'g' && IS_SPACE(token[1])) {
            std::vector<std::string> names;
            while (!IS_NEW_LINE(token[0])) {
               std::string str = parse_string(&token);
               names.push_back(str);
               token += strspn(token, " \t\r");  // skip tag
            }
            if (names.size() < 2) {
                name = "";
            } else {
                std::stringstream ss;
                ss << names[1];
                for (size_t i = 2; i < names.size(); i++)
                    ss << " " << names[i];
                name = ss.str();
            }
            add_group(name);
            continue;
        }
    }
    return false;
}
