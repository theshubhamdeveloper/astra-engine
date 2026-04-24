#pragma once
#include "ecs/component/component_manager.hpp"

namespace astra::ecs::system {
class System {
  protected:
    component::ComponentManager &componentManager;

  public:
    bool isEnabled;

    explicit System(component::ComponentManager &componentManager)
        : componentManager(componentManager), isEnabled(true) {}

    virtual void update(double deltaTime) const = 0;

    virtual ~System() = default;
};
}
