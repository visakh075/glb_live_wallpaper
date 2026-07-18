#include "Boids.h"

#include "Renderer.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace
{
    constexpr float MAX_SPEED = 120.0f;
    constexpr float VIEW_RADIUS = 60.0f;

    constexpr float SEPARATION_RADIUS  = 20.0f;

    constexpr float SEPARATION_FORCE   = 120.0f;
    constexpr float ALIGNMENT_FORCE    = 2.0f;
    constexpr float COHESION_FORCE     = 0.5f;
}

Boids::Boids()
{
    std::srand(
        static_cast<unsigned>(
            std::time(nullptr)));
}

bool Boids::initialize(
    Renderer& renderer)
{
    (void)renderer;

    createBoids(200);

    return true;
}

void Boids::resize(
    int width,
    int height)
{
    m_width = width;
    m_height = height;
}

void Boids::createBoids(
    int count)
{
    m_boids.clear();

    m_boids.reserve(count);

    for(int i=0;i<count;i++)
    {
        Boid b;

        b.x =
            static_cast<float>(
                rand()%std::max(1,m_width));

        b.y =
            static_cast<float>(
                rand()%std::max(1,m_height));

        float angle =
            static_cast<float>(rand()) /
            RAND_MAX *
            6.2831853f;

        float speed =
            50.0f +
            static_cast<float>(rand()%50);

        b.vx =
            std::cos(angle) * speed;

        b.vy =
            std::sin(angle) * speed;

        b.ax = 0.0f;
        b.ay = 0.0f;

        m_boids.push_back(b);
    }
}

float Boids::distanceSquared(
    const Boid& a,
    const Boid& b) const
{
    float dx =
        a.x - b.x;

    float dy =
        a.y - b.y;

    return dx*dx + dy*dy;
}

void Boids::limitSpeed(
    Boid& boid)
{
    float speed =
        std::sqrt(
            boid.vx*boid.vx +
            boid.vy*boid.vy);

    if(speed <= MAX_SPEED)
        return;

    float scale =
        MAX_SPEED / speed;

    boid.vx *= scale;
    boid.vy *= scale;
}

void Boids::wrap(
    Boid& boid)
{
    if(boid.x < 0)
        boid.x += m_width;

    if(boid.x >= m_width)
        boid.x -= m_width;

    if(boid.y < 0)
        boid.y += m_height;

    if(boid.y >= m_height)
        boid.y -= m_height;
}

void Boids::separation(int i)
{
    constexpr float FORCE = 120.0f;

    Boid& me = m_boids[i];

    float steerX = 0.0f;
    float steerY = 0.0f;

    int count = 0;

    for(size_t j = 0; j < m_boids.size(); j++)
    {
        if(i == static_cast<int>(j))
            continue;

        const Boid& other = m_boids[j];

        float dx = me.x - other.x;
        float dy = me.y - other.y;

        float d2 = dx * dx + dy * dy;

        if(d2 > SEPARATION_RADIUS * SEPARATION_RADIUS)
            continue;

        if(d2 < 0.001f)
            continue;

        float d = std::sqrt(d2);

        steerX += dx / d;
        steerY += dy / d;

        count++;
    }

    if(count == 0)
        return;

    steerX /= count;
    steerY /= count;

    me.ax += steerX * FORCE;
    me.ay += steerY * FORCE;
}

void Boids::alignment(int i)
{
    constexpr float FORCE = 2.0f;

    Boid& me = m_boids[i];

    float avgVX = 0.0f;
    float avgVY = 0.0f;

    int count = 0;

    for(size_t j = 0; j < m_boids.size(); j++)
    {
        if(i == static_cast<int>(j))
            continue;

        const Boid& other = m_boids[j];

        if(distanceSquared(me, other) >
            VIEW_RADIUS * VIEW_RADIUS)
            continue;

        avgVX += other.vx;
        avgVY += other.vy;

        count++;
    }

    if(count == 0)
        return;

    avgVX /= count;
    avgVY /= count;

    me.ax += (avgVX - me.vx) * FORCE;
    me.ay += (avgVY - me.vy) * FORCE;
}

void Boids::cohesion(int i)
{
    constexpr float FORCE = 0.5f;

    Boid& me = m_boids[i];

    float centerX = 0.0f;
    float centerY = 0.0f;

    int count = 0;

    for(size_t j = 0; j < m_boids.size(); j++)
    {
        if(i == static_cast<int>(j))
            continue;

        const Boid& other = m_boids[j];

        if(distanceSquared(me, other) >
            VIEW_RADIUS * VIEW_RADIUS)
            continue;

        centerX += other.x;
        centerY += other.y;

        count++;
    }

    if(count == 0)
        return;

    centerX /= count;
    centerY /= count;

    me.ax += (centerX - me.x) * FORCE;
    me.ay += (centerY - me.y) * FORCE;
}

void Boids::update(float dt)
{
    //---------------------------------
    // Clear acceleration
    //---------------------------------

    for(auto& boid : m_boids)
    {
        boid.ax = 0.0f;
        boid.ay = 0.0f;
    }

    //---------------------------------
    // Compute steering
    //---------------------------------

    for(size_t i=0;i<m_boids.size();i++)
    {
        separation(i);
        alignment(i);
        cohesion(i);
    }

    //---------------------------------
    // Integrate
    //---------------------------------

    for(auto& boid : m_boids)
    {
        boid.vx += boid.ax * dt;
        boid.vy += boid.ay * dt;

        limitSpeed(boid);

        boid.x += boid.vx * dt;
        boid.y += boid.vy * dt;

        wrap(boid);
    }
}

void Boids::render(Renderer& renderer)
{
    constexpr float SIZE = 8.0f;

    for(const auto& boid : m_boids)
    {
        float angle =
            std::atan2(
                boid.vy,
                boid.vx);

        float c = std::cos(angle);
        float s = std::sin(angle);

        float x1 = boid.x + c * SIZE;
        float y1 = boid.y + s * SIZE;

        float x2 =
            boid.x
            - c * SIZE * 0.5f
            - s * SIZE * 0.5f;

        float y2 =
            boid.y
            - s * SIZE * 0.5f
            + c * SIZE * 0.5f;

        float x3 =
            boid.x
            - c * SIZE * 0.5f
            + s * SIZE * 0.5f;

        float y3 =
            boid.y
            - s * SIZE * 0.5f
            - c * SIZE * 0.5f;

        renderer.drawTriangle(
            x1,y1,
            x2,y2,
            x3,y3,
            0.2f,
            0.7f,
            1.0f,
            1.0f);
    }
}

void Boids::mousePress(
    int button,
    int x,
    int y)
{
    switch(button)
    {
        //--------------------------------
        // Left
        //--------------------------------

        case Button1:
        {
            for(int i=0;i<25;i++)
            {
                Boid b;

                b.x = x;
                b.y = y;

                float angle =
                    (float)rand()/RAND_MAX
                    *6.2831853f;

                float speed =
                    60.0f
                    +(rand()%40);

                b.vx =
                    std::cos(angle)*speed;

                b.vy =
                    std::sin(angle)*speed;

                b.ax = 0;
                b.ay = 0;

                m_boids.push_back(b);
            }

            break;
        }

        //--------------------------------
        // Middle
        //--------------------------------

        case Button2:

            createBoids(200);

            break;

        //--------------------------------
        // Right
        //--------------------------------

        case Button3:

            if(m_boids.size()>25)
                m_boids.resize(
                    m_boids.size()-25);

            break;
    }
}

void Boids::shutdown()
{
    m_boids.clear();
}