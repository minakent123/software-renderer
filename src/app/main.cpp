#include <iostream>

#include "core/SoftwareRenderer.h"
#include "core/Surface.h"
#include "platform/InputState.h"
#include "platform/KeyCode.h"
#include "platform/SDLPlatform.h"

int main()
{
    platform::SDLPlatform platform(800, 600, "software_renderer");
    if (!platform.IsValid()) {
        std::cerr << "Fatal error: " << platform.GetLastError() << '\n';
        return 1;
    }

    core::Surface surface(platform.GetWidth(), platform.GetHeight());
    core::SoftwareRenderer renderer;

    while (true) {
        platform.BeginFrame();
        platform.PollEvents();

        const platform::InputState& input = platform.GetInput();
        if (input.Quit || input.IsKeyDown(platform::KeyCode::Escape)) {
            break;
        }

        if (surface.GetWidth() != platform.GetWidth() || surface.GetHeight() != platform.GetHeight()) {
            surface.Resize(platform.GetWidth(), platform.GetHeight());
        }

        renderer.Render(surface, platform.GetNowSeconds());
        platform.PresentRgba8(
            surface.GetColorData(),
            surface.GetWidth(),
            surface.GetHeight(),
            surface.GetColorPitchBytes());

        if (!platform.GetLastError().empty()) {
            std::cerr << "Platform error: " << platform.GetLastError() << '\n';
            return 1;
        }
    }

    return 0;
}
