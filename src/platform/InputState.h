#pragma once

#include <array>
#include <cstddef>

#include "platform/KeyCode.h"

namespace platform {

constexpr std::size_t KeyCodeCount = static_cast<std::size_t>(KeyCode::Count);

struct InputState
{
    bool quit = false;
    int mouseX = 0;
    int mouseY = 0;
    int mouseDeltaX = 0;
    int mouseDeltaY = 0;
    float wheelDeltaY = 0.0F;
    bool mouseDownL = false;
    bool mouseDownR = false;
    std::array<bool, KeyCodeCount> keysDown{};
    std::array<bool, KeyCodeCount> keysPressed{};
    std::array<bool, KeyCodeCount> keysReleased{};

    void BeginFrame();
    bool IsKeyDown(KeyCode code) const;
    void SetKeyDown(KeyCode code, bool down);
};

}  // namespace platform
