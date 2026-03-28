#pragma once

#include "core/Vector3.h"

namespace core {

struct Vertex3
{
    Vector3 position;
    float r;
    float g;
    float b;
};

inline Vertex3 Translate(const Vertex3& vertex, const Vector3& translation)
{
    return {
        Add(vertex.position, translation),
        vertex.r,
        vertex.g,
        vertex.b,
    };
}

}  // namespace core
