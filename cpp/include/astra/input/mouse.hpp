#pragma once

#include <cstdint>

#include <astra/math/vector.hpp>

namespace astra::input {
    constexpr int MOUSE_BUTTON_COUNT = 5;

    enum class MouseButton { Left, Right, Middle, X1, X2, INVALID };

    struct Mouse {
        math::vec2 position;
        math::vec2 previousPosition;

        bool buttonsCurrent[MOUSE_BUTTON_COUNT];
        bool buttonsPrevious[MOUSE_BUTTON_COUNT];

        float wheelDelta;

        Mouse();

        [[nodiscard]] bool isMouseDown(MouseButton button) const;

        [[nodiscard]] bool isMousePressed(MouseButton button) const;

        [[nodiscard]] bool isMouseReleased(MouseButton button) const;

        static MouseButton convertSdlToMouseButton(uint8_t sdlButton);

        math::vec2 getMousePosition() const;

        math::vec2 getMouseDelta() const;

        double getScrollDelta() const;
    };
}
