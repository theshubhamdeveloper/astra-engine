#pragma once
#include "keyboard.hpp"
#include "mouse.hpp"

namespace astra::input {
    struct Input {
        Keyboard keyboard;
        Mouse mouse;

        Input();

        void updateState();

        void updateCurrentToPrevious();

        bool quitRequested() const;

    private:
        bool quitRequested_;
    };
}
