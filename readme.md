# LiveWall

**LiveWall** is a lightweight, open-source wallpaper engine for Linux written entirely in modern C++.

Unlike traditional wallpaper applications, LiveWall is designed as a **framework** where every wallpaper is implemented as an independent module. Developers can create their own animated wallpapers by simply adding a new wallpaper class and rebuilding the project.

The project focuses on simplicity, performance, and extensibility while avoiding unnecessary dependencies.

## Features

* Native Linux implementation using **X11** and **OpenGL**
* Lightweight and fast
* Modular wallpaper architecture
* Simple renderer for 2D graphics
* Event-driven input system
* Easy-to-extend wallpaper API
* Modern CMake project structure
* Designed for learning graphics programming and engine architecture

## Current Wallpapers

* Conway's Game of Life

More wallpapers are currently under development, including:

* Boids
* Particle System
* Rain
* Snow
* Fire
* Fluid Simulation
* Browser-based Wallpapers

## Project Goals

LiveWall aims to become a complete wallpaper engine for Linux while remaining simple enough that anyone can understand its source code.

The long-term vision includes:

* Plugin-based wallpaper system
* Shader support
* Texture loading
* 3D model rendering
* Audio-reactive wallpapers
* Browser/HTML wallpapers
* Configuration system
* Wayland support
* Multi-monitor support

## Philosophy

LiveWall is built around a few simple principles:

* Keep the core engine independent of wallpapers.
* Make wallpapers easy to create.
* Keep dependencies minimal.
* Write clean, readable C++.
* Learn by building rather than hiding complexity behind large libraries.

## Contributing

Contributions, ideas, bug reports, and new wallpapers are always welcome.

Whether you're fixing a typo, improving the renderer, or creating an entirely new wallpaper, every contribution helps improve the project.

## License

This project is open source and released under the MIT License.
