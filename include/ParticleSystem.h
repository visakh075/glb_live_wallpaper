#pragma once

#include <vector>

#include "Effect.h"
#include "Particle.h"

class Renderer;

class ParticleSystem : public Effect
{
public:

    bool initialize(Renderer& renderer) override;

    void shutdown() override;

    void resize(
        int width,
        int height) override;

    void update(
        float dt) override;

    void render(
        Renderer& renderer) override;

protected:

    int m_width = 0;
    int m_height = 0;

    std::vector<Particle> particles;

    virtual void spawn() = 0;

    virtual void updateParticle(
        Particle& particle,
        float dt);

    virtual void renderParticle(
        Renderer& renderer,
        const Particle& particle);
};