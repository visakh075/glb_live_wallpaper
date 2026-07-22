#include "Wallpapers/CoreStats/CoreStats.h"
#include "Core/Renderer.h"

#include <fstream>
#include <string>
#include <cstdio>
#include <thread>
#include <chrono>

namespace
{

CPUState ReadCPUState()
{
    std::ifstream file("/proc/stat");

    std::string cpu;
    CPUState s{};

    file >> cpu
         >> s.user
         >> s.nice
         >> s.system
         >> s.idle
         >> s.iowait
         >> s.irq
         >> s.softirq
         >> s.steal
         >> s.guest
         >> s.guest_nice;

    return s;
}

float GetCPUUsage(CPUState& prev)
{
    CPUState cur = ReadCPUState();

    uint64_t prevIdle =
        prev.idle +
        prev.iowait;

    uint64_t idle =
        cur.idle +
        cur.iowait;

    uint64_t prevTotal =
        prev.user +
        prev.nice +
        prev.system +
        prev.idle +
        prev.iowait +
        prev.irq +
        prev.softirq +
        prev.steal;

    uint64_t total =
        cur.user +
        cur.nice +
        cur.system +
        cur.idle +
        cur.iowait +
        cur.irq +
        cur.softirq +
        cur.steal;

    uint64_t totald =
        total -
        prevTotal;

    uint64_t idled =
        idle -
        prevIdle;

    prev = cur;

    if(totald == 0)
        return 0.f;

    return
        (float)(totald - idled)
        * 100.f
        / (float)totald;
}

float GetRAMUsage()
{
    std::ifstream file("/proc/meminfo");

    std::string key;
    std::string unit;
    uint64_t value;

    uint64_t total = 0;
    uint64_t available = 0;

    while(file >> key >> value >> unit)
    {
        if(key == "MemTotal:")
            total = value;

        else if(key == "MemAvailable:")
            available = value;
    }

    if(total == 0)
        return 0.f;

    return
        (total - available)
        * 100.f
        / total;
}

float GetGPUUsage()
{
    FILE* fp =
        popen(
            "nvidia-smi "
            "--query-gpu=utilization.gpu "
            "--format=csv,noheader,nounits",
            "r");

    if(!fp)
        return 0.f;

    float gpu = 0.f;

    fscanf(fp, "%f", &gpu);

    pclose(fp);

    return gpu;
}

}

color_t rgb2clr(
        uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a)
{
    return {
        r / 255.f,
        g / 255.f,
        b / 255.f,
        a / 255.f
    };
}

bool CoreStats::initialize(Renderer& renderer)
{
    renderer.loadFont(
        "/usr/local/share/fonts/Hermit-Bold.otf",
        80);

    m_prevCpu =
        ReadCPUState();

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    return true;
}

void CoreStats::update(float dt)
{
    static float timer = 0.f;

    timer += dt;

    if(timer >= 1.0f)
    {
        timer = 0.f;

        float cpu =
            GetCPUUsage(
                m_prevCpu);

        float ram =
            GetRAMUsage();

        float gpu =
            GetGPUUsage();

        // smoothing
        m_cpu =
            m_cpu * 0.8f +
            cpu * 0.2f;

        m_ram =
            m_ram * 0.8f +
            ram * 0.2f;

        m_gpu =
            m_gpu * 0.8f +
            gpu * 0.2f;

        // printf(
        //     "CPU %.1f "
        //     "RAM %.1f "
        //     "GPU %.1f\n",
        //     m_cpu,
        //     m_ram,
        //     m_gpu);
    }

    cpu_dial.val = m_cpu;
    ram_dial.val = m_ram;
    gpu_dial.val = m_gpu;
}

void CoreStats::cicularDial(
        dial_cfg_t& dial_cfg,
        Renderer& renderer)
{
    float ratio =
        dial_cfg.val /
        (dial_cfg.max_val -
         dial_cfg.min_val);

    float angle =
        (dial_cfg.a2 -
         dial_cfg.a1)
        * ratio;

    renderer.drawArcDial(
        dial_cfg.center.x,
        dial_cfg.center.y,
        dial_cfg.in_rad -
            dial_cfg.sep,
        dial_cfg.out_rad +
            dial_cfg.sep,
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

void CoreStats::render(
        Renderer& renderer)
{
    cicularDial(
        cpu_dial,
        renderer);

    cicularDial(
        ram_dial,
        renderer);

    cicularDial(
        gpu_dial,
        renderer);

    char str[64];

    std::snprintf(
        str,
        sizeof(str),
        "CPU %05.1f%%",
        m_cpu);

    renderer.cdrawText(
        cpu_dial.center.x,
        cpu_dial.center.y,
        str,
        Blue.r,
        Blue.g,
        Blue.b,
        Blue.a);

    std::snprintf(
        str,
        sizeof(str),
        "RAM %05.1f%%",
        m_ram);

    renderer.cdrawText(
        ram_dial.center.x,
        ram_dial.center.y,
        str,
        Blue.r,
        Blue.g,
        Blue.b,
        Blue.a);

    std::snprintf(
        str,
        sizeof(str),
        "GPU %05.1f%%",
        m_gpu);

    renderer.cdrawText(
        gpu_dial.center.x,
        gpu_dial.center.y,
        str,
        Blue.r,
        Blue.g,
        Blue.b,
        Blue.a);
}

// #include "Core/RegisterWallpaper.h"

// LIVEWALL_REGISTER_DEFAULT_WALLPAPER(
//     CoreStats)