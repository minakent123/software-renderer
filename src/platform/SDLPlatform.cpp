#include "platform/SDLPlatform.h"

#include <SDL.h>
#include <string>

namespace platform {
namespace {

KeyCode MapKeyCode(SDL_Keycode sdlKeyCode)
{
    switch (sdlKeyCode) {
    case SDLK_ESCAPE:
        return KeyCode::Escape;
    default:
        return KeyCode::Unknown;
    }
}

std::string BuildSdlError(const std::string& message)
{
    return message + ": " + SDL_GetError();
}

}  // namespace

struct SDLPlatform::Impl
{
    SDL_Window* pWindow = nullptr;
    SDL_Renderer* pRenderer = nullptr;
    SDL_Texture* pTexture = nullptr;
    InputState Input;
    int Width = 0;
    int Height = 0;
    bool IsValid = false;
    std::string LastError;
};

SDLPlatform::SDLPlatform(int width, int height, std::string_view title)
    : m_pImpl(std::make_unique<Impl>())
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        m_pImpl->LastError = BuildSdlError("SDL_Init failed");
        return;
    }

    m_pImpl->pWindow = SDL_CreateWindow(
        std::string(title).c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (m_pImpl->pWindow == nullptr) {
        m_pImpl->LastError = BuildSdlError("SDL_CreateWindow failed");
        SDL_Quit();
        return;
    }

    m_pImpl->pRenderer = SDL_CreateRenderer(
        m_pImpl->pWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_pImpl->pRenderer == nullptr) {
        m_pImpl->LastError = BuildSdlError("SDL_CreateRenderer failed");
        SDL_DestroyWindow(m_pImpl->pWindow);
        m_pImpl->pWindow = nullptr;
        SDL_Quit();
        return;
    }

    m_pImpl->Width = width;
    m_pImpl->Height = height;
    m_pImpl->IsValid = true;
}

SDLPlatform::~SDLPlatform()
{
    if (m_pImpl == nullptr) {
        return;
    }

    if (m_pImpl->pTexture != nullptr) {
        SDL_DestroyTexture(m_pImpl->pTexture);
    }

    if (m_pImpl->pRenderer != nullptr) {
        SDL_DestroyRenderer(m_pImpl->pRenderer);
    }

    if (m_pImpl->pWindow != nullptr) {
        SDL_DestroyWindow(m_pImpl->pWindow);
    }

    SDL_Quit();
}

bool SDLPlatform::IsValid() const
{
    return m_pImpl->IsValid;
}

std::string_view SDLPlatform::GetLastError() const
{
    return m_pImpl->LastError;
}

void SDLPlatform::BeginFrame()
{
    if (!m_pImpl->IsValid) {
        return;
    }

    m_pImpl->Input.BeginFrame();
}

void SDLPlatform::PollEvents()
{
    if (!m_pImpl->IsValid) {
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            m_pImpl->Input.Quit = true;
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            const KeyCode code = MapKeyCode(event.key.keysym.sym);
            m_pImpl->Input.SetKeyDown(code, event.type == SDL_KEYDOWN);
            break;
        }

        case SDL_MOUSEMOTION:
            m_pImpl->Input.MouseX = event.motion.x;
            m_pImpl->Input.MouseY = event.motion.y;
            m_pImpl->Input.MouseDeltaX += event.motion.xrel;
            m_pImpl->Input.MouseDeltaY += event.motion.yrel;
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                m_pImpl->Input.MouseDownL = event.type == SDL_MOUSEBUTTONDOWN;
            }

            if (event.button.button == SDL_BUTTON_RIGHT) {
                m_pImpl->Input.MouseDownR = event.type == SDL_MOUSEBUTTONDOWN;
            }
            break;

        case SDL_MOUSEWHEEL:
            m_pImpl->Input.WheelDeltaY += static_cast<float>(event.wheel.preciseY);
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                m_pImpl->Width = event.window.data1;
                m_pImpl->Height = event.window.data2;
            }
            break;

        default:
            break;
        }
    }
}

const InputState& SDLPlatform::GetInput() const
{
    return m_pImpl->Input;
}

void SDLPlatform::PresentRgba8(const std::uint32_t* pPixels, int width, int height, int pitchBytes)
{
    if (!m_pImpl->IsValid || pPixels == nullptr || width <= 0 || height <= 0) {
        return;
    }

    int textureWidth = 0;
    int textureHeight = 0;
    if (m_pImpl->pTexture != nullptr) {
        SDL_QueryTexture(m_pImpl->pTexture, nullptr, nullptr, &textureWidth, &textureHeight);
    }

    if (m_pImpl->pTexture == nullptr || textureWidth != width || textureHeight != height) {
        if (m_pImpl->pTexture != nullptr) {
            SDL_DestroyTexture(m_pImpl->pTexture);
            m_pImpl->pTexture = nullptr;
        }

        m_pImpl->pTexture = SDL_CreateTexture(
            m_pImpl->pRenderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            width,
            height);
        if (m_pImpl->pTexture == nullptr) {
            m_pImpl->LastError = BuildSdlError("SDL_CreateTexture failed");
            m_pImpl->Input.Quit = true;
            return;
        }
    }

    if (SDL_UpdateTexture(m_pImpl->pTexture, nullptr, pPixels, pitchBytes) != 0) {
        m_pImpl->LastError = BuildSdlError("SDL_UpdateTexture failed");
        m_pImpl->Input.Quit = true;
        return;
    }

    if (SDL_RenderClear(m_pImpl->pRenderer) != 0) {
        m_pImpl->LastError = BuildSdlError("SDL_RenderClear failed");
        m_pImpl->Input.Quit = true;
        return;
    }

    if (SDL_RenderCopy(m_pImpl->pRenderer, m_pImpl->pTexture, nullptr, nullptr) != 0) {
        m_pImpl->LastError = BuildSdlError("SDL_RenderCopy failed");
        m_pImpl->Input.Quit = true;
        return;
    }

    SDL_RenderPresent(m_pImpl->pRenderer);
}

double SDLPlatform::GetNowSeconds() const
{
    if (!m_pImpl->IsValid) {
        return 0.0;
    }

    return static_cast<double>(SDL_GetPerformanceCounter()) / static_cast<double>(SDL_GetPerformanceFrequency());
}

int SDLPlatform::GetWidth() const
{
    return m_pImpl->Width;
}

int SDLPlatform::GetHeight() const
{
    return m_pImpl->Height;
}

}  // namespace platform
