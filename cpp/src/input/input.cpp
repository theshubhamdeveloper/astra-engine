#include <ranges>

#include <SDL3/SDL_events.h>
#include <astra/input/input.hpp>

#include "astra/platform/window.hpp"

namespace astra::input {
    Input::Input(SDL_Window *window) : m_window(window),
                                       m_textInput(false) {
    }

    void Input::updateState() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            int buttonIndex;
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    m_onQuit.emit();
                    break;

                case SDL_EVENT_WINDOW_RESIZED:
                case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                    m_onResize.emit();
                    break;

                case SDL_EVENT_TEXT_INPUT:
                    m_onText.emit(event.text.text);
                    break;

                case SDL_EVENT_KEY_DOWN:
                    keyboard.current[event.key.scancode] = true;
                    keyboard.m_onKeyDown.emit(event.key.scancode, event.key.repeat);
                    break;

                case SDL_EVENT_KEY_UP:
                    keyboard.current[event.key.scancode] = false;
                    break;

                case SDL_EVENT_MOUSE_MOTION:
                    mouse.position.x = event.motion.x;
                    mouse.position.y = event.motion.y;
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    buttonIndex = static_cast<int>(Mouse::convertSdlToMouseButton(event.button.button));
                    if (buttonIndex == -1)
                        break;

                    mouse.buttonsCurrent[buttonIndex] = true;
                    break;

                case SDL_EVENT_MOUSE_BUTTON_UP:
                    buttonIndex = static_cast<int>(Mouse::convertSdlToMouseButton(event.button.button));
                    if (buttonIndex == -1)
                        break;

                    mouse.buttonsCurrent[buttonIndex] = false;
                    break;

                case SDL_EVENT_MOUSE_WHEEL:
                    mouse.wheelDelta += event.wheel.y;
                    break;

                default:
                    break;
            }
        }
    }

    void Input::updateCurrentToPrevious() {
        std::ranges::copy(keyboard.current, keyboard.previous);
        std::ranges::copy(mouse.buttonsCurrent, mouse.buttonsPrevious);
        mouse.wheelDelta = 0;
        mouse.previousPosition = mouse.position;
    }

    void Input::startTextInput() {
        SDL_StartTextInput(m_window);
        m_textInput = true;
    }

    void Input::stopTextInput() {
        SDL_StopTextInput(m_window);
        m_textInput = false;
    }

    bool Input::isTextInput() const {
        return m_textInput;
    }

    core::SignalView<std::string> &Input::onText() {
        return m_onText;
    }

    core::SignalView<> &Input::onQuit() {
        return m_onQuit;
    }

    core::SignalView<> &Input::onResize() {
        return m_onResize;
    }
}
