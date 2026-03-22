#include "core/Surface.h"

#include <algorithm>

namespace core {
namespace {

bool TryGetPixelIndex(int width, int height, int x, int y, std::size_t& index)
{
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }

    index = static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x);
    return true;
}

}  // namespace

Surface::Surface(int width, int height)
{
    Resize(width, height);
}

void Surface::Resize(int width, int height)
{
    m_width = width;
    m_height = height;

    const std::size_t pixelCount = static_cast<std::size_t>(m_width) * static_cast<std::size_t>(m_height);
    m_colorBuffer.resize(pixelCount);
    m_depthBuffer.resize(pixelCount);
}

void Surface::ClearColor(uint32_t color)
{
    std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), color);
}

void Surface::ClearDepth(float depth)
{
    std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), depth);
}

void Surface::PutPixel(int x, int y, uint32_t color)
{
    std::size_t index = 0;
    if (!TryGetPixelIndex(m_width, m_height, x, y, index)) {
        return;
    }

    m_colorBuffer[index] = color;
}

bool Surface::PutPixel(int x, int y, uint32_t color, float depth)
{
    std::size_t index = 0;
    if (!TryGetPixelIndex(m_width, m_height, x, y, index)) {
        return false;
    }

    if (depth >= m_depthBuffer[index]) {
        return false;
    }

    m_depthBuffer[index] = depth;
    m_colorBuffer[index] = color;
    return true;
}

int Surface::GetWidth() const
{
    return m_width;
}

int Surface::GetHeight() const
{
    return m_height;
}

int Surface::GetColorPitchBytes() const
{
    return static_cast<int>(sizeof(uint32_t) * static_cast<std::size_t>(m_width));
}

uint32_t* Surface::GetColorData()
{
    return m_colorBuffer.data();
}

const uint32_t* Surface::GetColorData() const
{
    return m_colorBuffer.data();
}

}  // namespace core
