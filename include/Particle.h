#pragma once

struct Particle
{
    float x = 0.0f;
    float y = 0.0f;

    float vx = 0.0f;
    float vy = 0.0f;

    float ax = 0.0f;
    float ay = 0.0f;

    float size = 4.0f;

    float rotation = 0.0f;

    float angularVelocity = 0.0f;

    float life = 1.0f;
    float maxLife = 1.0f;

    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;

    bool alive = true;
};