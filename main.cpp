#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "glad/glad.h"
#include "core/time.hpp"
#include "ecs/world/world.hpp"
#include "input/input.hpp"
#include "platform/window.hpp"
#include "render/renderer.hpp"
using namespace astra;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

auto vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos.x , aPos.y, 0.0, 1.0);
}
)";

auto fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

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

    float vertices[] =
    {
        0.0f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        nullptr
    );
    glEnableVertexAttribArray(0);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vsSource = vertexShaderSource;
    glShaderSource(vertexShader, 1, &vsSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsSource = fragmentShaderSource;
    glShaderSource(fragmentShader, 1, &fsSource, nullptr);
    glCompileShader(fragmentShader);


    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

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

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // world.update(time.deltaTime());

        input.updateCurrentToPrevious();

        window.render();
    }

    window.destroy();
    return 0;
}
