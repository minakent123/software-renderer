#pragma once

#include <memory>
#include <string_view>

#include "platform/IPlatform.h"

namespace platform {

class SDLPlatform final : public IPlatform
{
public:
    SDLPlatform(int width, int height, std::string_view title);
    ~SDLPlatform() override;

    SDLPlatform(const SDLPlatform&) = delete;
    SDLPlatform& operator=(const SDLPlatform&) = delete;
    SDLPlatform(SDLPlatform&&) = delete;
    SDLPlatform& operator=(SDLPlatform&&) = delete;

    bool IsValid() const;
    std::string_view GetLastError() const;
    void BeginFrame() override;
    void PollEvents() override;
    const InputState& GetInput() const override;
    void PresentRgba8(const std::uint32_t* pPixels, int width, int height, int pitchBytes) override;
    double GetNowSeconds() const override;
    int GetWidth() const override;
    int GetHeight() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_pImpl;
};

}  // namespace platform
