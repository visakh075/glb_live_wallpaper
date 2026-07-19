#pragma once

class Renderer;
struct Event;

class Effect
{
public:

    virtual ~Effect(){}

    virtual bool initialize(Renderer&)
    {
        return true;
    }

    virtual void shutdown(){}

    virtual void resize(
        int,
        int)
    {
    }

    virtual void update(float)=0;

    virtual void render(Renderer&)=0;

    virtual void onEvent(
        const Event&)
    {
    }
};