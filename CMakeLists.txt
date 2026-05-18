cmake_minimum_required(VERSION 4.0)
project(astra_engine)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)

add_library(glad
        third_party/glad/src/glad.c
)

target_include_directories(glad PUBLIC
        third_party/glad/include
)

find_package(OpenGL REQUIRED)

add_executable(astra_engine MACOSX_BUNDLE main.cpp
        src/platform/window.cpp
        src/render/draw.cpp
        src/input/input.cpp
        src/input/keyboard.cpp
        src/input/mouse.cpp
        src/core/time.cpp
        src/ecs/entity/entity_manager.cpp
        src/ecs/component/component_manager.cpp
        src/ecs/system/system_manager.cpp
        src/render/renderer.cpp
        src/render/render_dispatch.cpp
        src/ecs/system/shape_system.cpp
        src/ecs/world/world.cpp
        src/ecs/system/interaction_system.cpp
        src/ecs/system/camera_system.cpp
        src/assets/image_handler.cpp
        src/assets/shader.cpp
        src/render/mesh.cpp
        src/assets/texture.cpp
)

target_compile_definitions(astra_engine PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(astra_engine PRIVATE
        include
        third_party/stb
)

target_link_libraries(astra_engine PRIVATE
        SDL3::SDL3
        glad
        OpenGL::GL
)

add_custom_command(TARGET astra_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR}
        $<TARGET_BUNDLE_CONTENT_DIR:astra_engine>/Resources/assets
)