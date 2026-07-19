#pragma once

#include <cstdint>

struct Cell
{
    bool alive = false;

    float alpha = 0.0f;

    float scale = 0.0f;

    uint16_t age = 0;
};