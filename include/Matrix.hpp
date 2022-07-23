#ifndef Matrix_hpp
#define Matrix_hpp

#include "Tuple.hpp"
#include "Helper.hpp"

#include <initializer_list>
#include <iostream>
#include <string>
#include <sstream>

template <uint8_t ROWS, uint8_t COLS>
class Matrix {
public:
    Matrix() {};
    explicit Matrix(std::initializer_list<std::initializer_list<float>> data);
    static Matrix identity();
    void debug() const;
    bool operator==(const Matrix &rhs) const;
    bool operator!=(const Matrix &rhs) const;
    void transpose();
    Matrix transposed() const;
    float* operator[](int x);
    float determinant() const;
    bool invertible() const;

    float at(uint8_t row, uint8_t col) const {
        return matrix[row][col];
    }

    float& at(uint8_t row, uint8_t col) {
        return matrix[row][col];
    }

    template <uint8_t R=ROWS, uint8_t C=COLS>
    typename std::enable_if<(R > 2) && (C > 2), Matrix<ROWS - 1, COLS - 1>>::type
    submatrix(uint8_t r, uint8_t c) const {
        Matrix<ROWS - 1, COLS - 1> out;
        uint8_t out_i = 0, out_j = 0;
        for (uint8_t i = 0; i < ROWS; i++) {
            if (i == r)
                continue;
            out_j = 0;
            for (uint8_t j = 0; j < COLS; j++) {
                if (j == c)
                    continue;
                out[out_i][out_j] = matrix[i][j];
                out_j++;
            }
            out_i++;
        }
        return out;
    }

    template <uint8_t R=ROWS, uint8_t C=COLS>
    typename std::enable_if<(R > 2) && (C > 2), float>::type
    minor(uint8_t r, uint8_t c) const {
        return submatrix(r, c).determinant();
    }

    template <uint8_t R=ROWS, uint8_t C=COLS>
    typename std::enable_if<(R > 2) && (C > 2), float>::type
    cofactor(uint8_t r, uint8_t c) const {
        return minor(r, c) * (((r + c) % 2 == 0) ? 1 : -1);
    }
    Matrix inverse() const;
    // TODO: enable_if 4x4
    Matrix operator*(const Matrix &rhs) const;
    Tuple operator*(const Tuple &t) const;
    Matrix operator+(const Matrix &m) const;
//    friend Matrix operator*(const float s, const Matrix &m);
//    friend Matrix operator*(const Matrix &m, const float s);
    Matrix operator*(const float s);
protected:
    float matrix[ROWS][COLS];
};

template class Matrix<2,2>;
template class Matrix<3,3>;
template class Matrix<4,4>;

template <uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS> Matrix<ROWS, COLS>::inverse() const {
    Matrix<ROWS, COLS> o;
    float idet = 1.0 / determinant();
    for (uint8_t i = 0; i < ROWS; i++) {
        for (uint8_t j = 0; j < COLS; j++)
            o[i][j] = cofactor(j, i) * idet;
    }
    return o;
}

template <uint8_t ROWS, uint8_t COLS>
bool Matrix<ROWS, COLS>::invertible() const {
    return determinant() != 0;
}

// TODO: Does this actually create a new object?
template <uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS> Matrix<ROWS, COLS>::transposed() const {
    Matrix<ROWS, COLS> out = *this;
    out.transpose();
    return out;
}

template<>
inline float Matrix<2, 2>::determinant() const {
    return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
}

// TODO: Loop unrolling?
template <uint8_t ROWS, uint8_t COLS>
float Matrix<ROWS, COLS>::determinant() const {
    float d = 0.0;
    for (uint8_t j = 0; j < COLS; j++)
        d += matrix[0][j] * cofactor(0, j);
    return d;
}

// TODO: SSE, caching
template <uint8_t ROWS, uint8_t COLS>
void Matrix<ROWS, COLS>::transpose() {
    for (uint8_t i = 0; i < ROWS - 1; i++) {
        for (uint8_t j = i + 1; j < COLS; j++) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }
}

template<>
inline Matrix<2, 2> Matrix<2, 2>::identity() {
    return Matrix<2, 2> {
        {1,0},
        {0,1}};
}

template<>
inline Matrix<3, 3> Matrix<3, 3>::identity() {
    return Matrix<3, 3>{
        {1,0,0},
        {0,1,0},
        {0,0,1}};
}

template<>
inline Matrix<4, 4> Matrix<4, 4>::identity() {
    return Matrix<4, 4>{
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}};
}

