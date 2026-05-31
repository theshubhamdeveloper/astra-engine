#include <algorithm>

#include <SDL3/SDL_scancode.h>
#include <astra/ecs/systems/camera_system.hpp>
#include <astra/ecs/components/camera.hpp>
#include <astra/input/mouse.hpp>

namespace astra::ecs::systems {
    CameraSystem::CameraSystem(ComponentManager &componentManager, components::Camera &camera,
                               const input::Input &input) : System(componentManager), input(input), camera(camera) {
    }

    void CameraSystem::update(double deltaTime) {
        // Panning if held Space + left Dragging OR held Middle Button
        const bool canPan = (input.keyboard.isKeyDown(SDL_SCANCODE_SPACE) &&
                             input.mouse.isMouseDown(input::MouseButton::Left)) ||
                            input.mouse.isMouseDown(input::MouseButton::Middle);
        if (canPan)
            camera.position += input.mouse.getMouseDelta() / camera.zoom;

        // Zooming with Mouse Pivot if Cmd or Ctrl or Meta key held
        if (input.keyboard.isKeyDown(SDL_SCANCODE_LGUI)) {
            const math::Vec2 mousePosBefore = screenToWorld(camera, input.mouse.position);

            camera.zoom = std::clamp(camera.zoom + (input.mouse.wheelDelta * camera.zoomSpeed), camera.minZoom,
                                     camera.maxZoom);

            const math::Vec2 mousePosAfter = screenToWorld(camera, input.mouse.position);

            camera.position += mousePosBefore - mousePosAfter;
        }
    }
}
