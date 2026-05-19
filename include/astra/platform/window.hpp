#pragma once

#include <string>

#include <SDL3/SDL.h>
#include <astra/math/point.hpp>
#include <astra/math/vec2.hpp>

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
