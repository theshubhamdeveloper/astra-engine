#pragma once

#include <astra/ecs/component_manager.hpp>
#include <astra/ecs/components/camera.hpp>
#include <astra/ecs/components/interaction.hpp>
#include <astra/ecs/components/shape.hpp>
#include <astra/ecs/components/transform.hpp>
#include <astra/ecs/system.hpp>
#include <astra/input/input.hpp>
#include <astra/math/vec2.hpp>

namespace astra::ecs::systems {
    struct HitItem {
        int zindex;
        components::Interaction *interaction;
    };

    class InteractionSystem : public System {
        const input::Input &input;
        const components::Camera &camera;
        components::Interaction *lastInteraction;

    public:
        InteractionSystem(ComponentManager &componentManager, const components::Camera &camera,
                          const input::Input &input);

        void update(double deltaTime) override;

    private:
        components::Interaction *hitTestInteraction();

        static bool hitTest(const components::Transform &transform,
                            const components::RectGeometry &rectangleGeometry,
                            const components::Interaction &interaction, const math::Vec2 &position);

        static bool hitTest(const components::Transform &transform, const components::CircleGeometry &circleGeometry,
                            const components::Interaction &interaction, const math::Vec2 &position);

        static bool hitTest(const components::Transform &transform,
                            const components::TriangleGeometry &triangleGeometry,
                            const components::Interaction &interaction, const math::Vec2 &position);

        static bool hitTest(const components::Transform &transform, const components::LineGeometry &lineGeometry,
                            const components::Interaction &interaction, const math::Vec2 &position);
    };
}
