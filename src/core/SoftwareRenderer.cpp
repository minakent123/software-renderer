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

int Abs(int value)
{
    return value < 0 ? -value : value;
}

void DrawLine(Surface& target, int x0, int y0, int x1, int y1, uint32_t color)
{
    const int deltaX = Abs(x1 - x0);
    const int deltaY = Abs(y1 - y0);
    const int stepX = x0 < x1 ? 1 : -1;
    const int stepY = y0 < y1 ? 1 : -1;

    int error = deltaX - deltaY;

    while (true) {
        target.PutPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        const int doubledError = error * 2;
        if (doubledError > -deltaY) {
            error -= deltaY;
            x0 += stepX;
        }

        if (doubledError < deltaX) {
            error += deltaX;
            y0 += stepY;
        }
    }
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
    const int lineEndX = lineStartX + lineLength - 1;
    const int diagonalStartY = height / 3;
    const int diagonalEndY = diagonalStartY + (height / 3);

    DrawLine(target, lineStartX, centerY, lineEndX, centerY, PackRgba8(255, 255, 255, 255));
    DrawLine(target, lineStartX, diagonalStartY, lineEndX, diagonalEndY, PackRgba8(255, 180, 80, 255));
}

}  // namespace core
