#include <astra/ecs/world.hpp>
#include <astra/ecs/systems/camera_system.hpp>
#include <astra/ecs/systems/interaction_system.hpp>
#include <astra/ecs/systems/render_system.hpp>

namespace astra::ecs {
    World::World(const input::Input &input) : input(input) {
    }

    void World::initialize(graphics::Renderer &renderer,
                           graphics::ResourceManager &resourceManager) {
        camera.zoomSpeed = 0.2f;
        camera.moveSpeed = 1.5f;
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
