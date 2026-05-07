#pragma once
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "render/framebuffer.hpp"
#include <cstdint>
#include <string>

namespace astra::platform {
class Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    std::string title;
    int32_t width, height, pixelW, pixelH;

  public:
    Window(std::string title, int32_t width, int32_t height);

    void initialize();

    void destroy() const;

    void render(const render::Buffer& buffer) const;

    int32_t getPixelW() const;
    int32_t getPixelH() const;
};
}
