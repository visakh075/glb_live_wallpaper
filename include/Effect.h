#pragma once

#include <X11/Xlib.h>
#include <X11/keysym.h>

class Renderer;

class Effect
{
public:

    virtual ~Effect() = default;

    virtual bool initialize(Renderer& renderer)
    {
        return true;
    }

    virtual void shutdown()
    {
    }

    virtual void resize(
        int width,
        int height) = 0;

    virtual void update(
        float dt) = 0;

    virtual void render(
        Renderer& renderer) = 0;

    virtual void keyPress(
        KeySym key)
    {
    }

    virtual void keyRelease(
        KeySym key)
    {
    }

    virtual void mousePress(
        int button,
        int x,
        int y)
    {
    }

    virtual void mouseRelease(
        int button,
        int x,
        int y)
    {
    }

    virtual void mouseMove(
        int x,
        int y)
    {
    };
};