#include <SDL3/SDL.h>
#include <freetype/freetype.h>

#include <astra/core/resource_manager.hpp>
#include <astra/core/time.hpp>
#include <astra/ecs/world.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/input/input.hpp>
#include <astra/platform/window.hpp>

#include "astra/core/atlas_builder.hpp"

using namespace astra;

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        std::cout << "SDL could not initialize! SDL error: \n" << SDL_GetError() << std::endl;
        return 3;
    }

    FT_Library library;

    if (FT_Init_FreeType(&library) != FT_Err_Ok) {
        std::cout << "Could not initialize FreeType library! \n" << std::endl;
        return 3;
    };

    auto window = platform::Window("Astra Engine", {SCREEN_WIDTH, SCREEN_HEIGHT});

    auto resourceManager = core::ResourceManager();

    auto time = core::Time();
    auto input = input::Input();

    auto graphicCamera = graphics::GraphicCamera{
        {},
        0,
        1,
    };

    auto renderer = graphics::Renderer(resourceManager, graphicCamera);

    auto world = ecs::World(input);

    window.initialize(false);

    graphicCamera.projection =
            graphics::GraphicCamera::orthographic(0, window.sizeInPixels().x, window.sizeInPixels().y, 0);

    renderer.initialize(math::vec2{window.sizeInPixels()} / math::vec2{window.size()});

    world.initialize(renderer, resourceManager);

    const ecs::components::Camera &worldCamera = world.getCamera();

    const auto jetbrainsFont = resourceManager.fonts.load({
        &resourceManager, library, "../Resources/fonts/jetbrains-mono/static/JetBrainsMono-Regular.ttf", 0
    });

    bool running = true;

    while (running) {
        time.update();
        input.updateState();

        if (input.quitRequested())
            running = false;

        window.clear(math::Color{14, 26, 37});

        renderer.begin();

        world.update(time.deltaTime());

        //sync graphicCamera and worldCamera
        graphicCamera.position.x = floor(worldCamera.position.x);
        graphicCamera.position.y = floor(worldCamera.position.y);
        graphicCamera.zoom = worldCamera.zoom;


        renderer.drawRect({
            .position = {0, 0},
            .size = {200, 200},
            .rotation = 0,
            .style = {
                .fill = {128, 255, 124},
                .strokeWidth = 1,
            }
        });

        renderer.drawText({
            .position = {0, 0},
            .font = jetbrainsFont,
            .text = std::format("Frame: {} ms\nFPS: {}\nDraw Calls: {}", time.deltaTime() * 1000, time.fps(),
                                renderer.drawCallCount()),
            .color = {255, 210, 129},
            .size = 18
        });


        renderer.end();

        input.updateCurrentToPrevious();
        window.render();
    }

    window.destroy();
    return 0;
}
