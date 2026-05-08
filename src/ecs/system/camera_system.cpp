#include "ecs/system/camera_system.hpp"
#include "SDL3/SDL_scancode.h"
#include "ecs/components/camera.hpp"
#include "input/mouse.hpp"
#include <algorithm>

namespace astra::ecs::system {
    CameraSystem::CameraSystem(component::ComponentManager &componentManager, component::Camera &camera,
                               const input::Input &input)
        : System(componentManager), camera(camera), input(input) {
    }

    void CameraSystem::update(double deltaTime) {
        // Panning if held Space + left Dragging OR held Middle Button
        const bool canPan = (input.keyboard.isKeyDown(SDL_SCANCODE_SPACE) &&
                             input.mouse.isMouseDown(input::MouseButton::Left)) ||
                            input.mouse.isMouseDown(input::MouseButton::Middle);
        if (canPan)
            camera.position += input.mouse.getMouseDelta() / camera.zoom;

        // Zooming with Mouse Piovt
        math::Vec2 mousePosBefore = CameraSystem::screenToWorld(camera, input.mouse.position);

        camera.zoom = std::clamp(camera.zoom + input.mouse.wheelDelta, 0.1f, 10.0f);

        math::Vec2 mousePosAfter = CameraSystem::screenToWorld(camera, input.mouse.position);

        camera.position += mousePosBefore - mousePosAfter;
    }
}