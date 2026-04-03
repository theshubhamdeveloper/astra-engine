#include "input/keyboard.hpp"

namespace astra::input {
    Keyboard::Keyboard() : current{false}, previous{false} {
    }

    bool Keyboard::isKeyDown(const SDL_Scancode key) const {
        return current[key];
    }

    bool Keyboard::isKeyPressed(const SDL_Scancode key) const {
        return !previous[key] && current[key];
    }

    bool Keyboard::isKeyReleased(SDL_Scancode key) const {
        return previous[key] && !current[key];
    }
}
