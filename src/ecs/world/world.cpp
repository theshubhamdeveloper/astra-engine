#include "ecs/world/world.hpp"
#include "ecs/system/shape_system.hpp"

namespace astra::ecs::world {
void World::initialize(render::Renderer& renderer) {
    systemManager.addSystem(new system::ShapeSystem(componentManager, renderer));
}

void World::update(const double deltaTime) {
    systemManager.update(deltaTime);
}
}
