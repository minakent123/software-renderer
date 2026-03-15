#include "core/SoftwareRenderer.h"

#include <cstdint>

#include "core/Surface.h"

namespace core {
namespace {

std::uint32_t PackRgba8(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
    return static_cast<std::uint32_t>(r) | (static_cast<std::uint32_t>(g) << 8U) |
           (static_cast<std::uint32_t>(b) << 16U) | (static_cast<std::uint32_t>(a) << 24U);
}

}  // namespace

void SoftwareRenderer::Render(Surface& target, double /*timeSeconds*/) const
{
    target.ClearColor(PackRgba8(24, 28, 40, 255));
    target.ClearDepth(1.0F);

    const int width = target.GetWidth();
    const int height = target.GetHeight();

    if (width <= 0 || height <= 0) {
        return;
    }

    const int centerX = width / 2;
    const int centerY = height / 2;
    target.PutPixel(centerX, centerY, PackRgba8(255, 255, 255, 255));
}

}  // namespace core
