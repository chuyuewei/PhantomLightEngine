/**
 * @file Matrix.h
 * @brief 矩阵数学库
 */

#pragma once

#include <cmath>
#include <array>

#include "Vector.h"
#include "../PhantomLightEngine.h"

namespace PLE {

/**
 * @brief 3x3矩阵类
 */
class PLE_API Matrix3 {
public:
    // 存储格式：行优先
    std::array<float, 9> m;

    // 构造函数
    Matrix3();
    Matrix3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22);
    Matrix3(const Matrix3& other);

    // 访问元素
    float& operator()(int row, int col);
    float operator()(int row, int col) const;

    // 运算符重载
    Matrix3 operator+(const Matrix3& other) const;
    Matrix3 operator-(const Matrix3& other) const;
    Matrix3 operator*(float scalar) const;
    Matrix3 operator*(const Matrix3& other) const;
    Vector3 operator*(const Vector3& vec) const;

    Matrix3& operator+=(const Matrix3& other);
    Matrix3& operator-=(const Matrix3& other);
    Matrix3& operator*=(float scalar);
    Matrix3& operator*=(const Matrix3& other);

    bool operator==(const Matrix3& other) const;
    bool operator!=(const Matrix3& other) const;

    // 矩阵操作
    Matrix3 Transpose() const;
    float Determinant() const;
    Matrix3 Inverse() const;

    // 静态方法
    static Matrix3 Identity();
    static Matrix3 Zero();
    static Matrix3 Scale(float x, float y, float z);
    static Matrix3 Scale(const Vector3& scale);
    static Matrix3 RotationX(float radians);
    static Matrix3 RotationY(float radians);
    static Matrix3 RotationZ(float radians);
    static Matrix3 Rotation(float radiansX, float radiansY, float radiansZ);
};

/**
 * @brief 4x4矩阵类
 */
class PLE_API Matrix4 {
public:
    // 存储格式：行优先
    std::array<float, 16> m;

    // 构造函数
    Matrix4();
    Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);
    Matrix4(const Matrix4& other);
    Matrix4(const Matrix3& mat3);

    // 访问元素
    float& operator()(int row, int col);
    float operator()(int row, int col) const;

    // 运算符重载
    Matrix4 operator+(const Matrix4& other) const;
    Matrix4 operator-(const Matrix4& other) const;
    Matrix4 operator*(float scalar) const;
    Matrix4 operator*(const Matrix4& other) const;
    Vector4 operator*(const Vector4& vec) const;

    Matrix4& operator+=(const Matrix4& other);
    Matrix4& operator-=(const Matrix4& other);
    Matrix4& operator*=(float scalar);
    Matrix4& operator*=(const Matrix4& other);

    bool operator==(const Matrix4& other) const;
    bool operator!=(const Matrix4& other) const;

    // 矩阵操作
    Matrix4 Transpose() const;
    float Determinant() const;
    Matrix4 Inverse() const;

    // 静态方法
    static Matrix4 Identity();
    static Matrix4 Zero();
    static Matrix4 Translation(float x, float y, float z);
    static Matrix4 Translation(const Vector3& translation);
    static Matrix4 Scale(float x, float y, float z);
    static Matrix4 Scale(const Vector3& scale);
    static Matrix4 RotationX(float radians);
    static Matrix4 RotationY(float radians);
    static Matrix4 RotationZ(float radians);
    static Matrix4 Rotation(float radiansX, float radiansY, float radiansZ);
    static Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
    static Matrix4 Perspective(float fovY, float aspectRatio, float nearZ, float farZ);
    static Matrix4 Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ);
};

// Matrix3 实现

inline Matrix3::Matrix3() {
    *this = Identity();
}

inline Matrix3::Matrix3(float m00, float m01, float m02,
                        float m10, float m11, float m12,
                        float m20, float m21, float m22) {
    m[0] = m00; m[1] = m01; m[2] = m02;
    m[3] = m10; m[4] = m11; m[5] = m12;
    m[6] = m20; m[7] = m21; m[8] = m22;
}

inline Matrix3::Matrix3(const Matrix3& other) {
    m = other.m;
}

inline float& Matrix3::operator()(int row, int col) {
    return m[row * 3 + col];
}

inline float Matrix3::operator()(int row, int col) const {
    return m[row * 3 + col];
}

