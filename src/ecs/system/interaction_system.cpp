#include "ecs/system/interaction_system.hpp"
#include "ecs/system/camera_system.hpp"
#include "input/mouse.hpp"
#include "math/vec2.hpp"

namespace astra::ecs::system {
InteractionSystem::InteractionSystem(component::ComponentManager& componentManager, const component::Camera& camera,
                                     const input::Input& input)
    : System(componentManager), input(input), camera(camera) {}

void InteractionSystem::update(double deltaTime) {
    component::Interaction* currentInteraction = hitTestInteraction();

    const auto interactionStorage = componentManager.getStorage<component::Interaction>();
    if (!interactionStorage)
        return;

    for (size_t interactionIndex = 0; interactionIndex < interactionStorage->size(); interactionIndex++) {
        component::Interaction& interaction = interactionStorage->getComponentAt(interactionIndex);
        interaction.leftPressed = false;
        interaction.leftReleased = false;
        interaction.rightPressed = false;
        interaction.rightReleased = false;
        interaction.exited = false;
        interaction.entered = false;
    }

    if (lastInteraction) {
        if (lastInteraction != currentInteraction) {
            lastInteraction->hovered = false;
            lastInteraction->exited = true;
        }
    }

    if (currentInteraction) {
        if (currentInteraction != lastInteraction)
            currentInteraction->entered = true;
        else
            currentInteraction->entered = false;

        currentInteraction->hovered = true;

        if (input.mouse.isMousePressed(input::MouseButton::Left))
            currentInteraction->leftPressed = true;

        else if (input.mouse.isMouseReleased(input::MouseButton::Left))
            currentInteraction->leftReleased = true;

        else if (input.mouse.isMousePressed(input::MouseButton::Right))
            currentInteraction->rightPressed = true;

        else if (input.mouse.isMouseReleased(input::MouseButton::Right))
            currentInteraction->rightReleased = true;
    }

    lastInteraction = currentInteraction;
}
component::Interaction* InteractionSystem::hitTestInteraction() {
    const auto transformStorage = componentManager.getStorage<component::Transform>();
    const auto shapeStorage = componentManager.getStorage<component::Shape>();
    const auto interactionStorage = componentManager.getStorage<component::Interaction>();

    HitItem topHitItem{0, nullptr};

    if (transformStorage == nullptr || shapeStorage == nullptr || interactionStorage == nullptr)
        return nullptr;

    for (size_t interactionIndex = 0; interactionIndex < interactionStorage->size(); interactionIndex++) {
        const entity::EntityId entityId = interactionStorage->getEntityIdAt(interactionIndex);

        if (!shapeStorage->hasComponent(entityId) || !transformStorage->hasComponent(entityId))
            continue;

        component::Interaction& interaction = interactionStorage->getComponentAt(interactionIndex);
        component::Shape& shape = shapeStorage->getComponent(entityId);
        component::Transform& transform = transformStorage->getComponent(entityId);

        if (!interaction.enabled || !shape.style.display)
            continue;

        std::visit(
            [this, &topHitItem, &shape, &interaction, &transform](auto&& shapeGeometry) {
                component::Transform screenPosTransform = transform;
                screenPosTransform.position = CameraSystem::worldToScreen(camera, transform.position);
                screenPosTransform.scale *= camera.zoom;

                if (hitTest(screenPosTransform, shapeGeometry, interaction, input.mouse.position) &&
                    topHitItem.zindex <= transform.zindex) {
                    topHitItem.interaction = &interaction;
                    topHitItem.zindex = transform.zindex;
                }
            },
            shape.geometry);
    };

    return topHitItem.interaction;
}

// TODO: Add rotation
bool InteractionSystem::hitTest(const component::Transform& transform,
                                const component::RectangleGeometry& rectangleGeometry,
                                const component::Interaction& interaction, const math::Vec2& position) {
    if (position.x > transform.position.x &&
        position.x < transform.position.x + (rectangleGeometry.size.x * transform.scale.x) &&
        position.y > transform.position.y &&
        position.y < transform.position.y + (rectangleGeometry.size.y * transform.scale.y))
        return true;

    return false;
}

bool InteractionSystem::hitTest(const component::Transform& transform, const component::CircleGeometry& circleGeometry,
                                const component::Interaction& interaction, const math::Vec2& position) {
    float radius = circleGeometry.radius * std::max(transform.scale.x, transform.scale.y);
    if (position.distanceSquared(transform.position) <= radius * radius)
        return true;

    return false;
}

bool InteractionSystem::hitTest(const component::Transform& transform, const component::LineGeometry& lineGeometry,
                                const component::Interaction& interaction, const math::Vec2& position) {
    // TODO: HitTest for Line
    return false;
}
}
