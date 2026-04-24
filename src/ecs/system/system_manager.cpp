#include "ecs/system/system_manager.hpp"

namespace astra::ecs::system {
void SystemManager::update(const double deltaTime) const {
    for (const auto system : systems) {
        if (system->isEnabled)
            system->update(deltaTime);
    }
}

void SystemManager::addSystem(System *system) { systems.push_back(system); }
}
