#include <algorithm>
#include <ranges>
#include <vector>

#include <astra/ecs/systems/render_system.hpp>
#include <astra/ecs/component_storage.hpp>
#include <astra/ecs/components/camera.hpp>
#include <astra/ecs/components/shape.hpp>
#include <astra/ecs/components/transform.hpp>
#include <astra/ecs/entity.hpp>
#include <astra/ecs/systems/camera_system.hpp>
#include <astra/graphics/render_dispatch.hpp>

namespace astra::ecs::systems {
    RenderSystem::RenderSystem(ComponentManager &componentManager, graphics::Renderer &renderer)
        : System(componentManager), renderer(renderer) {
    }

    void RenderSystem::update(double deltaTime) {
        const auto transformStorage = componentManager.getStorage<components::Transform>();
        const auto shapeStorage = componentManager.getStorage<components::Shape>();

        if (!transformStorage || !shapeStorage)
            return;

        std::vector<DrawItem> drawItems;
        drawItems.reserve(shapeStorage->size());

        for (size_t shapeIndex = 0; shapeIndex < shapeStorage->size(); shapeIndex++) {
            const EntityId entityId = shapeStorage->getEntityIdAt(shapeIndex);

            if (!transformStorage->hasComponent(entityId))
                continue;

            components::Shape *shape = &shapeStorage->getComponentAt(shapeIndex);

            if (!shape->style.display)
                continue;

            drawItems.emplace_back(&transformStorage->getComponent(entityId), shape);
        };

        std::ranges::stable_sort(drawItems,
                                 [](const DrawItem &a, const DrawItem &b) {
                                     return a.transform->zindex < b.transform->zindex;
                                 });

        for (auto &drawItem: drawItems) {
            std::visit(
                [this, &drawItem](auto &&shapeGeometry) {
                    graphics::dispatch::shape(renderer, *drawItem.transform, shapeGeometry, drawItem.shape->style);
                },
                drawItem.shape->geometry);
        }
    }
}
