#pragma once

#include "Core/Wallpaper.h"
#include "Core/Renderer.h"

#include <cmath>
#include <cstdint>

namespace
{
    typedef struct
    {
        float x;
        float y;
    } cpoint_t;

    typedef struct
    {
        uint64_t user;
        uint64_t nice;
        uint64_t system;
        uint64_t idle;
        uint64_t iowait;
        uint64_t irq;
        uint64_t softirq;
        uint64_t steal;
        uint64_t guest;
        uint64_t guest_nice;
    } CPUState;

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

color_t rgb2clr(
        uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a);

class CoreStats : public Wallpaper
{
public:
    bool initialize(Renderer&) override;
    void update(float dt) override;
    void render(Renderer& renderer) override;

    void cicularDial(
            dial_cfg_t& cfg,
            Renderer& renderer);

private:

    float m_cpu = 0.f;
    float m_ram = 0.f;
    float m_gpu = 0.f;

    CPUState m_prevCpu{};

    color_t Blue{0.32f,0.58f,0.88f,1.f};
    color_t Gray{0.48f,0.50f,0.54f,1.f};

    dial_cfg_t cpu_dial{
        {500,300},
        90.f,
        110.f,
        Blue,
        Gray,
        3.f,
        -2 * M_PI,
        0.f,
        0.f,
        100.f,
        0.f
    };

    dial_cfg_t ram_dial{
        {500,600},
        90.f,
        110.f,
        Blue,
        Gray,
        3.f,
        -2 * M_PI,
        0.f,
        0.f,
        100.f,
        0.f
    };

    dial_cfg_t gpu_dial{
        {500,900},
        90.f,
        110.f,
        Blue,
        Gray,
        3.f,
        -2 * M_PI,
        0.f,
        0.f,
        100.f,
        0.f
    };
};