#pragma once

#include <SDL3/SDL.h>

#include <astra/core/signal.hpp>

namespace astra::input {
    struct Keyboard {
        core::Signal<SDL_Scancode, bool> m_onKeyDown;
        bool current[SDL_SCANCODE_COUNT];
        bool previous[SDL_SCANCODE_COUNT];

        Keyboard();

        core::SignalView<SDL_Scancode, bool> &onKeyDown();

        bool isKeyDown(SDL_Scancode key) const;

        bool isKeyPressed(SDL_Scancode key) const;

        bool isKeyReleased(SDL_Scancode key) const;
    };
}
