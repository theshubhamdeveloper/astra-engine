#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <astra/assets/image.hpp>
#include <astra/core/time.hpp>
#include <astra/ecs/world.hpp>
#include <astra/graphics/shader.hpp>
#include <astra/graphics/mesh.hpp>
#include <astra/input/input.hpp>
#include <astra/platform/window.hpp>

using namespace astra;

constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 700;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return 3;
    }

    auto window = platform::Window("Astra Engine", {SCREEN_WIDTH, SCREEN_HEIGHT});
    auto time = core::Time();
    auto input = input::Input();
    // auto world = ecs::world::World();

    window.initialize(false);

    const std::vector<math::Vertex> vertices = {
        {{-0.5f, 1.0f,}, {0, 1}, math::Color::red()},
        {{0.5f, 1.0f,}, {1, 1}, math::Color::red()},
        {{-0.5f, -1.0f,}, {0, 0}, math::Color::red()},
        {{0.5f, -1.0f,}, {1, 0}, math::Color::red()},
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2,
        1, 2, 3
    };

    const auto shader = graphics::Shader("../Resources/assets/shaders/texture.vert",
                                         "../Resources/assets/shaders/texture.frag");

    auto mesh = graphics::Mesh(vertices, indices);

    mesh.addAttribute(0,
                      2,
                      GL_FLOAT,
                      GL_FALSE,
                      sizeof(math::Vertex),
                      nullptr);

    // mesh.addAttribute(1,
    //                   4,
    //                   GL_UNSIGNED_BYTE,
    //                   GL_TRUE,
    //                   sizeof(math::Vertex),                                         
    //                   reinterpret_cast<void *>(offsetof(math::Vertex, color)));

    mesh.addAttribute(1,
                      2,
                      GL_FLOAT,
                      GL_FALSE,
                      sizeof(math::Vertex),
                      reinterpret_cast<void *>(offsetof(math::Vertex, uv)));

    mesh.unbind();

    const graphics::Texture texture{assets::Image::load("../Resources/assets/images/texture.png"), 0};
    texture.bindToUniform(shader, "tex0");


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

        window.clear(math::Color::white());

        shader.use();
        texture.use();
        mesh.draw();

        // world.update(time.deltaTime());

        input.updateCurrentToPrevious();

        window.render();
    }

    window.destroy();
    return 0;
}
