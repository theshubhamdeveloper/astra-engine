#include "ecs/world/world.hpp"
#include "ecs/system/camera_system.hpp"
#include "ecs/system/interaction_system.hpp"
#include "ecs/system/shape_system.hpp"
#include "math/point.hpp"

namespace astra::ecs::world {
void World::initialize(render::Renderer& renderer, input::Input& input, const math::Point& viewportSize) {
    camera = {{0, 0}, 1, {{0, 0}, viewportSize.toVec2()}};
    systemManager.addSystem(new system::ShapeSystem(componentManager, camera, renderer));
    systemManager.addSystem(new system::InteractionSystem(componentManager, camera, input));
    systemManager.addSystem(new system::CameraSystem(componentManager, camera, input));
}

void World::update(const double deltaTime) {
    systemManager.update(deltaTime);
}

void World::setViewportSize(const math::Point& size) {
    camera.viewport.size = size.toVec2();
}
}
