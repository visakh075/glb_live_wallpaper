cmake_minimum_required(VERSION 3.10)

project(Wallpaper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

add_executable(wallpaper
    main.cpp
)

target_link_libraries(wallpaper
    PRIVATE
        OpenGL::GL
        X11::X11
        pthread
)