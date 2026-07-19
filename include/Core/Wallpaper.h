#pragma once

#include "Core/Event.h"

class Renderer;

class Wallpaper
{
public:

    virtual ~Wallpaper() = default;

    virtual bool initialize(
        Renderer& renderer)
    {
        (void)renderer;
        return true;
    }

    virtual void shutdown()
    {
    }

    virtual void resize(
        int width,
        int height)
    {
        (void)width;
        (void)height;
    }

    virtual void update(
        float dt) = 0;

    virtual void render(
        Renderer& renderer) = 0;

    virtual void onEvent(
        const Event& event)
    {
        (void)event;
    }
};