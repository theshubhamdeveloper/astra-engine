#include "ecs/entity/entity_manager.hpp"

namespace astra::ecs::entity {
    EntityManager::EntityManager() : nextEntityId(0) {
    }

    Entity EntityManager::createEntity() {
        if (!freeIds.empty()) {
            const EntityId id = freeIds.front();
            const Entity entity = {id, generations[id]};
            freeIds.pop();
            return entity;
        }

        const EntityId id = nextEntityId++;
        generations.push_back(1);

        return {id, 1};
    }

    void EntityManager::destroyEntity(const Entity &entity) {
        if (!isAlive(entity))
            return;

        freeIds.push(entity.id);
        ++generations[entity.id];
    }

    bool EntityManager::isAlive(const Entity &entity) const {
        if (entity.id >= nextEntityId)
            return false;

        return generations[entity.id] == entity.generation;
    }
}
