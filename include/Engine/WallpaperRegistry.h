#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Wallpaper;

class WallpaperRegistry
{
public:

    using Factory =
        std::function<
            std::unique_ptr<Wallpaper>()>;

    static WallpaperRegistry& instance();

    bool registerWallpaper(
        const std::string& name,
        Factory factory,
        bool isDefault = false);

    std::unique_ptr<Wallpaper> create(
        const std::string& name);

    std::vector<std::string> names() const;

    std::string defaultWallpaper() const;

private:

    WallpaperRegistry() = default;

    struct WallpaperInfo
    {
        Factory factory;
        bool isDefault = false;
    };

    std::unordered_map<
        std::string,
        WallpaperInfo> m_wallpapers;
};