#pragma once

#include <memory>

#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/Wallpaper.h"

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

    std::unique_ptr<Wallpaper> m_wallpaper;
};