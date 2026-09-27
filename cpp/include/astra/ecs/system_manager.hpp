#pragma once

#include <vector>

#include <astra/ecs/system.hpp>

namespace astra::ecs {
    class SystemManager {
        std::vector<System *> systems;

    public:
        void update(const double deltaTime) const {
            for (const auto system: systems) {
                if (system->isEnabled)
                    system->update(deltaTime);
            }
        }

        void addSystem(System *system) {
            systems.push_back(system);
        }
    };
}
