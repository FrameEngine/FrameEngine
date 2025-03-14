cmake_minimum_required(VERSION 3.10)
project(FrameEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_policy(SET CMP0072 NEW)

#STB_image
include_directories(external/include)

# GLFW
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
include_directories(external/glfw/include)

# GLAD
include(FetchContent)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glad)
target_include_directories(glad INTERFACE ${glad_SOURCE_DIR}/include)

# OpenGL
find_package(OpenGL REQUIRED)

# fmt through FetchContent if missing
find_package(fmt QUIET)
if(NOT fmt_FOUND)
    message(STATUS "fmt not found, fetching...")
    include(FetchContent)
    FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(fmt)
endif()

# Include
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_library(FrameEngineLib ${ENGINE_SOURCES})

target_include_directories(FrameEngineLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(FrameEngineLib PUBLIC OpenGL::GL glfw glad fmt::fmt)

add_executable(frame_engine src/main.cpp)
target_link_libraries(frame_engine PRIVATE FrameEngineLib)

# Testing
enable_testing()
find_package(Catch2 REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
# Remove main.cpp from the list of sources
list(FILTER SOURCES EXCLUDE REGEX ".*src/main\\.cpp$")


# function(add_unit_test TEST_NAME TEST_SOURCE)
#   add_executable(${TEST_NAME} ${TEST_SOURCE} ${SOURCES})
#     target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain glad glfw OpenGL::GL fmt::fmt)
#     add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
# endfunction()
#
# add_unit_test(test_vector3 tests/unit/test_vector3.cpp)
# add_unit_test(test_ecs tests/unit/test_ecs.cpp)
# add_unit_test(test_camera tests/unit/test_camera.cpp)
# add_unit_test(test_matrix4 tests/unit/test_matrix4.cpp)
#
#
# # Doxygen Documentation
# find_package(Doxygen REQUIRED)
# if(DOXYGEN_FOUND)
#     set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
#     set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)
#
#     add_custom_target(doc
#         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
#         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#         COMMENT "Generating Doxygen documentation"
#         VERBATIM
#     )
# endif()
