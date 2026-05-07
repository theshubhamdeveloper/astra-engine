#pragma once
#include "ecs/components/camera.hpp"
#include "ecs/system/system.hpp"
#include "input/input.hpp"
#include "math/vec2.hpp"

namespace astra::ecs::system {
class CameraSystem : public System {
    const input::Input& input;
    component::Camera& camera;

  public:
    CameraSystem(component::ComponentManager& componentManager, component::Camera& camera, const input::Input& input);
    void update(double deltaTime) override;

    static inline math::Vec2 worldToScreen(const component::Camera& camera, const math::Vec2& worldPosition) {
        return ((worldPosition - camera.position) * camera.zoom) + (camera.viewport.size * 0.5f);
    }

    static inline math::Vec2 screenToWorld(const component::Camera& camera, const math::Vec2& screenPosition) {
        return ((screenPosition - (camera.viewport.size * 0.5f)) / camera.zoom) + camera.position;
    }
};
}
