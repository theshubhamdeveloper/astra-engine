#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "core/time.hpp"
#include "input/input.hpp"
#include "platform/window.hpp"
#include "render/renderer.hpp"
#include <iostream>
using namespace astra;

constexpr int SCREEN_HEIGHT = 500;
constexpr int SCREEN_WIDTH = 700;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return 3;
    }

    auto window = platform::Window("Astra Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    auto renderer = render::Renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    auto time = core::Time();
    auto input = input::Input();

    bool running = true;

    while (running) {
        time.update();

        input.updateState();

        std::cout << "Frame: " << time.deltaTime() * 1000 << " ms" << "\n"
                  << "FPS: " << time.fps() << "\n";

        if (input.quitRequested())
            running = false;

        renderer.clear(math::Color::red());

        window.render(renderer.getBuffer());

        input.updateCurrentToPrevious();
    }

    window.destroy();
    return 0;
}
