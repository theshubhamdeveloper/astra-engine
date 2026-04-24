#pragma once
#include "ecs/entity/entity.hpp"

namespace astra::ecs::component {
class IComponentStorage {
  public:
    virtual ~IComponentStorage() = default;

  private:
    friend class ComponentManager;
    virtual void remove(const entity::Entity &entity) = 0;
};
}
