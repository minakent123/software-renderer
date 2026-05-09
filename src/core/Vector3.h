#pragma once

namespace core {

struct Vector3
{
    float x;
    float y;
    float z;
};

inline Vector3 Add(const Vector3& a, const Vector3& b)
{
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    };
}

inline Vector3 Subtract(const Vector3& a, const Vector3& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };
}

inline float Dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3 Cross(const Vector3& a, const Vector3& b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

}  // namespace core
