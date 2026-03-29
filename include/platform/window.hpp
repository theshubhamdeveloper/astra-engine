#pragma once
#include <string>
#include "render/framebuffer.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

namespace astra::platform {
    class Window {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;

        std::string title;
        int32_t width, height;

    public:
        Window(std::string title, int32_t width, int32_t height);

        void initialize();

        void destroy() const;

        void render(const render::Buffer &buffer) const;
    };
}
