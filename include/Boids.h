#pragma once

#include <vector>

#include "Effect.h"
#include "Boid.h"

class Renderer;

class Boids : public Effect
{
public:

    Boids();

    bool initialize(Renderer& renderer) override;

    void shutdown() override;

    void resize(
        int width,
        int height) override;

    void update(
        float dt) override;

    void render(
        Renderer& renderer) override;

    void mousePress(
        int button,
        int x,
        int y) override;

private:

    int m_width = 0;
    int m_height = 0;

    std::vector<Boid> m_boids;

    //--------------------------------

    void createBoids(
        int count);

    void updateBoids(
        float dt);

    //--------------------------------

    void separation(
        int i);

    void alignment(
        int i);

    void cohesion(
        int i);

    //--------------------------------

    void wrap(Boid& boid);

    void limitSpeed(Boid& boid);

    float distanceSquared(
        const Boid& a,
        const Boid& b) const;
};