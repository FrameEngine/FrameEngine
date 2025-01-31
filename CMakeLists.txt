cmake_minimum_required(VERSION 3.10)
project(FrameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(X11 REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(simulation ${SOURCES})
target_link_libraries(simulation ${X11_LIBRARIES})

enable_testing()

find_package(Catch2 REQUIRED)
add_executable(test_vector3 tests/unit/test_vector3.cpp)
target_link_libraries(test_vector3 PRIVATE Catch2::Catch2WithMain)

# Register the test
add_test(NAME Vector3Tests COMMAND test_vector3)
