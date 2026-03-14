#pragma once

#include <array>
#include <cstddef>

#include "platform/KeyCode.h"

namespace platform {

constexpr std::size_t KeyCodeCount = static_cast<std::size_t>(KeyCode::Count);

struct InputState
{
    bool Quit = false;
    int MouseX = 0;
    int MouseY = 0;
    int MouseDeltaX = 0;
    int MouseDeltaY = 0;
    float WheelDeltaY = 0.0F;
    bool MouseDownL = false;
    bool MouseDownR = false;
    std::array<bool, KeyCodeCount> KeysDown{};
    std::array<bool, KeyCodeCount> KeysPressed{};
    std::array<bool, KeyCodeCount> KeysReleased{};

    void BeginFrame();
    bool IsKeyDown(KeyCode code) const;
    void SetKeyDown(KeyCode code, bool down);
};

}  // namespace platform
