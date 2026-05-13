#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "core/time.hpp"
#include "ecs/world/world.hpp"
#include "input/input.hpp"
#include "platform/window.hpp"
#include "render/renderer.hpp"
using namespace astra;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return 3;
    }

    auto window = platform::Window("Astra Engine", {SCREEN_WIDTH, SCREEN_HEIGHT});
    auto time = core::Time();
    auto input = input::Input();
    auto world = ecs::world::World();

    window.initialize(false);

    //
    // auto renderer = render::Renderer();
    //
    // world.initialize(renderer, input, window.getWindowSize());

    bool running = true;

    while (running) {
        time.update();

        input.updateState();

        std::cout << "Frame: " << time.deltaTime() * 1000 << " ms" << "\n"
                << "FPS: " << time.fps() << "\n";

        if (input.quitRequested())
            running = false;

        window.clear({255, 255, 255});

        // world.update(time.deltaTime());

        input.updateCurrentToPrevious();

        window.render();
    }

    window.destroy();
    return 0;
}
