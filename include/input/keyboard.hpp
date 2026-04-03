#pragma once
#include "SDL3/SDL_scancode.h"

namespace astra::input {
    struct Keyboard {
        bool current[SDL_SCANCODE_COUNT];
        bool previous[SDL_SCANCODE_COUNT];

        Keyboard();

        bool isKeyDown(SDL_Scancode key) const;

        bool isKeyPressed(SDL_Scancode key) const;

        bool isKeyReleased(SDL_Scancode key) const;
    };
}
