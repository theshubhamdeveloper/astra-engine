#pragma once

#include <astra/ecs/components/camera.hpp>
#include <astra/ecs/component_manager.hpp>
#include <astra/ecs/entity_manager.hpp>
#include <astra/ecs/system_manager.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/input/input.hpp>
#include <astra/math/point.hpp>

namespace astra::ecs {
    class World {
        EntityManager entityManager;
        ComponentManager componentManager;
        SystemManager systemManager;
        components::Camera camera;

    public:
        void initialize(graphics::Renderer &renderer, input::Input &input, const math::Point &viewportSize);

        void update(double deltaTime);

        void setViewportSize(const math::Point &size);
    };
}
