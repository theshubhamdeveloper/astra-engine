#pragma once

#include <astra/ecs/component_manager.hpp>

namespace astra::ecs {
    class System {
    protected:
        ComponentManager &componentManager;

    public:
        bool isEnabled;

        explicit System(ComponentManager &componentManager)
            : componentManager(componentManager), isEnabled(true) {
        }

        virtual void update(double deltaTime) = 0;

        virtual ~System() = default;
    };
}
