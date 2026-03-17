#include "core/SoftwareRenderer.h"

#include <cmath>
#include <stdint.h>
#include <utility>

#include "core/Surface.h"

namespace core {
namespace {

struct Point
{
    int X;
    int Y;
};

uint32_t PackRgba8(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return static_cast<uint32_t>(r) | (static_cast<uint32_t>(g) << 8U) | (static_cast<uint32_t>(b) << 16U) |
           (static_cast<uint32_t>(a) << 24U);
}

int Abs(int value)
{
    return value < 0 ? -value : value;
}

void SortVerticesByY(Point& v0, Point& v1, Point& v2)
{
    if (v1.Y < v0.Y || (v1.Y == v0.Y && v1.X < v0.X)) {
        std::swap(v0, v1);
    }

    if (v2.Y < v1.Y || (v2.Y == v1.Y && v2.X < v1.X)) {
        std::swap(v1, v2);
    }

    if (v1.Y < v0.Y || (v1.Y == v0.Y && v1.X < v0.X)) {
        std::swap(v0, v1);
    }
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

void DrawHorizontalSpan(Surface& target, int x0, int x1, int y, uint32_t color)
{
    if (x1 < x0) {
        std::swap(x0, x1);
    }

    for (int x = x0; x <= x1; ++x) {
        target.PutPixel(x, y, color);
    }
}

bool IsScanlineInEdgeRange(int y, const Point& a, const Point& b)
{
    if (a.Y == b.Y) {
        return false;
    }

    const int minY = a.Y < b.Y ? a.Y : b.Y;
    const int maxY = a.Y < b.Y ? b.Y : a.Y;
    return y >= minY && y < maxY;
}

double GetEdgeIntersectionX(const Point& a, const Point& b, int y)
{
    const double scanlineY = static_cast<double>(y) + 0.5;
    const double deltaY = static_cast<double>(b.Y - a.Y);
    const double t = (scanlineY - static_cast<double>(a.Y)) / deltaY;
    return static_cast<double>(a.X) + t * static_cast<double>(b.X - a.X);
}

void FillTriangle(Surface& target, Point v0, Point v1, Point v2, uint32_t color)
{
    SortVerticesByY(v0, v1, v2);

    if (v0.Y == v2.Y) {
        return;
    }

    for (int y = v0.Y; y < v2.Y; ++y) {
        double intersections[2] = {};
        int intersectionCount = 0;

        if (IsScanlineInEdgeRange(y, v0, v1)) {
            intersections[intersectionCount++] = GetEdgeIntersectionX(v0, v1, y);
        }

        if (IsScanlineInEdgeRange(y, v1, v2)) {
            intersections[intersectionCount++] = GetEdgeIntersectionX(v1, v2, y);
        }

        if (IsScanlineInEdgeRange(y, v0, v2)) {
            intersections[intersectionCount++] = GetEdgeIntersectionX(v0, v2, y);
        }

        if (intersectionCount != 2) {
            continue;
        }

        const double leftIntersection = intersections[0] < intersections[1] ? intersections[0] : intersections[1];
        const double rightIntersection = intersections[0] < intersections[1] ? intersections[1] : intersections[0];
        const int startX = static_cast<int>(std::ceil(leftIntersection - 0.5));
        const int endX = static_cast<int>(std::floor(rightIntersection - 0.5));

        if (startX <= endX) {
            DrawHorizontalSpan(target, startX, endX, y, color);
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

    const Point apex = {width / 2, height / 4};
    const Point left = {width / 4, (height * 3) / 4};
    const Point right = {(width * 3) / 4, (height * 3) / 4};
    const uint32_t fillColor = PackRgba8(255, 180, 80, 255);
    const uint32_t outlineColor = PackRgba8(255, 245, 220, 255);

    FillTriangle(target, apex, left, right, fillColor);
    DrawLine(target, apex.X, apex.Y, left.X, left.Y, outlineColor);
    DrawLine(target, left.X, left.Y, right.X, right.Y, outlineColor);
    DrawLine(target, right.X, right.Y, apex.X, apex.Y, outlineColor);
}

}  // namespace core
