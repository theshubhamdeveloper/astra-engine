#include <utility>
#include "platform/window.hpp"
#include "SDL3/SDL_init.h"

namespace astra::platform {
    Window::Window(std::string title, const int32_t width, const int32_t height) : window(nullptr), renderer(nullptr),
        texture(nullptr), title(std::move(title)), width(width), height(height) {
    }

    void Window::initialize() {
        window = SDL_CreateWindow(
            title.c_str(),
            width,
            height,
            0
        );

        renderer = SDL_CreateRenderer(window, nullptr);

        texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            width,
            height
        );
    }

    void Window::destroy() const {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Window::render(const renderer::Buffer &buffer) const {
        const auto pitch = static_cast<int32_t>(width * sizeof(uint32_t));

        SDL_UpdateTexture(texture, nullptr, buffer.data(), pitch);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
}
