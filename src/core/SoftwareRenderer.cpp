#include "core/SoftwareRenderer.h"

#include <algorithm>
#include <cmath>
#include <stdint.h>
#include <utility>

#include "core/Surface.h"

namespace core {
namespace {

struct Point
{
    int x;
    int y;
};

struct Color
{
    double r;
    double g;
    double b;
};

struct Vertex
{
    Point position;
    Color color;
};

struct EdgeSample
{
    double x;
    Color color;
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

uint8_t ToByte(double value)
{
    return static_cast<uint8_t>(std::clamp(std::round(value), 0.0, 255.0));
}

uint32_t PackColor(const Color& color)
{
    return PackRgba8(ToByte(color.r), ToByte(color.g), ToByte(color.b), 255);
}

Color LerpColor(const Color& a, const Color& b, double t)
{
    return {
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
    };
}

void SortVerticesByY(Vertex& v0, Vertex& v1, Vertex& v2)
{
    if (v1.position.y < v0.position.y || (v1.position.y == v0.position.y && v1.position.x < v0.position.x)) {
        std::swap(v0, v1);
    }

    if (v2.position.y < v1.position.y || (v2.position.y == v1.position.y && v2.position.x < v1.position.x)) {
        std::swap(v1, v2);
    }

    if (v1.position.y < v0.position.y || (v1.position.y == v0.position.y && v1.position.x < v0.position.x)) {
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

void DrawHorizontalSpanInterpolated(
    Surface& target,
    int x0,
    int x1,
    int y,
    EdgeSample leftSample,
    EdgeSample rightSample)
{
    if (x1 < x0) {
        std::swap(x0, x1);
        std::swap(leftSample, rightSample);
    }

    const double deltaX = rightSample.x - leftSample.x;
    if (deltaX == 0.0) {
        for (int x = x0; x <= x1; ++x) {
            target.PutPixel(x, y, PackColor(leftSample.color));
        }
        return;
    }

    for (int x = x0; x <= x1; ++x) {
        const double sampleX = static_cast<double>(x) + 0.5;
        const double t = std::clamp((sampleX - leftSample.x) / deltaX, 0.0, 1.0);
        target.PutPixel(x, y, PackColor(LerpColor(leftSample.color, rightSample.color, t)));
    }
}

bool IsScanlineInEdgeRange(int y, const Vertex& a, const Vertex& b)
{
    if (a.position.y == b.position.y) {
        return false;
    }

    const int minY = a.position.y < b.position.y ? a.position.y : b.position.y;
    const int maxY = a.position.y < b.position.y ? b.position.y : a.position.y;
    return y >= minY && y < maxY;
}

EdgeSample GetEdgeSample(const Vertex& a, const Vertex& b, int y)
{
    const double scanlineY = static_cast<double>(y) + 0.5;
    const double deltaY = static_cast<double>(b.position.y - a.position.y);
    const double t = (scanlineY - static_cast<double>(a.position.y)) / deltaY;

    return {
        static_cast<double>(a.position.x) + t * static_cast<double>(b.position.x - a.position.x),
        LerpColor(a.color, b.color, t),
    };
}

void FillTriangle(Surface& target, Vertex v0, Vertex v1, Vertex v2)
{
    SortVerticesByY(v0, v1, v2);

    if (v0.position.y == v2.position.y) {
        return;
    }

    for (int y = v0.position.y; y < v2.position.y; ++y) {
        EdgeSample intersections[2] = {};
        int intersectionCount = 0;

        if (IsScanlineInEdgeRange(y, v0, v1)) {
            intersections[intersectionCount++] = GetEdgeSample(v0, v1, y);
        }

        if (IsScanlineInEdgeRange(y, v1, v2)) {
            intersections[intersectionCount++] = GetEdgeSample(v1, v2, y);
        }

        if (IsScanlineInEdgeRange(y, v0, v2)) {
            intersections[intersectionCount++] = GetEdgeSample(v0, v2, y);
        }

        if (intersectionCount != 2) {
            continue;
        }

        const EdgeSample leftSample = intersections[0].x < intersections[1].x ? intersections[0] : intersections[1];
        const EdgeSample rightSample = intersections[0].x < intersections[1].x ? intersections[1] : intersections[0];
        const int startX = static_cast<int>(std::ceil(leftSample.x - 0.5));
        const int endX = static_cast<int>(std::floor(rightSample.x - 0.5));

        if (startX <= endX) {
            DrawHorizontalSpanInterpolated(target, startX, endX, y, leftSample, rightSample);
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

    const Vertex apex = {{width / 2, height / 4}, {255.0, 80.0, 80.0}};
    const Vertex left = {{width / 4, (height * 3) / 4}, {80.0, 255.0, 120.0}};
    const Vertex right = {{(width * 3) / 4, (height * 3) / 4}, {80.0, 140.0, 255.0}};
    const uint32_t outlineColor = PackRgba8(255, 245, 220, 255);

    FillTriangle(target, apex, left, right);
    DrawLine(target, apex.position.x, apex.position.y, left.position.x, left.position.y, outlineColor);
    DrawLine(target, left.position.x, left.position.y, right.position.x, right.position.y, outlineColor);
    DrawLine(target, right.position.x, right.position.y, apex.position.x, apex.position.y, outlineColor);
}

}  // namespace core
