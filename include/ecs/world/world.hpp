#pragma once
#include "ecs/component/component_manager.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/entity/entity_manager.hpp"
#include "ecs/system/system_manager.hpp"
#include "input/input.hpp"
#include "render/renderer.hpp"

namespace astra::ecs::world {
class World {
    entity::EntityManager entityManager;
    component::ComponentManager componentManager;
    system::SystemManager systemManager;
    component::Camera camera;

  public:
    void initialize(render::Renderer& renderer, input::Input& input);

    void update(double deltaTime);
};
}
