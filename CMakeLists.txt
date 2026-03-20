cmake_minimum_required(VERSION 4.0)
project(astra_engine)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(third_party/sdl EXCLUDE_FROM_ALL)

add_executable(astra_engine MACOSX_BUNDLE main.cpp)
