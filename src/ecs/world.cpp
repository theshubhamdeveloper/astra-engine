#include <astra/ecs/world.hpp>
#include <astra/ecs/systems/camera_system.hpp>
#include <astra/ecs/systems/interaction_system.hpp>
#include <astra/ecs/systems/render_system.hpp>

namespace astra::ecs {
    void World::initialize(graphics::Renderer &renderer, const input::Input &input) {
        camera = {{0, 0}, 1};
        systemManager.addSystem(new systems::RenderSystem(componentManager, renderer));
        systemManager.addSystem(new systems::InteractionSystem(componentManager, camera, input));
        systemManager.addSystem(new systems::CameraSystem(componentManager, camera, input));
    }

    void World::update(const double deltaTime) {
        systemManager.update(deltaTime);
    }

    const components::Camera &World::getCamera() const {
        return camera;
    }
}
