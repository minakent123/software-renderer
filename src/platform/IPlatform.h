#pragma once

#include <cstdint>

#include "platform/InputState.h"

namespace platform {

class IPlatform
{
public:
    virtual ~IPlatform() = default;

    virtual void BeginFrame() = 0;
    virtual void PollEvents() = 0;
    virtual const InputState& GetInput() const = 0;
    virtual void PresentRgba8(const std::uint32_t* pPixels, int width, int height, int pitchBytes) = 0;
    virtual double GetNowSeconds() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
};

}  // namespace platform
