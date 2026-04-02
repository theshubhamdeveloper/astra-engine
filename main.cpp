#include <iostream>
#include "core/time.hpp"
#include "input/input.hpp"
#include "platform/window.hpp"
#include "render/draw.hpp"
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
    auto time = astra::core::Time();
    auto input = astra::input::Input();

    window.initialize();

    bool toDrawCircle = false;
    unsigned circleRadius = 100;

    bool running = true;
    while (running) {
        time.update();

        input.updateState();

        std::cout << "Frame: " << time.deltaTime() * 1000 << " ms" << "\n";
        std::cout << "FPS: " << time.fps() << "\n";

        if (input.quitRequested()) {
            running = false;
        }

        framebuffer.clear(astra::math::Color::red());

        // TEST START
        if (input.mouse.IsMousePressed(astra::input::MouseButton::Right)) {
            toDrawCircle = true;
        }

        circleRadius += input.mouse.wheelDelta;

        if (toDrawCircle)
            drawCircle(framebuffer, input.mouse.position, circleRadius, astra::math::Color::green());
        // TEST END

        window.render(framebuffer.getBuffer());

        input.updateCurrentToPrevious();
    }
    window.destroy();

    return 0;
}
