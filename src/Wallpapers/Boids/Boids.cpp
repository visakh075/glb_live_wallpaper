#include "Wallpapers/Boids/Boids.h"

#include <cstdlib>

#include "Core/Renderer.h"
#include "Core/RegisterWallpaper.h"

bool Boids::initialize(
    Renderer&)
{
    m_boids.resize(300);

    for(auto& b :
        m_boids)
    {
        b.x =
            rand() % 1920;

        b.y =
            rand() % 1080;

        b.vx =
            ((rand()%100)/50.f)-1.f;

        b.vy =
            ((rand()%100)/50.f)-1.f;
    }

    return true;
}

void Boids::update(
    float dt)
{
    for(auto& b :
        m_boids)
    {
        b.x += b.vx * 100.f * dt;
        b.y += b.vy * 100.f * dt;

        if(b.x < 0)
            b.x += m_width;

        if(b.y < 0)
            b.y += m_height;

        if(b.x >= m_width)
            b.x -= m_width;

        if(b.y >= m_height)
            b.y -= m_height;
    }
}

void Boids::render(
    Renderer& renderer)
{
    for(const auto& b :
        m_boids)
    {
        renderer.drawRectangle(
            b.x,
            b.y,
            2,
            2,
            1,
            1,
            1);
    }
}

void Boids::onEvent(
    const Event& e)
{
    if(e.type ==
        Event::Type::
        WindowResize)
    {
        m_width =
            e.width;

        m_height =
            e.height;
    }
}

// LIVEWALL_REGISTER_DEFAULT_WALLPAPER(
//     Boids)