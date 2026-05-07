#include "platform/window.hpp"
#include "SDL3/SDL_init.h"
#include "math/point.hpp"
#include <utility>

namespace astra::platform {
Window::Window(std::string title, const math::Point& size)
    : window(nullptr), renderer(nullptr), texture(nullptr), title(std::move(title)), windowSize(size) {}

void Window::initialize() {
    window = SDL_CreateWindow(title.c_str(), windowSize.x, windowSize.y, SDL_WINDOW_HIGH_PIXEL_DENSITY);

    SDL_GetWindowSizeInPixels(window, &windowSizeInPixels.x, &windowSizeInPixels.y);

    renderer = SDL_CreateRenderer(window, nullptr);

    if (renderer) {
        SDL_SetRenderVSync(renderer, 1);
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, windowSizeInPixels.x,
                                windowSizeInPixels.y);
}

void Window::destroy() const {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::render(const render::Buffer& buffer) const {
    const auto pitch = static_cast<int32_t>(windowSizeInPixels.x * sizeof(uint32_t));

    SDL_UpdateTexture(texture, nullptr, buffer.data(), pitch);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
const math::Point& Window::getWindowSizeInPixels() const {
    return windowSizeInPixels;
}
}
