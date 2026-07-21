#include "Core/Renderer.h"

#include "Wallpapers/CoreStats/CoreStats.h"
#include "Core/Renderer.h"
#include <cstdint>
#include <ctime>
#include <cmath>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <thread>

#include <stdint.h>

namespace
{
    struct CPUState {
    size_t user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    };

    CPUState ReadCPUState() {
        std::ifstream file("/proc/stat");
        std::string cpu;
        CPUState state{};
        if (file >> cpu >> state.user >> state.nice >> state.system >> state.idle 
                >> state.iowait >> state.irq >> state.softirq >> state.steal 
                >> state.guest >> state.guest_nice) {
            return state;
        }
        return {};
    }
}
color_t rgb2clr(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{   
    float rf , gf, bf , af;
    rf = r/255;
    gf = g/255;
    bf = b/255;
    af = a/255;
    return color_t {rf,gf,bf,af};
}
bool CoreStats::initialize(Renderer&renderer)
{
    
    // /usr/share/fonts/truetype/noto/NotoSerif-Regular.ttf
    if(!renderer.loadFont(
            "/usr/local/share/fonts/Hermit-Bold.otf",
            100))
    {
        printf("Font load failed\n");
    }
    else
    {
        printf("Font loaded\n");
    }
    
    return true;
}

void CoreStats::update(float)
{
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);

    m_second = float(tm->tm_sec);

    time_dial.val = m_second;

}
void CoreStats::cicularDial(dial_cfg_t &dial_cfg,Renderer& renderer)
{
    float val_ratio = (dial_cfg.val / (dial_cfg.max_val - dial_cfg.min_val));
    float angle = ((dial_cfg.a2 - dial_cfg.a1) * val_ratio);

    renderer.drawArcDial(
        dial_cfg.center.x,
        dial_cfg.center.y,

        dial_cfg.in_rad - dial_cfg.sep,
        dial_cfg.out_rad + dial_cfg.sep,

        dial_cfg.a1,
        dial_cfg.a2,
        dial_cfg.border_c.r,
        dial_cfg.border_c.g,
        dial_cfg.border_c.b,
        dial_cfg.border_c.a);

    renderer.drawArcDial(
        dial_cfg.center.x,
        dial_cfg.center.y,
        dial_cfg.in_rad,
        dial_cfg.out_rad,
        dial_cfg.a1,
        dial_cfg.a1 + angle,
        dial_cfg.fill_c.r,
        dial_cfg.fill_c.g,
        dial_cfg.fill_c.b,
        dial_cfg.fill_c.a);
    
}
void CoreStats::render(Renderer& renderer)
{
    cicularDial(time_dial,renderer);

    char timeStr[32];

    std::snprintf(
    timeStr,
    sizeof(timeStr),
    "%02d",
    (int)m_second);
    
    renderer.cdrawText(
        time_dial.center.x,
        time_dial.center.y,
        timeStr,
        Blue.r,
        Blue.g,
        Blue.b,
        Blue.a    
    );
        
    
}

#include "Core/RegisterWallpaper.h"
LIVEWALL_REGISTER_DEFAULT_WALLPAPER(
    CoreStats)