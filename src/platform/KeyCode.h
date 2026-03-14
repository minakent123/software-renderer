#pragma once

#include <cstddef>

namespace platform {

enum class KeyCode : std::size_t
{
    Unknown = 0,
    Escape,
    Count,
};

}  // namespace platform