inline Matrix3 Matrix3::operator+(const Matrix3& other) const {
    Matrix3 result;
    for (int i = 0; i < 9; ++i) {
        result.m[i] = m[i] + other.m[i];
    }
    return result;
}

inline Matrix3 Matrix3::operator-(const Matrix3& other) const {
    Matrix3 result;
    for (int i = 0; i < 9; ++i) {
        result.m[i] = m[i] - other.m[i];
    }
    return result;
}

inline Matrix3 Matrix3::operator*(float scalar) const {
    Matrix3 result;
    for (int i = 0; i < 9; ++i) {
        result.m[i] = m[i] * scalar;
    }
    return result;
}

inline Matrix3 Matrix3::operator*(const Matrix3& other) const {
    Matrix3 result = Matrix3::Zero();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                result(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }
    return result;
}

inline Vector3 Matrix3::operator*(const Vector3& vec) const {
    return Vector3(
        m[0] * vec.x + m[1] * vec.y + m[2] * vec.z,
        m[3] * vec.x + m[4] * vec.y + m[5] * vec.z,
        m[6] * vec.x + m[7] * vec.y + m[8] * vec.z
    );
}

inline Matrix3& Matrix3::operator+=(const Matrix3& other) {
    for (int i = 0; i < 9; ++i) {
        m[i] += other.m[i];
    }
    return *this;
}

inline Matrix3& Matrix3::operator-=(const Matrix3& other) {
    for (int i = 0; i < 9; ++i) {
        m[i] -= other.m[i];
    }
    return *this;
}

inline Matrix3& Matrix3::operator*=(float scalar) {
    for (int i = 0; i < 9; ++i) {
        m[i] *= scalar;
    }
    return *this;
}

inline Matrix3& Matrix3::operator*=(const Matrix3& other) {
    *this = *this * other;
    return *this;
}

inline bool Matrix3::operator==(const Matrix3& other) const {
    for (int i = 0; i < 9; ++i) {
        if (m[i] != other.m[i]) {
            return false;
        }
    }
    return true;
}

inline bool Matrix3::operator!=(const Matrix3& other) const {
    return !(*this == other);
}

inline Matrix3 Matrix3::Transpose() const {
    return Matrix3(
        m[0], m[3], m[6],
        m[1], m[4], m[7],
        m[2], m[5], m[8]
    );
}

inline float Matrix3::Determinant() const {
    return m[0] * (m[4] * m[8] - m[5] * m[7]) -
           m[1] * (m[3] * m[8] - m[5] * m[6]) +
           m[2] * (m[3] * m[7] - m[4] * m[6]);
}

inline Matrix3 Matrix3::Inverse() const {
    float det = Determinant();
    if (std::abs(det) < 1e-6f) {
        return Identity(); // 矩阵不可逆，返回单位矩阵
    }

    float invDet = 1.0f / det;

    Matrix3 result;
    result.m[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
    result.m[1] = (m[2] * m[7] - m[1] * m[8]) * invDet;
    result.m[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;
    result.m[3] = (m[5] * m[6] - m[3] * m[8]) * invDet;
    result.m[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
    result.m[5] = (m[2] * m[3] - m[0] * m[5]) * invDet;
    result.m[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
    result.m[7] = (m[1] * m[6] - m[0] * m[7]) * invDet;
    result.m[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;

    return result;
}

inline Matrix3 Matrix3::Identity() {
    return Matrix3(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

inline Matrix3 Matrix3::Zero() {
    return Matrix3(
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    );
}

inline Matrix3 Matrix3::Scale(float x, float y, float z) {
    return Matrix3(
        x,    0.0f, 0.0f,
        0.0f, y,    0.0f,
        0.0f, 0.0f, z
    );
}

inline Matrix3 Matrix3::Scale(const Vector3& scale) {
    return Scale(scale.x, scale.y, scale.z);
}

inline Matrix3 Matrix3::RotationX(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);

    return Matrix3(
        1.0f, 0.0f, 0.0f,
        0.0f, c,    -s,
        0.0f, s,    c
    );
}

inline Matrix3 Matrix3::RotationY(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);

    return Matrix3(
        c,    0.0f, s,
        0.0f, 1.0f, 0.0f,
        -s,   0.0f, c
    );
}

inline Matrix3 Matrix3::RotationZ(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);

    return Matrix3(
        c,    -s,   0.0f,
        s,    c,    0.0f,
        0.0f, 0.0f, 1.0f
    );
}

inline Matrix3 Matrix3::Rotation(float radiansX, float radiansY, float radiansZ) {
    return RotationZ(radiansZ) * RotationY(radiansY) * RotationX(radiansX);
}

// Matrix4 实现

inline Matrix4::Matrix4() {
    *this = Identity();
}

inline Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                        float m10, float m11, float m12, float m13,
                        float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33) {
    m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
    m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
    m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
    m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
}

inline Matrix4::Matrix4(const Matrix4& other) {
    m = other.m;
}

inline Matrix4::Matrix4(const Matrix3& mat3) {
    m[0] = mat3.m[0]; m[1] = mat3.m[1]; m[2] = mat3.m[2]; m[3] = 0.0f;
    m[4] = mat3.m[3]; m[5] = mat3.m[4]; m[6] = mat3.m[5]; m[7] = 0.0f;
    m[8] = mat3.m[6]; m[9] = mat3.m[7]; m[10] = mat3.m[8]; m[11] = 0.0f;
    m[12] = 0.0f;     m[13] = 0.0f;     m[14] = 0.0f;      m[15] = 1.0f;
}

inline float& Matrix4::operator()(int row, int col) {
    return m[row * 4 + col];
}

inline float Matrix4::operator()(int row, int col) const {
    return m[row * 4 + col];
}

inline Matrix4 Matrix4::operator+(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.m[i] = m[i] + other.m[i];
    }
    return result;
}

inline Matrix4 Matrix4::operator-(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.m[i] = m[i] - other.m[i];
    }
    return result;
}

inline Matrix4 Matrix4::operator*(float scalar) const {
    Matrix4 result;
    for (int i = 0; i < 16; ++i) {
        result.m[i] = m[i] * scalar;
    }
    return result;
}

inline Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result = Matrix4::Zero();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                result(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }
    return result;
}

