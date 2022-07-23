#ifndef Transformations_hpp
#define Transformations_hpp

#include "Matrix.hpp"

#include <math.h> 

namespace Transform {
    Matrix<4, 4> translation(float x, float y, float z);
    Matrix<4, 4> scaling(float x, float y, float z);
    Matrix<4, 4> rotation_x(float r);
    Matrix<4, 4> rotation_y(float r);
    Matrix<4, 4> rotation_z(float r);
    Matrix<4, 4> shearing(float xy, float xz, float yx, float yz, float zx, float zy);
    Matrix<4, 4> view_transform(const Tuple &from, const Tuple &to, const Tuple &up);
    //  Rodrigues' rotation formula
    Matrix<4, 4> rotation(const Tuple &u, const Tuple &v);
};

#endif /* Transformations_hpp */
