#pragma once
#include <vector>
#include "system.hpp"

namespace astra::ecs::system {
    class SystemManager {
        std::vector<System *> systems;

    public:
        void update(double deltaTime) const;

        void addSystem(System *system);
    };
}