inline Vector4 Matrix4::operator*(const Vector4& vec) const {
    return Vector4(
        m[0] * vec.x + m[1] * vec.y + m[2] * vec.z + m[3] * vec.w,
        m[4] * vec.x + m[5] * vec.y + m[6] * vec.z + m[7] * vec.w,
        m[8] * vec.x + m[9] * vec.y + m[10] * vec.z + m[11] * vec.w,
        m[12] * vec.x + m[13] * vec.y + m[14] * vec.z + m[15] * vec.w
    );
}

inline Matrix4& Matrix4::operator+=(const Matrix4& other) {
    for (int i = 0; i < 16; ++i) {
        m[i] += other.m[i];
    }
    return *this;
}

inline Matrix4& Matrix4::operator-=(const Matrix4& other) {
    for (int i = 0; i < 16; ++i) {
        m[i] -= other.m[i];
    }
    return *this;
}

inline Matrix4& Matrix4::operator*=(float scalar) {
    for (int i = 0; i < 16; ++i) {
        m[i] *= scalar;
    }
    return *this;
}

inline Matrix4& Matrix4::operator*=(const Matrix4& other) {
    *this = *this * other;
    return *this;
}

inline bool Matrix4::operator==(const Matrix4& other) const {
    for (int i = 0; i < 16; ++i) {
        if (m[i] != other.m[i]) {
            return false;
        }
    }
    return true;
}

inline bool Matrix4::operator!=(const Matrix4& other) const {
    return !(*this == other);
}

inline Matrix4 Matrix4::Transpose() const {
    return Matrix4(
        m[0], m[4], m[8],  m[12],
        m[1], m[5], m[9],  m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    );
}

inline float Matrix4::Determinant() const {
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];

    return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
}

