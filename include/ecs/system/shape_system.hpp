#pragma once
#include "ecs/components/camera.hpp"
#include "ecs/components/shape.hpp"
#include "ecs/components/transform.hpp"
#include "render/renderer.hpp"
#include "system.hpp"

namespace astra::ecs::system {
    struct DrawItem {
        component::Transform *transform;
        component::Shape *shape;
    };

    class ShapeSystem : public System {
        render::Renderer &renderer;
        const component::Camera &camera;

    public:
        explicit ShapeSystem(component::ComponentManager &componentManager, const component::Camera &camera,
                             render::Renderer &renderer);

    private:
        void update(double deltaTime) override;
    };
}