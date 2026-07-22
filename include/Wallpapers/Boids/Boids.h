#pragma once

#include <vector>

#include "Core/Wallpaper.h"
#include "Wallpapers/Boids/Boid.h"

class Renderer;

class Boids :
    public Wallpaper
{
public:

    bool initialize(
        Renderer& renderer) override;

    void shutdown() override;

    void update(
        float dt) override;

    void render(
        Renderer& renderer) override;

    void onEvent(
        const Event& e) override;

private:

    int m_width = 0;
    int m_height = 0;

    std::vector<Boid> m_boids;

    float m_perceptionRadius = 50.f;

    float m_maxVelocity = 4.f;
    float m_maxAcceleration = 0.2f;

    float m_separationWeight = 1.5f;
    float m_alignmentWeight = 1.5f;
    float m_cohesionWeight = 1.0f;

    // float m_scale = 5.0f;
    // float m_bodyWidth = 0.5f;
    // float m_bodyLength = 2.0f;
    // float m_tailLength = 0.35f;

    float m_scale = 6.f;

    float m_bodyWidth  = 0.55f;
    float m_bodyLength = 2.5f;
    float m_tailLength = 0.4f;

    bool m_fillTriangles = false;


private:

    void createBoids();

    void wrap(
        Boid& boid);

    float distance(
        const Boid& a,
        const Boid& b);

    Vec2 separation(
        size_t index);

    Vec2 alignment(
        size_t index);

    Vec2 cohesion(
        size_t index);
};

int m_numBoids = 600;
int m_numGroups = 3;

int m_numPredators = 0;

struct Color
{
    float r;
    float g;
    float b;
};

std::vector<Color>
m_groupColors =
{
    {1.f,0.4f,0.4f},
    {0.4f,1.f,0.4f},
    {0.4f,0.6f,1.f},
    {1.f,1.f,0.4f},
    {1.f,0.4f,1.f}
};