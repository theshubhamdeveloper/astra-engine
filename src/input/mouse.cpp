#include "input/mouse.hpp"

#include "SDL3/SDL_mouse.h"

namespace astra::input {
    Mouse::Mouse() : position(math::Vec2::zero()),
                     previousPosition(math::Vec2::zero()),
                     buttonsCurrent{false},
                     buttonsPrevious{false},
                     wheelDelta(0) {
        float positionX;
        float positionY;
        SDL_GetMouseState(&positionX, &positionY);

        position.x = positionX;
        position.y = positionY;
    }

    bool Mouse::isMouseDown(MouseButton button) const {
        return buttonsCurrent[static_cast<int>(button)];
    }

    bool Mouse::isMousePressed(MouseButton button) const {
        const bool previous = buttonsPrevious[static_cast<int>(button)];
        const bool current = buttonsCurrent[static_cast<int>(button)];

        return !previous && current;
    }

    bool Mouse::isMouseReleased(MouseButton button) const {
        const bool previous = buttonsPrevious[static_cast<int>(button)];
        const bool current = buttonsCurrent[static_cast<int>(button)];

        return previous && !current;
    }

    MouseButton Mouse::convertSdlToMouseButton(const uint8_t sdlButton) {
        switch (sdlButton) {
            case SDL_BUTTON_LEFT:
                return MouseButton::Left;

            case SDL_BUTTON_MIDDLE:
                return MouseButton::Middle;

            case SDL_BUTTON_RIGHT:
                return MouseButton::Right;

            case SDL_BUTTON_X1:
                return MouseButton::X1;

            case SDL_BUTTON_X2:
                return MouseButton::X2;

            default:
                return MouseButton::INVALID;
        }
    }

    math::Vec2 Mouse::getMousePosition() const {
        return position;
    }

    math::Vec2 Mouse::getMouseDelta() const {
        return previousPosition - position;
    }

    double Mouse::getScrollDelta() const {
        return wheelDelta;
    }
}
