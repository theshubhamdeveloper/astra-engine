// #include <SDL3/SDL.h>
// #include <freetype/freetype.h>
//
// #include <astra/core/resource_manager.hpp>
// #include <astra/core/time.hpp>
// #include <astra/ecs/world.hpp>
// #include <astra/graphics/renderer.hpp>
// #include <astra/input/input.hpp>
// #include <astra/platform/window.hpp>
//
// using namespace astra;
//
// constexpr int SCREEN_WIDTH = 1280;
// constexpr int SCREEN_HEIGHT = 720;
//
// int main() {
//     if (SDL_Init(SDL_INIT_VIDEO) == false) {
//         std::cout << "SDL could not initialize! SDL error: \n" << SDL_GetError() << std::endl;
//         return 3;
//     }
//
//     FT_Library library;
//
//     if (FT_Init_FreeType(&library) != FT_Err_Ok) {
//         std::cout << "Could not initialize FreeType library! \n" << std::endl;
//         return 3;
//     };
//
//     auto window = platform::Window("Astra Engine", {SCREEN_WIDTH, SCREEN_HEIGHT});
//
//     auto resourceManager = core::ResourceManager();
//
//     auto time = core::Time();
//
//     auto graphicCamera = graphics::GraphicCamera{
//         {},
//         0,
//         1,
//     };
//
//     auto renderer = graphics::Renderer(resourceManager, graphicCamera);
//
//     window.initialize(false);
//
//     auto input = input::Input(window.window());
//
//     auto world = ecs::World(input);
//
//     graphicCamera.projection =
//             graphics::GraphicCamera::orthographic(0, window.sizeInPixels().x, window.sizeInPixels().y, 0);
//
//     renderer.initialize(math::vec2{window.sizeInPixels()} / math::vec2{window.size()});
//
//     world.initialize(renderer, resourceManager);
//
//     const ecs::components::Camera &worldCamera = world.getCamera();
//
//     const auto jetbrainsFontFamily = resourceManager.fontFamilies.load({&resourceManager, library});
//     auto &jetbrainsFont = resourceManager.fontFamilies.get(jetbrainsFontFamily);
//     jetbrainsFont.addFace("../Resources/fonts/jetbrains-mono/JetBrainsMono-VariableFont_wght.ttf");
//     jetbrainsFont.addFace("../Resources/fonts/jetbrains-mono/JetBrainsMono-Italic-VariableFont_wght.ttf");
//
//     double cooldown = 0.5;
//     bool appear = false;
//
//     bool running = true;
//
//     uint32_t currentPos = 0;
//     std::string data = "Hello world my fellow";
//
//
//     input.onText().connect([&data, &currentPos](const std::string &text) {
//         data.insert(data.begin() + currentPos, text.begin(), text.end());
//         currentPos += text.size();
//     });
//
//     input.keyboard.onKeyDown().connect([&data, &currentPos](const SDL_Scancode &key, const bool repeat) {
//         SDL_Keymod mods = SDL_GetModState();
//
//         bool cmd =
//                 mods & SDL_KMOD_GUI;
//
//         bool ctrl =
//                 mods & SDL_KMOD_CTRL;
//
//         bool alt =
//                 mods & SDL_KMOD_ALT;
//
//         bool shift =
//                 mods & SDL_KMOD_SHIFT;
//
//         switch (key) {
//             case SDL_SCANCODE_BACKSPACE: {
//                 if (data.empty() || currentPos == 0) break;
//
//                 if (alt) {
//                     for (int i = currentPos; i >= 0; --i) {
//                         if (data[i] == ' ') {
//                             data.erase(data.begin() + i, data.begin() + currentPos);
//                         }
//                     }
//                     break;
//                 }
//
//                 data.erase(data.begin() + currentPos);
//                 currentPos -= 1;
//
//                 break;
//             }
//
//             case SDL_SCANCODE_RETURN: {
//                 data.insert(data.begin() + currentPos, '\n');
//                 currentPos += 1;
//                 break;
//             }
//
//             case SDL_SCANCODE_LEFT: {
//                 if (currentPos != 0) {
//                     currentPos -= 1;
//                 }
//                 break;
//             }
//
//             case SDL_SCANCODE_RIGHT: {
//                 if (currentPos != data.size()) {
//                     currentPos += 1;
//                 }
//                 break;
//             }
//             case SDL_SCANCODE_TAB: {
//                 std::string tab = "    ";
//                 data.insert(data.begin() + currentPos, tab.begin(), tab.end());
//                 currentPos += tab.size();
//                 break;
//             }
//             default:
//                 break;
//         }
//     });
//
//     while (running) {
//         time.update();
//         input.updateState();
//
//         input.onQuit().connect([&running]() {
//             running = false;
//         });
//
//         window.clear(math::Color{14, 26, 37});
//
//         renderer.begin();
//
//         world.update(time.deltaTime());
//
//         //sync graphicCamera and worldCamera
//         graphicCamera.position.x = floor(worldCamera.position.x);
//         graphicCamera.position.y = floor(worldCamera.position.y);
//         graphicCamera.zoom = worldCamera.zoom;
//
//         if (input.keyboard.isKeyPressed(SDL_SCANCODE_I)) {
//             input.startTextInput();
//         }
//
//         if (input.keyboard.isKeyPressed(SDL_SCANCODE_ESCAPE)) {
//             input.stopTextInput();
//         }
//
//         cooldown -= time.deltaTime();
//
//         if (cooldown <= 0) {
//             appear = !appear;
//             cooldown = 0.5;
//         }
//
//         // renderer.drawText({
//         //     .position = {-500, -500},
//         //     .fontFamily = jetbrainsFontFamily,
//         //     .text = std::format("Frame: {} ms\nFPS: {}\nDraw Calls: {}\nInput Text: {}", time.deltaTime() * 1000,
//         //                         time.fps(),
//         //                         renderer.drawCallCount(), input.isTextInput()),
//         //     .size = 20,
//         //     .style = {.color = math::Color::white(), .weight = 600, .italic = true},
//         // });
//
//         // renderer.drawRect({
//         //     .position = {600, 600},
//         //     .size = {424, 124},
//         //     .rotation = 0,
//         //     .style = graphics::RectStyle{
//         //         .fill = {255, 255, 255, 255},
//         //         .cornerRadius = math::vec4{16},
//         //     }
//         // });
//
//         renderer.drawText({
//             .position = {0, 0},
//             .fontFamily = jetbrainsFontFamily,
//             .text = data,
//             .size = 18,
//             .style = {.color = math::Color::red(), .weight = 700, .italic = true},
//         });
//
//         renderer.end();
//
//         window.render();
//         input.updateCurrentToPrevious();
//     }
//     window.destroy();
//     return 0;
// };

#include "astra/graphics/font/text_layout.hpp"

int main() {
    astra::graphics::TextLayout textLayout;

    astra::graphics::TextBlock textBlock = {
        .text = "Hello beautiful World! My Mind Is GOOD",
        .textStyle = {},
        .spans = {
            {
                .range = {.begin = 6, .end = 16},
                .style = {}
            },
            {
                .range = {.begin = 16, .end = 23},
                .style = {}
            },
            {
                .range = {.begin = 34, .end = 38},
                .style = {}
            },
        },
    };

    textLayout.layout(textBlock);

    return 0;
}
