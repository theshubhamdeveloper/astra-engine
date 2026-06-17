#include <SDL3/SDL.h>
#include <freetype/freetype.h>

#include <astra/core/resource_manager.hpp>
#include <astra/core/time.hpp>
#include <astra/ecs/world.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/input/input.hpp>
#include <astra/platform/window.hpp>

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
        {0, 0}, 0, 1,
        graphics::GraphicCamera::orthographic(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0)
    };
    auto renderer = graphics::Renderer(resourceManager, graphicCamera);

    auto world = ecs::World(input);

    window.initialize(false);

    resourceManager.setResourceRootPath("../Resources/");

    renderer.initialize();

    world.initialize(renderer, resourceManager);

    const ecs::components::Camera &worldCamera = world.getCamera();

    bool running = true;

    while (running) {
        time.update();

        input.updateState();

        std::cout << "Frame: " << time.deltaTime() * 1000 << " ms" << "\n"
                << "FPS: " << time.fps() << "\n";

        if (input.quitRequested())
            running = false;

        window.clear(math::Color{14, 26, 37});

        world.update(time.deltaTime());

        //sync graphicCamera and worldCamera
        graphicCamera.position = worldCamera.position;
        graphicCamera.zoom = worldCamera.zoom;

        renderer.flush();

        input.updateCurrentToPrevious();

        window.render();
    }

    window.destroy();
    return 0;
}
