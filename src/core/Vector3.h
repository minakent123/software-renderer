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

}  // namespace core
