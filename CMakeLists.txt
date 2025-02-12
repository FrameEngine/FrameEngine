cmake_minimum_required(VERSION 3.10)
project(FrameEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(external/glfw)
include_directories(external/glfw/include)

add_library(glad STATIC external/glad/src/glad.c)
include_directories(external/glad/include)

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(frame_engine ${SOURCES})

target_link_libraries(frame_engine OpenGL::GL glfw glad)

enable_testing()

find_package(Catch2 REQUIRED)

add_executable(test_vector3 tests/unit/test_vector3.cpp)
target_link_libraries(test_vector3 PRIVATE Catch2::Catch2WithMain)

add_executable(test_ecs tests/unit/test_ecs.cpp)
target_link_libraries(test_ecs PRIVATE Catch2::Catch2WithMain)

# Register the tests
add_test(NAME Vector3Tests COMMAND test_vector3)
add_test(NAME ECSTests COMMAND test_ecs)
