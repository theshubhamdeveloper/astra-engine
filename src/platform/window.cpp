#include "platform/window.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "math/point.hpp"
#include <utility>

namespace astra::platform {
    Window::Window(std::string title, const math::Point &size)
        : window(nullptr), renderer(nullptr), texture(nullptr), title(std::move(title)), windowSize(size) {
    }

    void Window::initialize(const bool resizable) {
        Uint32 flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
        if (resizable) {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        window = SDL_CreateWindow(title.c_str(), windowSize.x, windowSize.y,
                                  flags);

        SDL_GetWindowSizeInPixels(window, &windowSizeInPixels.x, &windowSizeInPixels.y);

        dpiScale.x = static_cast<float>(windowSizeInPixels.x) / windowSize.x;
        dpiScale.y = static_cast<float>(windowSizeInPixels.y) / windowSize.y;

        renderer = SDL_CreateRenderer(window, nullptr);

        if (renderer) {
            SDL_SetRenderVSync(renderer, 1);
        }

        createTexture();
    }

    void Window::destroy() const {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Window::render(const render::Buffer &buffer) const {
        const auto pitch = static_cast<int32_t>(windowSizeInPixels.x * sizeof(uint32_t));

        SDL_UpdateTexture(texture, nullptr, buffer.data(), pitch);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    void Window::createTexture() {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                    windowSizeInPixels.x,
                                    windowSizeInPixels.y);
    }

    void Window::updateOnResize() {
        SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
        SDL_GetWindowSizeInPixels(window, &windowSizeInPixels.x, &windowSizeInPixels.y);

        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        createTexture();
    }

    const math::Point &Window::getWindowSize() const {
        return windowSize;
    }

    const math::Point &Window::getWindowSizeInPixels() const {
        return windowSizeInPixels;
    }

    const math::Vec2 &Window::getDpiScale() const {
        return dpiScale;
    };
}