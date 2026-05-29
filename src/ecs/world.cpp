#include <astra/ecs/systems/camera_system.hpp>
#include <astra/ecs/systems/interaction_system.hpp>
#include <astra/ecs/systems/render_system.hpp>
#include <astra/ecs/world.hpp>

namespace astra::ecs {
    void World::initialize(graphics::Renderer &renderer, input::Input &input, const math::Vec2 &viewportSize) {
        camera = {{0, 0}, 1, {{0, 0}, viewportSize}};
        systemManager.addSystem(new systems::RenderSystem(componentManager, camera, renderer));
        systemManager.addSystem(new systems::InteractionSystem(componentManager, camera, input));
        systemManager.addSystem(new systems::CameraSystem(componentManager, camera, input));
    }

    void World::update(const double deltaTime) {
        systemManager.update(deltaTime);
    }

    void World::setViewportSize(const math::Vec2 &size) {
        camera.viewport.size = size;
    }
}
