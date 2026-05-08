#include "ecs/component/component_manager.hpp"

namespace astra::ecs::component {
    void ComponentManager::removeAll(const entity::EntityId &entityId) const {
        for (const auto storage: storages) {
            storage->remove(entityId);
        }
    }
}