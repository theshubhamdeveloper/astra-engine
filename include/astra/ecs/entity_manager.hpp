#pragma once

#include <queue>
#include <vector>

#include <astra/ecs/entity.hpp>

namespace astra::ecs {
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
