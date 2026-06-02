#include <astra/ecs/systems/interaction_system.hpp>
#include <astra/ecs/systems/camera_system.hpp>
#include <astra/input/mouse.hpp>
#include <astra/math/geometry.hpp>
#include <astra/math/vector.hpp>

namespace astra::ecs::systems {
    InteractionSystem::InteractionSystem(ComponentManager &componentManager, const components::Camera &camera,
                                         const input::Input &input) : System(componentManager), input(input),
                                                                      camera(camera), lastInteraction(nullptr) {
    }

    void InteractionSystem::update(double deltaTime) {
        components::Interaction *currentInteraction = hitTestInteraction();

        const auto interactionStorage = componentManager.getStorage<components::Interaction>();
        if (!interactionStorage)
            return;

        for (size_t interactionIndex = 0; interactionIndex < interactionStorage->size(); interactionIndex++) {
            components::Interaction &interaction = interactionStorage->getComponentAt(interactionIndex);
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

    components::Interaction *InteractionSystem::hitTestInteraction() {
        const auto transformStorage = componentManager.getStorage<components::Transform>();
        const auto shapeStorage = componentManager.getStorage<components::Shape>();
        const auto interactionStorage = componentManager.getStorage<components::Interaction>();

        HitItem topHitItem{0, nullptr};

        if (transformStorage == nullptr || shapeStorage == nullptr || interactionStorage == nullptr)
            return nullptr;

        for (size_t interactionIndex = 0; interactionIndex < interactionStorage->size(); interactionIndex++) {
            const EntityId entityId = interactionStorage->getEntityIdAt(interactionIndex);

            if (!shapeStorage->hasComponent(entityId) || !transformStorage->hasComponent(entityId))
                continue;

            components::Interaction &interaction = interactionStorage->getComponentAt(interactionIndex);
            auto &[geometry, style] = shapeStorage->getComponent(entityId);
            components::Transform &transform = transformStorage->getComponent(entityId);

            if (!interaction.enabled || !style.display)
                continue;

            std::visit(
                [this, &topHitItem, &interaction, &transform](auto &&shapeGeometry) {
                    if (hitTest(transform, shapeGeometry, interaction,
                                CameraSystem::screenToWorld(camera, input.mouse.position)) &&
                        topHitItem.zindex <= transform.zindex) {
                        topHitItem.interaction = &interaction;
                        topHitItem.zindex = transform.zindex;
                    }
                },
                geometry);
        };

        return topHitItem.interaction;
    }

    // TODO: Add rotation
    bool InteractionSystem::hitTest(const components::Transform &transform,
                                    const components::RectGeometry &rectangleGeometry,
                                    const components::Interaction &interaction, const math::Vec2 &position) {
        if (position.x > transform.position.x - (rectangleGeometry.size.x * transform.scale.x) / 2 &&
            position.x < transform.position.x + (rectangleGeometry.size.x * transform.scale.x) / 2 &&
            position.y > transform.position.y - (rectangleGeometry.size.y * transform.scale.y) / 2 &&
            position.y < transform.position.y + (rectangleGeometry.size.y * transform.scale.y) / 2)
            return true;

        return false;
    }

    bool InteractionSystem::hitTest(const components::Transform &transform,
                                    const components::CircleGeometry &circleGeometry,
                                    const components::Interaction &interaction, const math::Vec2 &position) {
        const float radius = circleGeometry.radius * std::max(transform.scale.x, transform.scale.y);
        if (position.distanceSquared(transform.position) <= radius * radius)
            return true;

        return false;
    }

    bool InteractionSystem::hitTest(const components::Transform &transform,
                                    const components::LineGeometry &lineGeometry,
                                    const components::Interaction &interaction, const math::Vec2 &position) {
        // TODO: HitTest for Line
        return false;
    }
}
