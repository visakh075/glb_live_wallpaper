#pragma once

struct Vec2
{
    float x = 0.f;
    float y = 0.f;

    Vec2 operator+(const Vec2& o) const
    {
        return {x + o.x, y + o.y};
    }

    Vec2 operator-(const Vec2& o) const
    {
        return {x - o.x, y - o.y};
    }

    Vec2 operator*(float s) const
    {
        return {x * s, y * s};
    }

    Vec2 operator/(float s) const
    {
        return {x / s, y / s};
    }

    Vec2& operator+=(const Vec2& o)
    {
        x += o.x;
        y += o.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& o)
    {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    Vec2& operator/=(float s)
    {
        x /= s;
        y /= s;
        return *this;
    }
};

struct Boid
{
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;

    int group = 0;

    bool predator = false;

    float r = 1.f;
    float g = 1.f;
    float b = 1.f;
};