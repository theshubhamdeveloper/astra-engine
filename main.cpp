#include "platform/window.hpp"
#include "render/framebuffer.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"

constexpr int SCREEN_HEIGHT = 500;
constexpr int SCREEN_WIDTH = 700;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return 3;
    }

    auto window = astra::platform::Window("Astra Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    auto framebuffer = astra::render::Framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);

    window.initialize();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // TEST
        framebuffer.putPixel(100, 100, astra::math::Color::red());

        window.render(framebuffer.getBuffer());
    }
    window.destroy();

    return 0;
}
