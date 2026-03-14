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
    MouseDeltaX = 0;
    MouseDeltaY = 0;
    WheelDeltaY = 0.0F;
    std::fill(KeysPressed.begin(), KeysPressed.end(), false);
    std::fill(KeysReleased.begin(), KeysReleased.end(), false);
}

bool InputState::IsKeyDown(KeyCode code) const
{
    if (code == KeyCode::Unknown) {
        return false;
    }

    return KeysDown[ToIndex(code)];
}

void InputState::SetKeyDown(KeyCode code, bool down)
{
    if (code == KeyCode::Unknown) {
        return;
    }

    const std::size_t index = ToIndex(code);
    const bool wasDown = KeysDown[index];

    KeysDown[index] = down;
    if (!wasDown && down) {
        KeysPressed[index] = true;
    }

    if (wasDown && !down) {
        KeysReleased[index] = true;
    }
}

}  // namespace platform