// TODO: Add unit test
//template <uint8_t ROWS, uint8_t COLS>
//Matrix<ROWS, COLS> operator*(float s, const Matrix<ROWS, COLS> &m) {
//    Matrix<4, 4> r = Matrix<4, 4>();
//    for (uint8_t i = 0; i < ROWS; i++) {
//        for (uint8_t j = 0; j < COLS; j++) {
//            r.matrix[i][j] = m.matrix[i][j] * s;
//        }
//    }
//    return r;
//}
//
//// TODO: Add unit test
//template <uint8_t ROWS, uint8_t COLS>
//Matrix<ROWS, COLS> operator*(const Matrix<ROWS, COLS> &m, const float s) {
//    return s * m;
//}

template <uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS> Matrix<ROWS, COLS>::operator*(const float s) {
    Matrix<ROWS, COLS> r = *this;
    for (uint8_t i = 0; i < ROWS; i++) {
        for (uint8_t j = 0; j < COLS; j++) {
            r.matrix[i][j] *= s;
        }
    }
    return r;
}

// TODO: Add unit test
template <uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS> Matrix<ROWS, COLS>::operator+(const Matrix<ROWS, COLS> &m) const {
    Matrix<ROWS, COLS> r = *this;
    for (uint8_t i = 0; i < ROWS; i++) {
        for (uint8_t j = 0; j < COLS; j++) {
            r.matrix[i][j] += m.matrix[i][j];
        }
    }
    return r;
}

template <uint8_t ROWS, uint8_t COLS>
Tuple Matrix<ROWS, COLS>::operator* (const Tuple &t) const {
    float x = t.get_x(), y = t.get_y(), z = t.get_z(), w = t.get_w();
    return Tuple(
        matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3] * w,
        matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3] * w,
        matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z + matrix[2][3] * w,
        matrix[3][0] * x + matrix[3][1] * y + matrix[3][2] * z + matrix[3][3] * w);
}

template <uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS> Matrix<ROWS, COLS>::operator* (const Matrix<ROWS, COLS> &rhs) const {
    Matrix<ROWS, COLS> o;
    for (uint8_t i = 0; i < ROWS; i++) {
        for (uint8_t j = 0; j < COLS; j++) {
            o[i][j] = matrix[i][0] * rhs.matrix[0][j] +
                      matrix[i][1] * rhs.matrix[1][j] +
                      matrix[i][2] * rhs.matrix[2][j] +
                      matrix[i][3] * rhs.matrix[3][j];
        }
    }
    return o;
}

template <uint8_t ROWS, uint8_t COLS>
bool Matrix<ROWS, COLS>::operator!= (const Matrix<ROWS, COLS> &rhs) const {
    return !(operator==(rhs));
}

template <uint8_t ROWS, uint8_t COLS>
bool Matrix<ROWS, COLS>::operator== (const Matrix<ROWS, COLS> &rhs) const {
    if (this == &rhs)
        return true;
    for (uint8_t i = 0; i < ROWS; i++) {
        for (uint8_t j = 0; j < COLS; j++) {
            if (!equal(matrix[i][j], rhs.matrix[i][j]))
                return false;
        }
    }
    return true;
}

template <uint8_t ROWS, uint8_t COLS>
Matrix<ROWS,COLS>::Matrix(std::initializer_list<std::initializer_list<float>> data) {
    uint8_t i = 0;
    for (auto row : data) {
        if (i >= ROWS)
            break;
        uint8_t col = 0;
        for (float v : row) {
            if (col >= COLS)
                break;
            matrix[i][col] = v;
            col++;
        }
        i++;
    }
}

template <uint8_t ROWS, uint8_t COLS>
float* Matrix<ROWS, COLS>::operator[](int x) {
    return matrix[x];
}

template <uint8_t ROWS, uint8_t COLS>
std::ostream& operator<<(std::ostream& os, const Matrix<ROWS, COLS> &m) {
    for (uint8_t i = 0; i < ROWS; i++) {
        os << '|';
        for (uint8_t j = 0; j < COLS; j++) {
            os << m.at(i, j);
            if (j != COLS - 1)
                os << ',';
        }
        os << '|' << std::endl;
    }
    return os;
}

template <uint8_t ROWS, uint8_t COLS>
void Matrix<ROWS, COLS>::debug() const {
    std::cout << *this << std::endl;
}

template <uint8_t ROWS, uint8_t COLS>
std::string to_string(const Matrix<ROWS, COLS> &m) {
    std::ostringstream ss;
    ss << m;
    return ss.str();
}

#endif /* Matrix_hpp */
