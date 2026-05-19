#include <iostream>
#include <utility>

#include <glad/glad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <astra/platform/window.hpp>
#include <astra/math/point.hpp>
#include <astra/math/color.hpp>

namespace astra::platform {
    Window::Window(std::string title, const math::Point &size)
        : window(nullptr), glContext(), title(std::move(title)), windowSize(size) {
    }

    void Window::initialize(const bool resizable) {
        Uint32 flags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL;
        if (resizable)
            flags |= SDL_WINDOW_RESIZABLE;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        window = SDL_CreateWindow(title.c_str(), windowSize.x, windowSize.y,
                                  flags);

        math::Point windowSizeInPixels;
        SDL_GetWindowSizeInPixels(window, &windowSizeInPixels.x, &windowSizeInPixels.y);

        glContext = SDL_GL_CreateContext(window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            std::abort();
        }

        glViewport(0, 0, windowSizeInPixels.x, windowSizeInPixels.y);
        SDL_GL_SwapWindow(window);
    }


    void Window::destroy() const {
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Window::clear(const math::Color &color) const {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::render() const {
        SDL_GL_SwapWindow(window);
    }

    void Window::updateOnResize() {
    }

    const math::Point &Window::getWindowSize() const {
        return windowSize;
    }

    const math::Vec2 &Window::getDpiScale() const {
        return dpiScale;
    };
}
