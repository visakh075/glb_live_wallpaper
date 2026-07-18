cmake_minimum_required(VERSION 3.16)

project(LiveWall LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

add_executable(livewall
    src/main.cpp
    src/Window.cpp
    src/Renderer.cpp
    src/Input.cpp
    src/GameOfLife.cpp
    src/boids.cpp
)

target_include_directories(livewall
    PRIVATE
        include
)

target_link_libraries(livewall
    PRIVATE
        OpenGL::GL
        ${X11_LIBRARIES}
        pthread
)

target_compile_options(livewall
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)