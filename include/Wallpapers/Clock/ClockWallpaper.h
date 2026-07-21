#pragma once

#include "Core/Wallpaper.h"
#include "Core/Renderer.h"
namespace {
    typedef struct
    {
        float x;
        float y;
    } cpoint_t;

    typedef struct {
        float r;
        float g;
        float b;
        float a;
    } color_t;

    cpoint_t center{500,500};

    float rad_out;

    float rad_in;

    float rad_sec;

    float rad_min;

    float rad_hr;
}


class ClockWallpaper : public Wallpaper
{
public:
    bool initialize(Renderer&) override;
    void update(float dt) override;
    void render(Renderer& renderer) override;
    void cirDial(cpoint_t center, float r1,float r2, float ang,color_t color,Renderer& renderer);
    void circleDrawLine(cpoint_t p1,cpoint_t p2,color_t color,Renderer& renderer);
    void circleOutline(cpoint_t center,float r,color_t color,Renderer& renderer);
    void circle(cpoint_t center,float r,color_t color,Renderer& renderer);
    float m;
private:
    float m_second = 0.f;
    float m_minute = 0.f;
    float m_hour = 0.f;
};
