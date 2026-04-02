#include "input/keyboard.hpp"

namespace astra::input {
    Keyboard::Keyboard() : current{false}, previous{false} {
    }

    bool Keyboard::IsKeyDown(const SDL_Scancode key) const {
        return current[key];
    }

    bool Keyboard::IsKeyPressed(const SDL_Scancode key) const {
        return !previous[key] && current[key];
    }

    bool Keyboard::IsKeyReleased(SDL_Scancode key) const {
        return previous[key] && !current[key];
    }
}
