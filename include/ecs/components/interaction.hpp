#pragma once

namespace astra::ecs::component {
    struct Interaction {
        bool enabled;

        bool hovered;
        bool entered;
        bool exited;

        bool leftPressed;
        bool leftClicked;
        bool leftReleased;

        bool rightPressed;
        bool rightClicked;
        bool rightReleased;
    };
}