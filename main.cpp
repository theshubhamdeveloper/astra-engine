#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "core/time.hpp"
#include "ecs/world/world.hpp"
#include "input/input.hpp"
#include "platform/window.hpp"
#include "render/renderer.hpp"
#include "assets/image_handler.hpp"
#include "assets/texture.hpp"
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

    auto renderer = render::Renderer(window.getWindowSizeInPixels(), window.getDpiScale());

    world.initialize(renderer, input, window.getWindowSize());

    const auto texture = assets::Texture::fromImage(assets::image_handler::load("../Resources/assets/texture.png"));

    bool running = true;

    while (running) {
        time.update();

        input.updateState();

        std::cout << "Frame: " << time.deltaTime() * 1000 << " ms" << "\n"
                << "FPS: " << time.fps() << "\n";

        if (input.quitRequested())
            running = false;

        if (input.windowResizeRequested()) {
            window.updateOnResize();
            renderer.onWindowResize(window.getWindowSizeInPixels(), window.getDpiScale());
            world.setViewportSize(window.getWindowSize());
        }

        renderer.clear(math::Color::black());

        world.update(time.deltaTime());

        renderer.drawTriangle({{432, 296}, {0, 0}, {255, 255, 255}},
                              {{1168, 296}, {1, 0}, {255, 255, 255}},
                              {{1168, 905}, {1, 1}, {255, 255, 255}}, texture);
        renderer.drawTriangle({{432, 296}, {0, 0}, {255, 255, 255}},
                              {{1168, 905}, {1, 1}, {255, 255, 255}},
                              {{432, 905}, {0, 1}, {255, 255, 255}}, texture);

        window.render(renderer.getBuffer());

        input.updateCurrentToPrevious();
    }

    window.destroy();
    return 0;
}
