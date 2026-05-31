#pragma once

#include <astra/ecs/component_manager.hpp>
#include <astra/ecs/entity_manager.hpp>
#include <astra/ecs/system_manager.hpp>
#include <astra/ecs/components/camera.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/input/input.hpp>

namespace astra::ecs {
    class World {
        EntityManager entityManager;
        ComponentManager componentManager;
        SystemManager systemManager;
        components::Camera camera;

    public:
        void initialize(graphics::Renderer &renderer, const input::Input &input,
                        graphics::ResourceManager &resourceManager);

        void update(double deltaTime);

        [[nodiscard]] const components::Camera &getCamera() const;
    };
}
