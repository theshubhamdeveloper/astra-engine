#pragma once
#include "SDL3/SDL_video.h"
#include "math/point.hpp"
#include "math/vec2.hpp"
#include <string>

namespace astra::math {
    struct Color;
}

namespace astra::platform {
    class Window {
        SDL_Window *window;
        SDL_GLContext glContext;

        std::string title;
        math::Point windowSize;
        math::Vec2 dpiScale;

    public:
        Window(std::string title, const math::Point &size);

        void initialize(bool resizable);

        void destroy() const;

        void clear(const math::Color &color) const;

        void render() const;

        void updateOnResize();

        [[nodiscard]] const math::Point &getWindowSize() const;

        [[nodiscard]] const math::Vec2 &getDpiScale() const;
    };
}
