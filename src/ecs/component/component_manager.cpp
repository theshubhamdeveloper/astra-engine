#include "ecs/component/component_manager.hpp"

namespace astra::ecs::component {
void ComponentManager::removeAll(const entity::Entity &entity) const {
    for (const auto storage : storages) {
        storage->remove(entity);
    }
}
}
