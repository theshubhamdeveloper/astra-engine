#pragma once

#include <string>
#include <astra/input/keyboard.hpp>
#include <astra/input/mouse.hpp>
#include <SDL3/SDL.h>

#include <astra/core/signal.hpp>

namespace astra::input {
    struct Input {
        Keyboard keyboard;
        Mouse mouse;

        explicit Input(SDL_Window *window);

        void updateState();

        void updateCurrentToPrevious();

        void startTextInput();

        void stopTextInput();

        core::SignalView<std::string> &onText();

        bool isTextInput() const;

        core::SignalView<> &onQuit();

        core::SignalView<> &onResize();

    private:
        SDL_Window *m_window;
        core::Signal<> m_onQuit;
        core::Signal<> m_onResize;
        bool m_textInput;
        core::Signal<std::string> m_onText;
    };
}
