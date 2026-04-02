#include <iostream>
#include <ranges>
#include "input/input.hpp"
#include "SDL3/SDL_events.h"

namespace astra::input {
    Input::Input() : quitRequested_(false) {
    }

    void Input::updateState() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            int buttonIndex;
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quitRequested_ = true;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    keyboard.current[event.key.scancode] = true;
                    break;

                case SDL_EVENT_KEY_UP:
                    keyboard.current[event.key.scancode] = false;
                    break;

                case SDL_EVENT_MOUSE_MOTION:
                    mouse.position.x = event.motion.x;
                    mouse.position.y = event.motion.y;
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    buttonIndex = static_cast<int>(Mouse::ConvertSdlToMouseButton(event.button.button));
                    if (buttonIndex == -1) {
                        std::cout << "Invalid mouse button index" << std::endl;
                        break;
                    }

                    mouse.buttonsCurrent[buttonIndex] = true;
                    break;

                case SDL_EVENT_MOUSE_BUTTON_UP:
                    buttonIndex = static_cast<int>(Mouse::ConvertSdlToMouseButton(event.button.button));
                    if (buttonIndex == -1) {
                        std::cout << "Invalid mouse button index" << std::endl;
                        break;
                    }

                    mouse.buttonsCurrent[buttonIndex] = false;
                    break;

                case SDL_EVENT_MOUSE_WHEEL:
                    mouse.wheelDelta = event.wheel.y;
                    break;

                default:
                    std::cout << "Unhandled event type: " << event.type << "\n";
                    break;
            }
        }
    }

    void Input::updateCurrentToPrevious() {
        std::ranges::copy(keyboard.current, keyboard.previous);
        std::ranges::copy(mouse.buttonsCurrent, mouse.buttonsPrevious);
        mouse.wheelDelta = 0;
    }

    bool Input::quitRequested() const {
        return quitRequested_;
    }
}
