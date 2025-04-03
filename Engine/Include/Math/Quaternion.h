/**
 * @file Quaternion.h
 * @brief 四元数数学库
 */

#pragma once

#include <cmath>

#include "Vector.h"
#include "Matrix.h"
#include "../PhantomLightEngine.h"

namespace PLE {

/**
 * @brief 四元数类
 */
class PLE_API Quaternion {
public:
    float x, y, z, w;

    // 构造函数
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Quaternion(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

    // 从轴角构造
    Quaternion(const Vector3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float s = std::sin(halfAngle);
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
        w = std::cos(halfAngle);
    }

    // 从欧拉角构造 (弧度)
    static Quaternion FromEulerAngles(float pitch, float yaw, float roll) {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;

        return q;
    }

    // 从旋转矩阵构造
    static Quaternion FromRotationMatrix(const Matrix3& m) {
        float trace = m(0, 0) + m(1, 1) + m(2, 2);
        Quaternion q;

        if (trace > 0.0f) {
            float s = 0.5f / std::sqrt(trace + 1.0f);
            q.w = 0.25f / s;
            q.x = (m(2, 1) - m(1, 2)) * s;
            q.y = (m(0, 2) - m(2, 0)) * s;
            q.z = (m(1, 0) - m(0, 1)) * s;
        } else {
            if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2)) {
                float s = 2.0f * std::sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2));
                q.w = (m(2, 1) - m(1, 2)) / s;
                q.x = 0.25f * s;
                q.y = (m(0, 1) + m(1, 0)) / s;
                q.z = (m(0, 2) + m(2, 0)) / s;
            } else if (m(1, 1) > m(2, 2)) {
                float s = 2.0f * std::sqrt(1.0f + m(1, 1) - m(0, 0) - m(2, 2));
                q.w = (m(0, 2) - m(2, 0)) / s;
                q.x = (m(0, 1) + m(1, 0)) / s;
                q.y = 0.25f * s;
                q.z = (m(1, 2) + m(2, 1)) / s;
            } else {
                float s = 2.0f * std::sqrt(1.0f + m(2, 2) - m(0, 0) - m(1, 1));
                q.w = (m(1, 0) - m(0, 1)) / s;
                q.x = (m(0, 2) + m(2, 0)) / s;
                q.y = (m(1, 2) + m(2, 1)) / s;
                q.z = 0.25f * s;
            }
        }

        return q;
    }

    // 运算符重载
    Quaternion operator+(const Quaternion& other) const {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Quaternion operator-(const Quaternion& other) const {
        return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y + y * other.w + z * other.x - x * other.z,
            w * other.z + z * other.w + x * other.y - y * other.x,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    Vector3 operator*(const Vector3& v) const {
        // 将向量转换为纯四元数
        Quaternion p(v.x, v.y, v.z, 0.0f);
        
        // 计算 q * p * q^-1
        Quaternion q_inv = Conjugate();
        Quaternion result = *this * p * q_inv;
        
        return Vector3(result.x, result.y, result.z);
    }

    bool operator==(const Quaternion& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool operator!=(const Quaternion& other) const {
        return !(*this == other);
    }

    // 四元数操作
    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    float LengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    void Normalize() {
        float length = Length();
        if (length > 0.0f) {
            float invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }
    }

    Quaternion Normalized() const {
        Quaternion result = *this;
        result.Normalize();
        return result;
    }

    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }

    Quaternion Inverse() const {
        float lengthSq = LengthSquared();
        if (lengthSq > 0.0f) {
            float invLengthSq = 1.0f / lengthSq;
            return Quaternion(-x * invLengthSq, -y * invLengthSq, -z * invLengthSq, w * invLengthSq);
        }
        return *this;
    }

    // 转换为欧拉角 (弧度)
    Vector3 ToEulerAngles() const {
        Vector3 angles;

        // 俯仰角 (pitch)
        float sinp = 2.0f * (w * y - z * x);
        if (std::abs(sinp) >= 1.0f) {
            angles.x = std::copysign(M_PI / 2.0f, sinp); // 使用90度，如果sinp为正则为90度，否则为-90度
        } else {
            angles.x = std::asin(sinp);
        }

        // 偏航角 (yaw)
        float siny_cosp = 2.0f * (w * z + x * y);
        float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
        angles.y = std::atan2(siny_cosp, cosy_cosp);

        // 滚转角 (roll)
        float sinr_cosp = 2.0f * (w * x + y * z);
        float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
        angles.z = std::atan2(sinr_cosp, cosr_cosp);

        return angles;
    }

    // 转换为旋转矩阵
    Matrix3 ToRotationMatrix() const {
        float xx = x * x;
        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yy = y * y;
        float yz = y * z;
        float yw = y * w;
        float zz = z * z;
        float zw = z * w;

        return Matrix3(
            1.0f - 2.0f * (yy + zz), 2.0f * (xy - zw),       2.0f * (xz + yw),
            2.0f * (xy + zw),       1.0f - 2.0f * (xx + zz), 2.0f * (yz - xw),
            2.0f * (xz - yw),       2.0f * (yz + xw),       1.0f - 2.0f * (xx + yy)
        );
    }

    // 转换为4x4矩阵
    Matrix4 ToMatrix4() const {
        Matrix3 rotMat = ToRotationMatrix();
        return Matrix4(rotMat);
    }

    // 球面线性插值
    static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        // 确保t在[0,1]范围内
        t = std::max(0.0f, std::min(t, 1.0f));

        // 计算四元数之间的夹角余弦
        float cosHalfTheta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

        // 如果q1和q2指向相反方向，我们需要反转其中一个以获得较短的路径
        Quaternion q2Copy = q2;
        if (cosHalfTheta < 0.0f) {
            q2Copy = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
            cosHalfTheta = -cosHalfTheta;
        }

        // 如果四元数非常接近，使用线性插值
        if (cosHalfTheta > 0.9999f) {
            return Quaternion(
                q1.x + t * (q2Copy.x - q1.x),
                q1.y + t * (q2Copy.y - q1.y),
                q1.z + t * (q2Copy.z - q1.z),
                q1.w + t * (q2Copy.w - q1.w)
            ).Normalized();
        }

        // 计算球面插值
        float halfTheta = std::acos(cosHalfTheta);
        float sinHalfTheta = std::sqrt(1.0f - cosHalfTheta * cosHalfTheta);

        float ratioA = std::sin((1.0f - t) * halfTheta) / sinHalfTheta;
        float ratioB = std::sin(t * halfTheta) / sinHalfTheta;

        return Quaternion(
            q1.x * ratioA + q2Copy.x * ratioB,
            q1.y * ratioA + q2Copy.y * ratioB,
            q1.z * ratioA + q2Copy.z * ratioB,
            q1.w * ratioA + q2Copy.w * ratioB
        );
    }

    // 静态方法
    static Quaternion Identity() {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }
};

} // namespace PLE