inline Matrix4 Matrix4::Inverse() const {
    float det = Determinant();
    if (std::abs(det) < 1e-6f) {
        return Identity(); // 矩阵不可逆，返回单位矩阵
    }

    float invDet = 1.0f / det;

    Matrix4 result;
    result.m[0] = (m[5] * (m[10] * m[15] - m[11] * m[14]) - m[9] * (m[6] * m[15] - m[7] * m[14]) + m[13] * (m[6] * m[11] - m[7] * m[10])) * invDet;
    result.m[1] = -(m[1] * (m[10] * m[15] - m[11] * m[14]) - m[9] * (m[2] * m[15] - m[3] * m[14]) + m[13] * (m[2] * m[11] - m[3] * m[10])) * invDet;
    result.m[2] = (m[1] * (m[6] * m[15] - m[7] * m[14]) - m[5] * (m[2] * m[15] - m[3] * m[14]) + m[13] * (m[2] * m[7] - m[3] * m[6])) * invDet;
    result.m[3] = -(m[1] * (m[6] * m[11] - m[7] * m[10]) - m[5] * (m[2] * m[11] - m[3] * m[10]) + m[9] * (m[2] * m[7] - m[3] * m[6])) * invDet;

    result.m[4] = -(m[4] * (m[10] * m[15] - m[11] * m[14]) - m[8] * (m[6] * m[15] - m[7] * m[14]) + m[12] * (m[6] * m[11] - m[7] * m[10])) * invDet;
    result.m[5] = (m[0] * (m[10] * m[15] - m[11] * m[14]) - m[8] * (m[2] * m[15] - m[3] * m[14]) + m[12] * (m[2] * m[11] - m[3] * m[10])) * invDet;
    result.m[6] = -(m[0] * (m[6] * m[15] - m[7] * m[14]) - m[4] * (m[2] * m[15] - m[3] * m[14]) + m[12] * (m[2] * m[7] - m[3] * m[6])) * invDet;
    result.m[7] = (m[0] * (m[6] * m[11] - m[7] * m[10]) - m[4] * (m[2] * m[11] - m[3] * m[10]) + m[8] * (m[2] * m[7] - m[3] * m[6])) * invDet;

    result.m[8] = (m[4] * (m[9] * m[15] - m[11] * m[13]) - m[8] * (m[5] * m[15] - m[7] * m[13]) + m[12] * (m[5] * m[11] - m[7] * m[9])) * invDet;
    result.m[9] = -(m[0] * (m[9] * m[15] - m[11] * m[13]) - m[8] * (m[1] * m[15] - m[3] * m[13]) + m[12] * (m[1] * m[11] - m[3] * m[9])) * invDet;
    result.m[10] = (m[0] * (m[5] * m[15] - m[7] * m[13]) - m[4] * (m[1] * m[15] - m[3] * m[13]) + m[12] * (m[1] * m[7] - m[3] * m[5])) * invDet;
    result.m[11] = -(m[0] * (m[5] * m[11] - m[7] * m[9]) - m[4] * (m[1] * m[11] - m[3] * m[9]) + m[8] * (m[1] * m[7] - m[3] * m[5])) * invDet;

    result.m[12] = -(m[4] * (m[9] * m[14] - m[10] * m[13]) - m[8] * (m[5] * m[14] - m[6] * m[13]) + m[12] * (m[5] * m[10] - m[6] * m[9])) * invDet;
    result.m[13] = (m[0] * (m[9] * m[14] - m[10] * m[13]) - m[8] * (m[1] * m[14] - m[2] * m[13]) + m[12] * (m[1] * m[10] - m[2] * m[9])) * invDet;
    result.m[14] = -(m[0] * (m[5] * m[14] - m[6] * m[13]) - m[4] * (m[1] * m[14] - m[2] * m[13]) + m[12] * (m[1] * m[6] - m[2] * m[5])) * invDet;
    result.m[15] = (m[0] * (m[5] * m[10] - m[6] * m[9]) - m[4] * (m[1] * m[10] - m[2] * m[9]) + m[8] * (m[1] * m[6] - m[2] * m[5])) * invDet;

    return result;
}

inline Matrix4 Matrix4::Identity() {
    return Matrix4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

inline Matrix4 Matrix4::Zero() {
    return Matrix4(
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    );
}

inline Matrix4 Matrix4::Translation(float x, float y, float z) {
    return Matrix4(
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

inline Matrix4 Matrix4::Translation(const Vector3& translation) {
    return Translation(translation.x, translation.y, translation.z);
}

inline Matrix4 Matrix4::Scale(float x, float y, float z) {
    return Matrix4(
        x,    0.0f, 0.0f, 0.0f,
        0.0f, y,    0.0f, 0.0f,
        0.0f, 0.0f, z,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

inline Matrix4 Matrix4::Scale(const Vector3& scale) {
    return Scale(scale.x, scale.y, scale.z);
}

inline Matrix4 Matrix4::RotationX(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);

    return Matrix4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, c,    -s,   0.0f,
        0.0f, s,    c,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

inline Matrix4 Matrix4::RotationY(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);

    return Matrix4(
        c,    0.0f, s,    0.0f,
        0.