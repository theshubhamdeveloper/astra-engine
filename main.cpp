#include "platform/window.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return 3;
    }

    auto window = astra::platform::Window("Astra Engine", 700, 500);
    window.initialize();

    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
    }
    window.destroy();

    return 0;
}
