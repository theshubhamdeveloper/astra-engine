#pragma once
#include "SDL3/SDL_scancode.h"

namespace astra::input {
    struct Keyboard {
        bool current[SDL_SCANCODE_COUNT];
        bool previous[SDL_SCANCODE_COUNT];

        Keyboard();

        bool IsKeyDown(SDL_Scancode key) const;

        bool IsKeyPressed(SDL_Scancode key) const;

        bool IsKeyReleased(SDL_Scancode key) const;
    };
}
