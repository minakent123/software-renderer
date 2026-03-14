#include "core/Surface.h"

#include <algorithm>

namespace core {

Surface::Surface(int width, int height)
{
    Resize(width, height);
}

void Surface::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;

    const std::size_t pixelCount = static_cast<std::size_t>(m_Width) * static_cast<std::size_t>(m_Height);
    m_ColorBuffer.resize(pixelCount);
    m_DepthBuffer.resize(pixelCount);
}

void Surface::ClearColor(std::uint32_t color)
{
    std::fill(m_ColorBuffer.begin(), m_ColorBuffer.end(), color);
}

void Surface::ClearDepth(float depth)
{
    std::fill(m_DepthBuffer.begin(), m_DepthBuffer.end(), depth);
}

int Surface::GetWidth() const
{
    return m_Width;
}

int Surface::GetHeight() const
{
    return m_Height;
}

int Surface::GetColorPitchBytes() const
{
    return static_cast<int>(sizeof(std::uint32_t) * static_cast<std::size_t>(m_Width));
}

std::uint32_t* Surface::GetColorData()
{
    return m_ColorBuffer.data();
}

const std::uint32_t* Surface::GetColorData() const
{
    return m_ColorBuffer.data();
}

}  // namespace core
