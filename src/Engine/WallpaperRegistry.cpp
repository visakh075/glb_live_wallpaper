#include "Engine/WallpaperRegistry.h"
#include "Core/Wallpaper.h"

WallpaperRegistry&
WallpaperRegistry::instance()
{
    static WallpaperRegistry registry;
    return registry;
}

bool WallpaperRegistry::registerWallpaper(
    const std::string& name,
    Factory factory,
    bool isDefault)
{
    WallpaperInfo info;

    info.factory = std::move(factory);
    info.isDefault = isDefault;

    return
    m_wallpapers.emplace(
        name,
        std::move(info)).second;
}

std::unique_ptr<Wallpaper>
WallpaperRegistry::create(
    const std::string& name)
{
    auto it =
        m_wallpapers.find(name);

    if(it == m_wallpapers.end())
        return nullptr;

    return it->second.factory();
}

std::vector<std::string>
WallpaperRegistry::names() const
{
    std::vector<std::string> result;

    for(const auto& wallpaper :
        m_wallpapers)
    {
        result.push_back(
            wallpaper.first);
    }

    return result;
}

std::string
WallpaperRegistry::defaultWallpaper() const
{
    for(const auto& wallpaper :
        m_wallpapers)
    {
        if(wallpaper.second.isDefault)
            return wallpaper.first;
    }

    if(!m_wallpapers.empty())
        return
        m_wallpapers.begin()->first;

    return {};
}