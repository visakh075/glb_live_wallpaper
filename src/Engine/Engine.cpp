#include "Engine/Engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{
    shutdown();
}

bool Engine::initialize()
{
    if(!m_window.create())
        return false;

    if(!m_renderer.initialize())
        return false;

    m_renderer.resize(
        m_window.width(),
        m_window.height());

    if(!m_wallpapers.initialize(
            m_renderer,
            m_window.width(),
            m_window.height()))
    {
        return false;
    }

    return true;
}

int Engine::run()
{
    if(!initialize())
        return -1;

    constexpr float dt =
        1.0f / 60.0f;

    while(m_window.running())
    {
        if(m_wallpapers.hasWallpaper())
        {
            m_window.pollEvents(
                m_wallpapers.wallpaper());
        }

        m_wallpapers.update(dt);

        m_renderer.beginFrame();

        m_wallpapers.render(
            m_renderer);

        m_renderer.endFrame();

        m_window.swapBuffers();
    }

    shutdown();

    return 0;
}

void Engine::shutdown()
{
    m_wallpapers.unload();

    m_renderer.shutdown();

    m_window.destroy();
}