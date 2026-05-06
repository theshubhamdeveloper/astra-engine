#include "ecs/system/shape_system.hpp"
#include "ecs/component/component_storage.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/shape.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/entity/entity.hpp"
#include "ecs/system/camera_system.hpp"
#include "render/render_dispatch.hpp"
#include <algorithm>
#include <vector>

namespace astra::ecs::system {
ShapeSystem::ShapeSystem(component::ComponentManager& componentManager, const component::Camera& camera,
                         render::Renderer& renderer)
    : System(componentManager), renderer(renderer), camera(camera) {}

void ShapeSystem::update(double deltaTime) {
    const auto transformStorage = componentManager.getStorage<component::Transform>();
    const auto shapeStorage = componentManager.getStorage<component::Shape>();

    if (!transformStorage || !shapeStorage)
        return;

    std::vector<DrawItem> drawItems;
    drawItems.reserve(shapeStorage->size());

    for (size_t shapeIndex = 0; shapeIndex < shapeStorage->size(); shapeIndex++) {
        const entity::EntityId entityId = shapeStorage->getEntityIdAt(shapeIndex);

        if (!transformStorage->hasComponent(entityId))
            continue;

        component::Shape* shape = &shapeStorage->getComponentAt(shapeIndex);

        if (!shape->style.display)
            continue;

        drawItems.emplace_back(&transformStorage->getComponent(entityId), shape);
    };

    std::stable_sort(drawItems.begin(), drawItems.end(),
                     [](const DrawItem& a, const DrawItem& b) { return a.transform->zindex < b.transform->zindex; });

    for (auto& drawItem : drawItems) {
        std::visit(
            [this, &drawItem](auto&& shapeGeometry) {
                component::Transform screenPosTransform = *drawItem.transform;
                screenPosTransform.position = CameraSystem::worldToScreen(camera, drawItem.transform->position);

                render::dispatch::shape(renderer, screenPosTransform, shapeGeometry, drawItem.shape->style);
            },
            drawItem.shape->geometry);
    }
}
}
