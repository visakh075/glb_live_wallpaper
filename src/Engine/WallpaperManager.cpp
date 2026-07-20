#include "Engine/WallpaperManager.h"

#include "Core/Wallpaper.h"
#include "Engine/WallpaperRegistry.h"
#include "Core/Renderer.h"

WallpaperManager::
WallpaperManager()
{
}

WallpaperManager::
~WallpaperManager()
{
    unload();
}

bool WallpaperManager::initialize(
    Renderer& renderer,
    int width,
    int height)
{
    return load(
        WallpaperRegistry::
        instance().
        defaultWallpaper(),
        renderer,
        width,
        height);
}

bool WallpaperManager::load(
    const std::string& name,
    Renderer& renderer,
    int width,
    int height)
{
    unload();

    m_wallpaper =
        WallpaperRegistry::
        instance().
        create(name);

    if(!m_wallpaper)
        return false;

    if(!m_wallpaper->
        initialize(renderer))
    {
        m_wallpaper.reset();
        return false;
    }

    Event e;

    e.type =
        Event::Type::
        WindowResize;

    e.width = width;
    e.height = height;

    m_wallpaper->onEvent(e);

    return true;
}

void WallpaperManager::unload()
{
    if(m_wallpaper)
    {
        m_wallpaper->shutdown();
        m_wallpaper.reset();
    }
}

void WallpaperManager::update(
    float dt)
{
    if(m_wallpaper)
        m_wallpaper->update(dt);
}

void WallpaperManager::render(
    Renderer& renderer)
{
    if(m_wallpaper)
        m_wallpaper->render(
            renderer);
}

bool WallpaperManager::
hasWallpaper() const
{
    return
        m_wallpaper != nullptr;
}

Wallpaper&
WallpaperManager::wallpaper()
{
    return *m_wallpaper;
}