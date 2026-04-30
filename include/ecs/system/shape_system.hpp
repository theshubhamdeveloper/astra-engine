#pragma once
#include "ecs/components/shape.hpp"
#include "ecs/components/transform.hpp"
#include "render/renderer.hpp"
#include "system.hpp"

namespace astra::ecs::system {

struct DrawItem {
    component::Transform transform;
    component::Shape shape;
};

class ShapeSystem : public System {
    render::Renderer& renderer;

  public:
    explicit ShapeSystem(component::ComponentManager& componentManager, render::Renderer& renderer);

  private:
    void update(double deltaTime) override;
};
}
