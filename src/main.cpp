#include <SDL3/SDL.h>

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
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return 3;
    }

    auto window = platform::Window("Astra Engine", {SCREEN_WIDTH, SCREEN_HEIGHT});

    auto resourceManager = graphics::ResourceManager();

    auto time = core::Time();
    auto input = input::Input();

    auto graphicCamera = graphics::GraphicCamera{
        {0, 0}, 0, 1,
        graphics::GraphicCamera::orthographic(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0)
    };
    auto renderer = graphics::Renderer(resourceManager, graphicCamera);

    auto world = ecs::World();

    window.initialize(false);

    resourceManager.setResourceRootPath("../Resources/assets/");

    renderer.initialize();

    world.initialize(renderer, input);


    const ecs::components::Camera &worldCamera = world.getCamera();

    const graphics::TextureHandel lufiTex = resourceManager.loadTexture("images/texture.png");

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

        renderer.drawRect({1000, 1000}, {200, 200}, 0, {255, 0, 0});
        renderer.drawRect({500, 500}, {200, 200}, 0, lufiTex);
        renderer.drawRect({900, 500}, {200, 200}, 0, lufiTex);
        renderer.drawRect({500, 1000}, {200, 200}, 0, lufiTex);

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
