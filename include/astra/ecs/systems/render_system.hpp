#pragma once

#include <astra/ecs/components/camera.hpp>
#include <astra/ecs/components/shape.hpp>
#include <astra/ecs/components/transform.hpp>
#include <astra/ecs/system.hpp>
#include <astra/graphics/renderer.hpp>

namespace astra::ecs::systems {
    struct DrawItem {
        components::Transform *transform;
        components::Shape *shape;
    };

    class RenderSystem : public System {
        graphics::Renderer &renderer;
        const components::Camera &camera;

    public:
        explicit RenderSystem(ComponentManager &componentManager, const components::Camera &camera,
                              graphics::Renderer &renderer);

    private:
        void update(double deltaTime) override;
    };
}
