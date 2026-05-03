#pragma once
#include "ecs/component/component_manager.hpp"
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
    input::Input& input;

  public:
    InteractionSystem(component::ComponentManager& componentManager, input::Input& input);
    void update(double deltaTime) override;

  private:
    component::Interaction* getInteractedComponent();

    static bool hitTest(const component::Transform& transform, const component::RectangleGeometry& rectangleGeometry,
                        const component::Interaction& interaction, const math::Vec2& position);
    static bool hitTest(const component::Transform& transform, const component::CircleGeometry& circleGeometry,
                        const component::Interaction& interaction, const math::Vec2& position);
    static bool hitTest(const component::Transform& transform, const component::LineGeometry& lineGeometry,
                        const component::Interaction& interaction, const math::Vec2& position);
};
}
