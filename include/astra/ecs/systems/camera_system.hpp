#pragma once

#include <astra/ecs/components/camera.hpp>
#include <astra/ecs/system.hpp>
#include <astra/input/input.hpp>
#include <astra/math/vector.hpp>

namespace astra::ecs::systems {
    class CameraSystem : public System {
        const input::Input &input;
        components::Camera &camera;

    public:
        CameraSystem(ComponentManager &componentManager, components::Camera &camera,
                     const input::Input &input);

        void update(double deltaTime) override;

        static math::vec2 worldToScreen(const components::Camera &camera, const math::vec2 &worldPosition) {
            return (worldPosition - camera.position) * camera.zoom;
        }

        static math::vec2 screenToWorld(const components::Camera &camera, const math::vec2 &screenPosition) {
            return (screenPosition / camera.zoom) + camera.position;
        }
    };
}
