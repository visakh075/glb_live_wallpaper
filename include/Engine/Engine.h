#pragma once

#include "Core/Window.h"
#include "Core/Renderer.h"

#include "Engine/WallpaperManager.h"

class Engine
{
public:

    Engine();
    ~Engine();

    bool initialize();

    int run();

    void shutdown();

private:

    WindowManager m_window;

    Renderer m_renderer;

    WallpaperManager m_wallpapers;
};