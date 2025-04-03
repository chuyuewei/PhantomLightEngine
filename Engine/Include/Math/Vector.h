/**
 * @file Vector.h
 * @brief 向量数学库
 */

#pragma once

#include <cmath>
#include <iostream>

#include "../PhantomLightEngine.h"

namespace PLE {

/**
 * @brief 2D向量类
 */
class PLE_API Vector2 {
public:
    float x, y;

    // 构造函数
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}
    Vector2(const Vector2& other) : x(other.x), y(other.y) {}

    // 运算符重载
    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vector2& other) const { return !(*this == other); }

    // 向量操作
    float Length() const { return std::sqrt(x * x + y * y); }
    float LengthSquared() const { return x * x + y * y; }
    
    Vector2 Normalized() const {
        float length = Length();
        if (length > 0.0f) {
            return Vector2(x / length, y / length);
        }
        return *this;
    }

    void Normalize() {
        float length = Length();
        if (length > 0.0f) {
            x /= length;
            y /= length;
        }
    }

    float Dot(const Vector2& other) const { return x * other.x + y * other.y; }

    // 静态方法
    static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
    static Vector2 One() { return Vector2(1.0f, 1.0f); }
    static Vector2 UnitX() { return Vector2(1.0f, 0.0f); }
    static Vector2 UnitY() { return Vector2(0.0f, 1.0f); }
};

/**
 * @brief 3D向量类
 */
class PLE_API Vector3 {
public:
    float x, y, z;

    // 构造函数
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

    // 运算符重载
    Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(const Vector3& other) const { return !(*this == other); }

    // 向量操作
    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    float LengthSquared() const { return x * x + y * y + z * z; }
    
    Vector3 Normalized() const {
        float length = Length();
        if (length > 0.0f) {
            return Vector3(x / length, y / length, z / length);
        }
        return *this;
    }

    void Normalize() {
        float length = Length();
        if (length > 0.0f) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    float Dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
    
    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // 静态方法
    static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
    static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
    static Vector3 UnitX() { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 UnitY() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 UnitZ() { return Vector3(0.0f, 0.0f, 1.0f); }
    static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
    static Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
    static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
    static Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
    static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }
    static Vector3 Backward() { return Vector3(0.0f, 0.0f, -1.0f); }
};

/**
 * @brief 4D向量类
 */
class PLE_API Vector4 {
public:
    float x, y, z, w;

    // 构造函数
    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector3& vec3, float w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) {}
    Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

    // 运算符重载
    Vector4 operator+(const Vector4& other) const { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
    Vector4 operator-(const Vector4& other) const { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
    Vector4 operator*(float scalar) const { return Vector4(x * scalar, y * scalar, z * scalar, w * scalar); }
    Vector4 operator/(float scalar) const { return Vector4(x / scalar, y / scalar, z / scalar, w / scalar); }

    Vector4& operator+=(const Vector4& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    Vector4& operator-=(const Vector4& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    Vector4& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Vector4& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
    bool operator!=(const Vector4& other) const { return !(*this == other); }

    // 向量操作
    float Length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
    float LengthSquared() const { return x * x + y * y + z * z + w * w; }
    
    Vector4 Normalized() const {
        float length = Length();
        if (length > 0.0f) {
            return Vector4(x / length, y / length, z / length, w / length);
        }
        return *this;
    }

    void Normalize() {
        float length = Length();
        if (length > 0.0f) {
            x /= length;
            y /= length;
            z /= length;
            w /= length;
        }
    }

    float Dot(const Vector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

    // 转换为Vector3
    Vector3 ToVector3() const { return Vector3(x, y, z); }

    // 静态方法
    static Vector4 Zero() { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
    static Vector4 One() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
    static Vector4 UnitX() { return Vector4(1.0f, 0.0f, 0.0f, 0.0f); }
    static Vector4 UnitY() { return Vector4(0.0f, 1.0f, 0.0f, 0.0f); }
    static Vector4 UnitZ() { return Vector4(0.0f, 0.0f, 1.0f, 0.0f); }
    static Vector4 UnitW() { return Vector4(0.0f, 0.0f, 0.0f, 1.0f); }
};

} // namespace PLE