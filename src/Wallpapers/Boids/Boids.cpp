#include "Wallpapers/Boids/Boids.h"

#include <cmath>
#include <cstdlib>
#include <algorithm>

#include "Core/Renderer.h"
#include "Core/RegisterWallpaper.h"

bool Boids::initialize(
    Renderer&)
{
    return true;
}

void Boids::shutdown()
{
    m_boids.clear();
}

void Boids::createBoids()
{
    m_boids.clear();

    for(int i=0;
        i<m_numBoids;
        i++)
    {
        Boid b;

        b.pos =
        {
            float(rand()%m_width),
            float(rand()%m_height)
        };

        float angle =
            float(rand())/
            RAND_MAX*
            6.283185f;

        b.vel =
        {
            cos(angle)
            *
            m_maxVelocity,

            sin(angle)
            *
            m_maxVelocity
        };

        b.group =
            rand()
            %
            m_numGroups;

        auto c =
            m_groupColors[
                b.group];

        b.r = c.r;
        b.g = c.g;
        b.b = c.b;

        m_boids.push_back(b);
    }

    //
    // predators
    //

    for(int i=0;
        i<m_numPredators;
        i++)
    {
        Boid p;

        p.predator =
            true;

        p.r = 1.f;
        p.g = 0.1f;
        p.b = 0.1f;

        p.pos =
        {
            float(rand()%m_width),
            float(rand()%m_height)
        };

        float angle =
            float(rand())/
            RAND_MAX*
            6.283185f;

        p.vel =
        {
            cos(angle)
            *
            m_maxVelocity
            *
            1.5f,

            sin(angle)
            *
            m_maxVelocity
            *
            1.5f
        };

        m_boids.push_back(p);
    }
}

static float length(
    const Vec2& v)
{
    return std::sqrt(
        v.x * v.x +
        v.y * v.y);
}

static Vec2 normalize(
    const Vec2& v)
{
    float l =
        length(v);

    if(l < 0.0001f)
        return {};

    return v / l;
}

static Vec2 limit(
    const Vec2& v,
    float max)
{
    float l =
        length(v);

    if(l <= max)
        return v;

    return
        normalize(v)
        * max;
}

float Boids::distance(
    const Boid& a,
    const Boid& b)
{
    float dx =
        fabs(
            a.pos.x -
            b.pos.x);

    float dy =
        fabs(
            a.pos.y -
            b.pos.y);

    if(dx >
       m_width * 0.5f)
    {
        dx =
            m_width -
            dx;
    }

    if(dy >
       m_height * 0.5f)
    {
        dy =
            m_height -
            dy;
    }

    return
        std::sqrt(
            dx * dx +
            dy * dy);
}

Vec2 Boids::separation(
    size_t index)
{
    Vec2 desired;

    int count = 0;

    auto& self =
        m_boids[index];

    for(size_t i = 0;
        i < m_boids.size();
        ++i)
    {
        if(i == index)
            continue;

        auto& other =
            m_boids[i];

        if(other.predator)
            continue;

        if(other.group !=
           self.group)
        {
            continue;
        }

        float d =
            distance(
                self,
                other);

        if(d >
           m_perceptionRadius)
        {
            continue;
        }

        if(d <
           0.001f)
        {
            continue;
        }

        Vec2 diff =
            self.pos -
            other.pos;

        diff /=
            d;

        desired +=
            diff;

        count++;
    }

    if(count > 0)
    {
        desired /=
            float(count);

        desired =
            normalize(
                desired)
            *
            m_maxVelocity;

        desired -=
            self.vel;

        desired =
            limit(
                desired,
                m_maxAcceleration);
    }

    return desired;
}

Vec2 Boids::alignment(
    size_t index)
{
    Vec2 desired;

    int count = 0;

    auto& self =
        m_boids[index];

    for(size_t i = 0;
        i < m_boids.size();
        ++i)
    {
        if(i == index)
            continue;

        auto& other =
            m_boids[i];

        if(other.predator)
            continue;

        if(other.group !=
           self.group)
        {
            continue;
        }

        float d =
            distance(
                self,
                other);

        if(d >
           m_perceptionRadius)
        {
            continue;
        }

        desired +=
            other.vel;

        count++;
    }

    if(count > 0)
    {
        desired /=
            float(count);

        desired =
            normalize(
                desired)
            *
            m_maxVelocity;

        desired -=
            self.vel;

        desired =
            limit(
                desired,
                m_maxAcceleration);
    }

    return desired;
}

