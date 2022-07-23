#ifndef obj_parser_hpp
#define obj_parser_hpp

#include "Helper.hpp"
#include "Triangle.hpp"
#include "SmoothTriangle.hpp"
#include "Shape.hpp"
#include "Group.hpp"

#include <string>
#include <vector>
#include <fstream>

struct vertex_index_t {
  int v_idx, vt_idx, vn_idx;
  vertex_index_t() : v_idx(-1), vt_idx(-1), vn_idx(-1) {}
  explicit vertex_index_t(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {}
  vertex_index_t(int vidx, int vtidx, int vnidx)
      : v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {}
};

class ObjGroup {
public:
    ObjGroup(std::string name);
    std::shared_ptr<Group> to_group() const;
    void add_face(ShapePtr face);
    const std::string name() const;
private:
    std::string name_;
    std::vector<ShapePtr> faces_;
};

class ObjParser {
public:
    ObjParser();
    bool parse_from_file(const std::string &fname);
    bool parse_from_string(const std::string &s);
    void add_group(std::string name);
    std::shared_ptr<Group> get_group(std::string name) const;
    std::shared_ptr<Group> default_group() const;
    std::shared_ptr<Group> obj_to_group() const;
    const Tuple vertex(size_t v) const;
    const Tuple normal(size_t v) const;
    const bool valid() const;
private:
    bool load_obj(const std::string &fname);
    bool load_obj(std::istream *is);
    void add_vertex(const char **token);
    void add_normal(const char **token);
    void add_face(const char *token);
    bool valid_;
    std::vector<Tuple> vertices_;
    std::vector<Tuple> normals_;
    std::vector<ObjGroup*> groups_;
    size_t n_line_;
};

#endif /* obj_parser_hpp */
