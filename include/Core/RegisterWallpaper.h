#pragma once

#include <memory>

#include "Engine/WallpaperRegistry.h"

#define LIVEWALL_REGISTER_WALLPAPER(TYPE)          \
namespace                                          \
{                                                  \
    [[maybe_unused]]                               \
    const bool registered_##TYPE =                 \
        WallpaperRegistry::instance()              \
            .registerWallpaper(                    \
                #TYPE,                             \
                []()                               \
                {                                  \
                    return std::make_unique<TYPE>(); \
                });                               \
}

#define LIVEWALL_REGISTER_DEFAULT_WALLPAPER(TYPE)  \
namespace                                          \
{                                                  \
    [[maybe_unused]]                               \
    const bool registered_##TYPE =                 \
        WallpaperRegistry::instance()              \
            .registerWallpaper(                    \
                #TYPE,                             \
                []()                               \
                {                                  \
                    return std::make_unique<TYPE>(); \
                },                                 \
                true);                             \
}