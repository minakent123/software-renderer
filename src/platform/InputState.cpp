#include "platform/InputState.h"

#include <algorithm>

namespace platform {
namespace {

std::size_t ToIndex(KeyCode code)
{
    return static_cast<std::size_t>(code);
}

}  // namespace

void InputState::BeginFrame()
{
    mouseDeltaX = 0;
    mouseDeltaY = 0;
    wheelDeltaY = 0.0F;
    std::fill(keysPressed.begin(), keysPressed.end(), false);
    std::fill(keysReleased.begin(), keysReleased.end(), false);
}

bool InputState::IsKeyDown(KeyCode code) const
{
    if (code == KeyCode::Unknown) {
        return false;
    }

    return keysDown[ToIndex(code)];
}

void InputState::SetKeyDown(KeyCode code, bool down)
{
    if (code == KeyCode::Unknown) {
        return;
    }

    const std::size_t index = ToIndex(code);
    const bool wasDown = keysDown[index];

    keysDown[index] = down;
    if (!wasDown && down) {
        keysPressed[index] = true;
    }

    if (wasDown && !down) {
        keysReleased[index] = true;
    }
}

}  // namespace platform
