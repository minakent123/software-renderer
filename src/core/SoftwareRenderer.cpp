#include "core/SoftwareRenderer.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "core/Surface.h"

namespace core {
namespace {

std::uint32_t PackRgba8(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
    return static_cast<std::uint32_t>(r) | (static_cast<std::uint32_t>(g) << 8U) |
           (static_cast<std::uint32_t>(b) << 16U) | (static_cast<std::uint32_t>(a) << 24U);
}

std::uint8_t ToChannel(double value)
{
    const double clamped = std::clamp(value, 0.0, 255.0);
    return static_cast<std::uint8_t>(clamped);
}

}  // namespace

void SoftwareRenderer::Render(Surface& target, double timeSeconds) const
{
    target.ClearDepth(1.0F);

    const int width = target.GetWidth();
    const int height = target.GetHeight();
    std::uint32_t* pColorBuffer = target.GetColorData();

    if (width <= 0 || height <= 0 || pColorBuffer == nullptr) {
        return;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const double u = static_cast<double>(x) / static_cast<double>(width);
            const double v = static_cast<double>(y) / static_cast<double>(height);
            const double wave = 0.5 + 0.5 * std::sin(timeSeconds + (u * 6.0) + (v * 4.0));

            const std::uint8_t r = ToChannel(u * 255.0);
            const std::uint8_t g = ToChannel(v * 255.0);
            const std::uint8_t b = ToChannel(wave * 255.0);

            pColorBuffer[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)] =
                PackRgba8(r, g, b, 255);
        }
    }
}

}  // namespace core
