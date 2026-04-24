#pragma once
#include <cstdint>

#include "math/vec2.hpp"

namespace astra::input {
constexpr int MOUSE_BUTTON_COUNT = 5;

enum class MouseButton { Left, Right, Middle, X1, X2, INVALID };

struct Mouse {
    math::Vec2 position;
    math::Vec2 previousPosition;

    bool buttonsCurrent[MOUSE_BUTTON_COUNT];
    bool buttonsPrevious[MOUSE_BUTTON_COUNT];

    double wheelDelta;

    Mouse();

    bool isMouseDown(MouseButton button) const;

    bool isMousePressed(MouseButton button) const;

    bool isMouseReleased(MouseButton button) const;

    static MouseButton convertSdlToMouseButton(uint8_t sdlButton);

    math::Vec2 getMousePosition() const;

    math::Vec2 getMouseDelta() const;

    double getScrollDelta() const;
};
}
