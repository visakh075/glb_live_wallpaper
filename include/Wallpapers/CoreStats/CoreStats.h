#pragma once

#include "Core/Wallpaper.h"
#include "Core/Renderer.h"
#include <cmath>
#include <stdint.h>
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

    typedef struct
    {
        cpoint_t center;
        float in_rad;
        float out_rad;
        color_t fill_c;
        color_t border_c;
        float sep;
        
        float a1;
        float a2;

        float min_val;
        float max_val;

        float val;
    } dial_cfg_t;

}
color_t rgb2clr(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
class CoreStats : public Wallpaper
{
public:
    bool initialize(Renderer&) override;
    void update(float dt) override;
    void render(Renderer& renderer) override;
    void cicularDial(dial_cfg_t&cfg , Renderer&renderer);
    
    float m;
private:
    float m_second = 0.f;
    float m_minute = 0.f;
    float m_hour = 0.f;
    color_t Blue {0.32,0.58,0.88,1};
    // color_t Back =
    dial_cfg_t time_dial{
        cpoint_t{
            500,500
        },
        90.f,
        100.f,
        Blue,
        color_t {0.48,0.50,0.54,1},
        1,
        0,
         -2 * M_PI,
        0,
        60,
        0
    };
};