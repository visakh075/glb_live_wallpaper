cmake_minimum_required(VERSION 3.16)

project(
    LiveWall
    LANGUAGES CXX
)

# --------------------------------------------------
# C++
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------
# Dependencies
# --------------------------------------------------

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)


# --------------------------------------------------
# Source Files
# --------------------------------------------------

set(CORE_SOURCES

    src/Core/Wallpaper.cpp
    src/Core/Input.cpp
    src/Core/Renderer.cpp
    src/Core/Window.cpp
    src/Engine/Engine.cpp
    src/Engine/WallpaperManager.cpp
    src/Engine/WallpaperRegistry.cpp
)

set(WALLPAPER_SOURCES

    src/Wallpapers/GameOfLife/GameOfLife.cpp
    src/Wallpapers/Boids/Boids.cpp
    src/Wallpapers/Clock/ClockWallpaper.cpp
    src/Wallpapers/CoreStats/CoreStats.cpp
)

set(SOURCES

    src/main.cpp

    ${CORE_SOURCES}

    ${WALLPAPER_SOURCES}
)

# --------------------------------------------------
# Executable
# --------------------------------------------------

add_executable(
    livewall
    ${SOURCES}
)

# --------------------------------------------------
# Include Directories
# --------------------------------------------------

target_include_directories(
    livewall

    PRIVATE

    include
)

# --------------------------------------------------
# Libraries
# --------------------------------------------------

target_link_libraries(
    livewall
    
    PRIVATE
    Freetype::Freetype
    OpenGL::GL
    Xrender
    ${X11_LIBRARIES}
    pthread
)

# --------------------------------------------------
# Compiler Options
# --------------------------------------------------

target_compile_options(
    livewall

    PRIVATE

    -Wall
    -Wextra
    -Wpedantic
)