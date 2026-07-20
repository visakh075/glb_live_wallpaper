#pragma once

#include <vector>

#include "Core/Wallpaper.h"

struct Boid
{
    float x;
    float y;

    float vx;
    float vy;
};

class Boids :
    public Wallpaper
{
public:

    bool initialize(
        Renderer& renderer)
        override;

    void update(
        float dt)
        override;

    void render(
        Renderer& renderer)
        override;

    void onEvent(
        const Event& e)
        override;

private:

    std::vector<Boid>
        m_boids;

    int m_width = 0;
    int m_height = 0;
};