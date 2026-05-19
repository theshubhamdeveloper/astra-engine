#pragma once

#include <astra/input/keyboard.hpp>
#include <astra/input/mouse.hpp>

namespace astra::input {
    struct Input {
        Keyboard keyboard;
        Mouse mouse;

        Input();

        void updateState();

        void updateCurrentToPrevious();

        bool quitRequested() const;

        bool windowResizeRequested() const;

    private:
        bool quitRequested_;
        bool windowResizeRequested_;
    };
}