Vec2 Boids::cohesion(
    size_t index)
{
    Vec2 desired;

    int count = 0;

    auto& self =
        m_boids[index];

    for(size_t i = 0;
        i < m_boids.size();
        ++i)
    {
        if(i == index)
            continue;

        auto& other =
            m_boids[i];

        if(other.predator)
            continue;

        if(other.group !=
           self.group)
        {
            continue;
        }

        float d =
            distance(
                self,
                other);

        if(d >
           m_perceptionRadius)
        {
            continue;
        }

        desired +=
            other.pos;

        count++;
    }

    if(count > 0)
    {
        desired /=
            float(count);

        desired -=
            self.pos;

        desired =
            normalize(
                desired)
            *
            m_maxVelocity;

        desired -=
            self.vel;

        desired =
            limit(
                desired,
                m_maxAcceleration);
    }

    return desired;
}

void Boids::update(
    float dt)
{
    for(size_t i = 0;
        i < m_boids.size();
        ++i)
    {
        auto& b =
            m_boids[i];

        //
        // Predator AI
        //
        if(b.predator)
        {
            int nearest =
                -1;

            float best =
                1e9f;

            for(size_t j = 0;
                j < m_boids.size();
                ++j)
            {
                if(m_boids[j]
                    .predator)
                {
                    continue;
                }

                float d =
                    distance(
                        b,
                        m_boids[j]);

                if(d < best)
                {
                    best =
                        d;

                    nearest =
                        j;
                }
            }

            if(nearest >= 0)
            {
                Vec2 desired =
                    m_boids[
                        nearest]
                        .pos
                    -
                    b.pos;

                desired =
                    normalize(
                        desired);

                desired =
                    desired
                    *
                    m_maxVelocity
                    *
                    2.f;

                b.acc +=
                    desired;
            }
        }
        else
        {
            //
            // Flee predators
            //
            for(auto& p :
                m_boids)
            {
                if(!p.predator)
                    continue;

                float d =
                    distance(
                        b,
                        p);

                if(d <
                   200.f)
                {
                    Vec2 flee =
                        b.pos -
                        p.pos;

                    flee =
                        normalize(
                            flee);

                    flee =
                        flee
                        *
                        4.f;

                    b.acc +=
                        flee;
                }
            }

            Vec2 sep =
                separation(i)
                *
                m_separationWeight;

            Vec2 ali =
                alignment(i)
                *
                m_alignmentWeight;

            Vec2 coh =
                cohesion(i)
                *
                m_cohesionWeight;

            b.acc +=
                sep +
                ali +
                coh;
        }

        b.acc =
            limit(
                b.acc,
                m_maxAcceleration);

        b.vel +=
            b.acc;

        if(length(
            b.vel) >
            0.001f)
        {
            float speed =
                b.predator ?
                m_maxVelocity
                * 1.6f :
                m_maxVelocity;

            b.vel =
                normalize(
                    b.vel)
                *
                speed;
        }

        b.pos +=
            b.vel *
            dt *
            60.f;

        wrap(b);

        b.acc = {};
    }
}

void Boids::wrap(
    Boid& b)
{
    if(b.pos.x < 0)
        b.pos.x += m_width;

    if(b.pos.x > m_width)
        b.pos.x -= m_width;

    if(b.pos.y < 0)
        b.pos.y += m_height;

    if(b.pos.y > m_height)
        b.pos.y -= m_height;
}

void Boids::render(
    Renderer& renderer)
{
    for(auto& b :
        m_boids)
    {
        float theta =
            atan2(
                b.vel.y,
                b.vel.x);

        float side =
            m_scale *
            (b.predator ?
                1.0f :
                m_bodyWidth);

        float nose =
            m_scale *
            (b.predator ?
                4.0f :
                m_bodyLength);

        float tail =
            m_scale *
            (b.predator ?
                1.0f :
                m_tailLength);

        float x =
            b.pos.x;

        float y =
            b.pos.y;

        float dirx =
            cos(theta);

        float diry =
            sin(theta);

        float perpx =
            -diry;

        float perpy =
            dirx;

        float x1 =
            x -
            perpx * side -
            dirx * tail;

        float y1 =
            y -
            perpy * side -
            diry * tail;

        float x2 =
            x +
            perpx * side -
            dirx * tail;

        float y2 =
            y +
            perpy * side -
            diry * tail;

        float x3 =
            x +
            dirx *
            nose;

        float y3 =
            y +
            diry *
            nose;

        renderer.drawLine(
            x1, y1,
            x2, y2,
            b.r,
            b.g,
            b.b,
            1.f);

        renderer.drawLine(
            x2, y2,
            x3, y3,
            b.r,
            b.g,
            b.b,
            1.f);

        renderer.drawLine(
            x3, y3,
            x1, y1,
            b.r,
            b.g,
            b.b,
            1.f);
    }
}
void Boids::onEvent(
    const Event& e)
{
    if(e.type ==
       Event::Type::WindowResize)
    {
        m_width =
            e.width;

        m_height =
            e.height;

        if(m_boids.empty())
        {
            createBoids();
        }
    }
}

// #include "Core/RegisterWallpaper.h"
// LIVEWALL_REGISTER_DEFAULT_WALLPAPER(
//     Boids)