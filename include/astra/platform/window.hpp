#pragma once

#include <string>

#include <SDL3/SDL.h>
#include <astra/math/vector.hpp>

namespace astra::math {
    struct Color;
}

namespace astra::platform {
    class Window {
        SDL_Window *window;
        SDL_GLContext glContext;

        std::string title;
        math::Vec2 windowSize;
        math::Vec2 dpiScale;

    public:
        Window(std::string title, const math::Vec2 &size);

        void initialize(bool resizable);

        void destroy() const;

        void clear(const math::Color &color) const;

        void render() const;

        void updateOnResize();

        [[nodiscard]] const math::Vec2 &getWindowSize() const;

        [[nodiscard]] const math::Vec2 &getDpiScale() const;
    };
}
