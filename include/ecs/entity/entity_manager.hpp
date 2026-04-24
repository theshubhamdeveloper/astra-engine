#pragma once
#include "entity.hpp"
#include <queue>
#include <vector>

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
