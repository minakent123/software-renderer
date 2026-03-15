#pragma once

#include <stdint.h>
#include <vector>

namespace core {

class Surface
{
public:
    Surface() = default;
    Surface(int width, int height);

    void Resize(int width, int height);
    void ClearColor(uint32_t color);
    void ClearDepth(float depth);
    void PutPixel(int x, int y, uint32_t color);

    int GetWidth() const;
    int GetHeight() const;
    int GetColorPitchBytes() const;

    uint32_t* GetColorData();
    const uint32_t* GetColorData() const;

private:
    int m_Width = 0;
    int m_Height = 0;
    std::vector<uint32_t> m_ColorBuffer;
    std::vector<float> m_DepthBuffer;
};

}  // namespace core
