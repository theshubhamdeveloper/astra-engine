#include "ecs/system/shape_system.hpp"
#include "ecs/component/component_storage.hpp"
#include "ecs/components/shape.hpp"
#include "ecs/entity/entity.hpp"
#include "render/render_dispatch.hpp"
#include <algorithm>
#include <vector>

namespace astra::ecs::system {
ShapeSystem::ShapeSystem(component::ComponentManager& componentManager, render::Renderer& renderer)
    : System(componentManager), renderer(renderer) {}

void ShapeSystem::update(double deltaTime) {
    const auto transformStorage = componentManager.getStorage<component::Transform>();
    const auto shapeStorage = componentManager.getStorage<component::Shape>();

    if (transformStorage == nullptr || shapeStorage == nullptr)
        return;

    std::vector<DrawItem> drawItems;
    drawItems.reserve(shapeStorage->size());

    for (size_t shapeIndex = 0; shapeIndex < shapeStorage->size(); shapeIndex++) {
        const entity::EntityId entityId = shapeStorage->getEntityIdAt(shapeIndex);

        if (!transformStorage->hasComponent(entityId))
            continue;

        component::Shape shape = shapeStorage->getComponentAt(shapeIndex);

        if (!shape.style.display)
            continue;

        drawItems.emplace_back(transformStorage->getComponent(entityId), shape);
    };

    std::stable_sort(drawItems.begin(), drawItems.end(),
                     [](const DrawItem& a, const DrawItem& b) { return a.transform.zindex < b.transform.zindex; });

    for (auto& drawItem : drawItems) {
        std::visit(
            [this, &drawItem](auto&& shapeGeometry) {
                render::dispatch::shape(renderer, drawItem.transform, shapeGeometry, drawItem.shape.style);
            },
            drawItem.shape.geometry);
    }
}
}
