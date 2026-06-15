cmake_minimum_required(VERSION 4.0)

project(astra_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_library(stb INTERFACE)

target_include_directories(stb INTERFACE
        third_party/stb
)

find_package(OpenGL REQUIRED)

add_executable(astra_engine MACOSX_BUNDLE
        src/main.cpp

        src/platform/window.cpp

        src/input/input.cpp
        src/input/keyboard.cpp
        src/input/mouse.cpp

        src/core/time.cpp

        src/ecs/entity_manager.cpp
        src/ecs/world.cpp

        src/ecs/systems/render_system.cpp
        src/ecs/systems/interaction_system.cpp
        src/ecs/systems/camera_system.cpp

        src/graphics/renderer.cpp
        src/graphics/render_dispatch.cpp
        src/graphics/shader.cpp
        src/graphics/mesh.cpp
        src/graphics/texture.cpp

        src/assets/image.cpp
)

target_compile_definitions(astra_engine PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(astra_engine PRIVATE
        include
)

target_link_libraries(astra_engine PRIVATE
        SDL3::SDL3
        OpenGL::GL
        glad
        stb
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(
            TARGET astra_engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${ASSETS_DIR}
            $<TARGET_BUNDLE_CONTENT_DIR:astra_engine>/Resources
    )
else ()
    add_custom_command(
            TARGET astra_engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ASSETS_DIR}
            $<TARGET_BUNDLE_CONTENT_DIR:astra_engine>/Resources
    )
endif ()