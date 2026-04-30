#pragma once
#include "ecs/component/component_manager.hpp"
#include "ecs/entity/entity_manager.hpp"
#include "ecs/system/system_manager.hpp"
#include "render/renderer.hpp"

namespace astra::ecs::world {
class World {
    entity::EntityManager entityManager;
    component::ComponentManager componentManager;
    system::SystemManager systemManager;

  public:
    void initialize(render::Renderer& renderer);

    void update(double deltaTime);
};
}
