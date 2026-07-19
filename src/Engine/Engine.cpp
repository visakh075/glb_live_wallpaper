#include "Engine/Engine.h"

#include "Wallpapers/GameOfLife/GameOfLife.h"

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

    m_wallpaper =
        std::make_unique<GameOfLife>();

    if(!m_wallpaper->initialize(m_renderer))
        return false;

    m_wallpaper->resize(
        m_window.width(),
        m_window.height());

    return true;
}

int Engine::run()
{
    if(!initialize())
        return -1;

    while(m_window.running())
    {
        m_window.pollEvents(
            *m_wallpaper);

        m_wallpaper->update(
            1.0f / 60.0f);

        m_renderer.beginFrame();

        m_wallpaper->render(
            m_renderer);

        m_renderer.endFrame();

        m_window.swapBuffers();
    }

    shutdown();

    return 0;
}

void Engine::shutdown()
{
    if(m_wallpaper)
    {
        m_wallpaper->shutdown();

        m_wallpaper.reset();
    }

    m_renderer.shutdown();

    m_window.destroy();
}