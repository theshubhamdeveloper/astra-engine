#pragma once
#include "ecs/component/component_manager.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/interaction.hpp"
#include "ecs/components/shape.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/system/system.hpp"
#include "input/input.hpp"
#include "math/vec2.hpp"

namespace astra::ecs::system {

struct HitItem {
    int zindex;
    component::Interaction* interaction;
};

class InteractionSystem : public System {
    const input::Input& input;
    const component::Camera& camera;
    component::Interaction* lastInteraction;

  public:
    InteractionSystem(component::ComponentManager& componentManager, const component::Camera& camera,
                      const input::Input& input);
    void update(double deltaTime) override;

  private:
    component::Interaction* hitTestInteraction();

    static bool hitTest(const component::Transform& transform, const component::RectangleGeometry& rectangleGeometry,
                        const component::Interaction& interaction, const math::Vec2& position);
    static bool hitTest(const component::Transform& transform, const component::CircleGeometry& circleGeometry,
                        const component::Interaction& interaction, const math::Vec2& position);
    static bool hitTest(const component::Transform& transform, const component::LineGeometry& lineGeometry,
                        const component::Interaction& interaction, const math::Vec2& position);
};
}
