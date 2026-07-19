#pragma once

#include <cstdint>

struct Event
{
    enum class Type
    {
        Invalid,

        WindowResize,

        MouseButtonPress,
        MouseButtonRelease,
        MouseMotion,

        KeyboardPress,
        KeyboardRelease
    };

    Type type = Type::Invalid;

    int x = 0;
    int y = 0;

    int width = 0;
    int height = 0;

    int button = 0;

    uint32_t key = 0;
};