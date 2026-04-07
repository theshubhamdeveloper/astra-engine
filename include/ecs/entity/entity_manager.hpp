#pragma once
#include <queue>
#include <vector>
#include "entity.hpp"

namespace astra::ecs::entity {
    class EntityManager {
        EntityId nextEntityId;
        std::queue<EntityId> freeIds;
        std::vector<EntityGeneration> generations;
    public:
        EntityManager();

        Entity createEntity();

        void destroyEntity(const Entity &entity);

        bool isAlive(const Entity &entity) const;
    };
}
