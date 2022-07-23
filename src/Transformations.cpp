#include "Transformations.hpp"

namespace Transform {
    Matrix<4, 4> translation(float x, float y, float z) {
        return Matrix<4, 4>({
            {1.0f, 0.0f, 0.0f, x},
            {0.0f, 1.0f, 0.0f, y},
            {0.0f, 0.0f, 1.0f, z},
            {0.0f, 0.0f, 0.0f, 1.0f}});
    }

    Matrix<4, 4> scaling(float x, float y, float z) {
        return Matrix<4, 4>({
            {x, 0.0f, 0.0f, 0.0f},
            {0.0f, y, 0.0f, 0.0f},
            {0.0f, 0.0f, z, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}});
    }

    Matrix<4, 4> rotation_x(float r) {
        float cos_r = cos(r);
        float sin_r = sin(r);
        return Matrix<4, 4>({
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, cos_r, -sin_r, 0.0f},
            {0.0f, sin_r, cos_r, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}});
    }

    Matrix<4, 4> rotation_y(float r) {
        float cos_r = cos(r);
        float sin_r = sin(r);
        return Matrix<4, 4>({
            {cos_r, 0.0f, sin_r, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {-sin_r, 0.0f, cos_r, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}});
    }

    Matrix<4, 4> rotation_z(float r) {
        float cos_r = cos(r);
        float sin_r = sin(r);
        return Matrix<4, 4>({
            {cos_r, -sin_r, 0.0f, 0.0f},
            {sin_r, cos_r, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}});
    }

    Matrix<4, 4> shearing(float xy, float xz, float yx, float yz, float zx, float zy) {
        return Matrix<4, 4>({
            {1.0f, xy, xz, 0.0f},
            {yx, 1.0f, yz, 0.0f},
            {zx, zy, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}});
    }

    Matrix<4, 4> view_transform(const Tuple &from, const Tuple &to, const Tuple &up) {
        const Tuple forward = (to - from).normalize();
        const Tuple left = forward.cross(up.normalize());
        const Tuple true_up = left.cross(forward);
        const Matrix<4, 4> orientation = Matrix<4, 4>({
            {left[0], left[1], left[2], 0.0f},
            {true_up[0], true_up[1], true_up[2], 0.0f},
            {-forward[0], -forward[1], -forward[2], 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}});
        return orientation * Transform::translation(-from[0], -from[1], -from[2]);
    }

    //  Rodrigues' rotation
    Matrix<4, 4> rotation(const Tuple &u, const Tuple &v) {
        float cosPhi = u.dot(v);
        Tuple uv = u.cross(v);
        float uvmagnitude = u.magnitude() * v.magnitude();
        float sinPhi = uv.magnitude() / uvmagnitude;

        Matrix<4, 4> m1 = Matrix<4, 4>({
            {cosPhi, 0.0f, 0.0f, 0.0f},
            {0.0f, cosPhi, 0.0f, 0.0f},
            {0.0f, 0.0f, cosPhi, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}});

        if (abs(sinPhi) < EPSILON)
            return m1;

        Tuple n = uv / sinPhi;
        Matrix<4, 4> m2 = Matrix<4, 4>({
            {n.get_x() * n.get_x(), n.get_x() * n.get_y(), n.get_x() * n.get_z(), 0.0f},
            {n.get_x() * n.get_y(), n.get_y() * n.get_y(), n.get_y() * n.get_z(), 0.0f},
            {n.get_x() * n.get_z(), n.get_z() * n.get_y(), n.get_z() * n.get_z(), 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f}});

        Matrix<4, 4> m3 = Matrix<4, 4>({
            {0.0f, -n.get_z(), n.get_y(), 0.0f},
            {n.get_z(), 0, -n.get_x(), 0.0f},
            {-n.get_y(), n.get_x(), 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f}});

        Matrix<4, 4> m = m1 + (m2 * (1.0f - cosPhi)) + (m3 * sinPhi);
        return m;
    }

}
