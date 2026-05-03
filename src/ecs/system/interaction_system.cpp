#include "ecs/system/interaction_system.hpp"
#include <cmath>
#include <vector>

namespace astra::ecs::system {
InteractionSystem::InteractionSystem(component::ComponentManager& componentManager, input::Input& input)
    : System(componentManager), input(input) {}

void InteractionSystem::update(double deltaTime) {
    component::Interaction* interactedComponent = getInteractedComponent();

    if (interactedComponent != nullptr) {
        interactedComponent->entered = false;
        if (!interactedComponent->hovered) {
            interactedComponent->hovered = true;
            interactedComponent->entered = true;
        }
    }

    const auto interactionStorage = componentManager.getStorage<component::Interaction>();

    if (interactionStorage == nullptr)
        return;

    for (size_t interactionIndex = 0; interactionIndex < interactionStorage->size(); interactionIndex++) {
        component::Interaction* interaction = &interactionStorage->getComponentAt(interactionIndex);

        interaction->exited = false;

        if (interactedComponent != interaction && interaction->hovered) {
            interaction->hovered = false;
            interaction->exited = true;
        }
    }
}
component::Interaction* InteractionSystem::getInteractedComponent() {
    const auto transformStorage = componentManager.getStorage<component::Transform>();
    const auto shapeStorage = componentManager.getStorage<component::Shape>();
    const auto interactionStorage = componentManager.getStorage<component::Interaction>();

    std::vector<HitItem> hitItems;

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
            [this, &hitItems, &shape, &interaction, &transform](auto&& shapeGeometry) {
                if (hitTest(transform, shapeGeometry, interaction, input.mouse.position))
                    hitItems.emplace_back(transform.zindex, &interaction);
            },
            shape.geometry);
    };

    if (hitItems.empty())
        return nullptr;

    std::sort(hitItems.begin(), hitItems.end(), [](const HitItem& a, const HitItem& b) { return a.zindex < b.zindex; });

    return hitItems.back().interaction;
}

// TODO: Add rotation
bool InteractionSystem::hitTest(const component::Transform& transform,
                                const component::RectangleGeometry& rectangleGeometry,
                                const component::Interaction& interaction, const math::Vec2& position) {
    if (position.x > transform.position.x && position.x < transform.position.x + rectangleGeometry.size.x &&
        position.y > transform.position.y && position.y < transform.position.y + rectangleGeometry.size.y)
        return true;

    return false;
}

bool InteractionSystem::hitTest(const component::Transform& transform, const component::CircleGeometry& circleGeometry,
                                const component::Interaction& interaction, const math::Vec2& position) {
    const double distaceFromCenter = std::hypot(transform.position.x - position.x, transform.position.y - position.y);

    if (distaceFromCenter <= circleGeometry.radius)
        return true;

    return false;
}

bool InteractionSystem::hitTest(const component::Transform& transform, const component::LineGeometry& lineGeometry,
                                const component::Interaction& interaction, const math::Vec2& position) {
    // TODO: HitTest for Line
    return false;
}
}
