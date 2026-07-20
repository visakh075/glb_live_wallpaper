#pragma once

#include <memory>
#include <string>

class Renderer;
class Wallpaper;

class WallpaperManager
{
public:

    WallpaperManager();
    ~WallpaperManager();

    bool initialize(
        Renderer& renderer,
        int width,
        int height);

    bool load(
        const std::string& name,
        Renderer& renderer,
        int width,
        int height);

    void unload();

    void update(
        float dt);

    void render(
        Renderer& renderer);

    bool hasWallpaper() const;

    Wallpaper& wallpaper();

private:

    std::unique_ptr<Wallpaper>
        m_wallpaper;
};