#include "core/SoftwareRenderer.h"

#include <stdint.h>

#include "core/Surface.h"

namespace core {
namespace {

uint32_t PackRgba8(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return static_cast<uint32_t>(r) | (static_cast<uint32_t>(g) << 8U) | (static_cast<uint32_t>(b) << 16U) |
           (static_cast<uint32_t>(a) << 24U);
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

    const int centerY = height / 2;
    const int lineLength = width / 2;
    const int lineStartX = (width - lineLength) / 2;
    const int lineEndX = lineStartX + lineLength;

    for (int x = lineStartX; x < lineEndX; ++x) {
        target.PutPixel(x, centerY, PackRgba8(255, 255, 255, 255));
    }
}

}  // namespace core
