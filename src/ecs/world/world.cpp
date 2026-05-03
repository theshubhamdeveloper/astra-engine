#include "ecs/world/world.hpp"
#include "ecs/system/interaction_system.hpp"
#include "ecs/system/shape_system.hpp"

namespace astra::ecs::world {
void World::initialize(render::Renderer& renderer, input::Input& input) {
    systemManager.addSystem(new system::ShapeSystem(componentManager, renderer));
    systemManager.addSystem(new system::InteractionSystem(componentManager, input));
}

void World::update(const double deltaTime) {
    systemManager.update(deltaTime);
}
}
