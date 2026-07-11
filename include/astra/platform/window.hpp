#pragma once

#include <string>

#include <SDL3/SDL.h>
#include <astra/math/vector.hpp>

namespace astra::math {
    struct Color;
}

namespace astra::platform {
    class Window {
        SDL_Window *m_window;
        SDL_GLContext m_glContext;

        std::string m_title;
        math::uvec2 m_size;
        math::uvec2 m_sizeInPixels;

    public:
        Window(std::string title, const math::uvec2 &size);

        void initialize(bool resizable);

        void destroy() const;

        void clear(const math::Color &color) const;

        void render() const;

        void updateOnResize();

        [[nodiscard]] const math::uvec2 &size() const;
    };
}
