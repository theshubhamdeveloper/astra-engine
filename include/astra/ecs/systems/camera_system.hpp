#pragma once

#include <astra/ecs/components/camera.hpp>
#include <astra/ecs/system.hpp>
#include <astra/input/input.hpp>
#include <astra/math/vec2.hpp>

namespace astra::ecs::systems {
    class CameraSystem : public System {
        const input::Input &input;
        components::Camera &camera;

    public:
        CameraSystem(ComponentManager &componentManager, components::Camera &camera,
                     const input::Input &input);

        void update(double deltaTime) override;

        static inline math::Vec2 worldToScreen(const components::Camera &camera, const math::Vec2 &worldPosition) {
            return ((worldPosition - camera.position) * camera.zoom) + (camera.viewport.size * 0.5f);
        }

        static inline math::Vec2 screenToWorld(const components::Camera &camera, const math::Vec2 &screenPosition) {
            return ((screenPosition - (camera.viewport.size * 0.5f)) / camera.zoom) + camera.position;
        }
    };
}
