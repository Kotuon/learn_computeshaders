cmake_minimum_required(VERSION 3.5.0)
project(cpp_template VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++latest")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++20")

    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

#
# GLFW options
#
option(GLFW_INSTALL OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

#
# Libraries with CMake
#
add_subdirectory(libraries/fmt)
add_subdirectory(libraries/traceSystem)
add_subdirectory(libraries/crashHandler)
add_subdirectory(libraries/profiler)
add_subdirectory(libraries/timeManager)
add_subdirectory(libraries/glfw)

#
# Source files
#
file(GLOB_RECURSE LIBRARY_SOURCES
    libraries/glad/src/glad.c
)
file(GLOB_RECURSE PROJECT_SOURCES
    src/*.cpp
    src/*.cxx
    src/*.cc
    src/*.c
)

file(COPY assets/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY assets/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

file(COPY assets/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY assets/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${LIBRARY_SOURCES})

include_directories(
    include/
    SYSTEM libraries/fmt/include
    SYSTEM libraries/traceSystem/include
    SYSTEM libraries/crashHandler/include
    SYSTEM libraries/profiler/include
    SYSTEM libraries/timeManager/include
    SYSTEM libraries/glad/include
    SYSTEM libraries/glfw/include
    SYSTEM libraries/glm
    SYSTEM libraries/glm/include
)

target_link_libraries(${PROJECT_NAME}
    glfw
    ${GLFW_LIBRARIES}
    ${GLAD_LIBRARIES}
    fmt::fmt
    traceSystem
    crashHandler
    profiler
    timeManager
)
