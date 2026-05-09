#pragma once
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "math/point.hpp"
#include "math/vec2.hpp"
#include "render/framebuffer.hpp"
#include <string>

namespace astra::platform {
    class Window {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;

        std::string title;
        math::Point windowSize, windowSizeInPixels;
        math::Vec2 dpiScale;

    public:
        Window(std::string title, const math::Point &size);

        void initialize(bool resizable);

        void destroy() const;

        void render(const render::Buffer &buffer) const;

        void createTexture();

        void updateOnResize();

        const math::Point &getWindowSizeInPixels() const;

        const math::Point &getWindowSize() const;

        const math::Vec2 &getDpiScale() const;
    };
}