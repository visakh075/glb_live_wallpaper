#include "Wallpapers/Clock/ClockWallpaper.h"
#include "Core/Renderer.h"
#include <ctime>
#include <cmath>

bool ClockWallpaper::initialize(Renderer&renderer)
{


    rad_out = 500;
    rad_in = 450;
    rad_sec = 440;

    center =
    {
        (renderer.width() / 2.0f) - 700,
        renderer.height() / 2.0f
    };    
    // /usr/share/fonts/truetype/noto/NotoSerif-Regular.ttf
    if(!renderer.loadFont(
            "/usr/local/share/fonts/Hermit-Bold.otf",
            48))
    {
        printf("Font load failed\n");
    }
    else
    {
        printf("Font loaded\n");
    }

    return true;

    return true;
}

void ClockWallpaper::update(float)
{
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);

    m_second = float(tm->tm_sec);
    m_minute = float(tm->tm_min) + m_second / 60.f;
    m_hour = float(tm->tm_hour % 12) + m_minute / 60.f;
    m_hour24 = float(tm->tm_hour);
}


void ClockWallpaper::circleDrawLine(
    cpoint_t p1,
    cpoint_t p2,
    color_t color,
    Renderer& renderer)
{
    renderer.drawLine(
        p1.x, p1.y,
        p2.x, p2.y,
        color.r,
        color.g,
        color.b,
        color.a
    );
}

void ClockWallpaper::cirDial(
    cpoint_t center,
    float r1,
    float r2,
    float ang,
    color_t color,
    Renderer& renderer)
{
    cpoint_t p1 {
        center.x + std::cos(ang) * r1,
        center.y + std::sin(ang) * r1
    };

    cpoint_t p2 {
        center.x + std::cos(ang) * r2,
        center.y + std::sin(ang) * r2
    };

    circleDrawLine(p1, p2, color, renderer);
}

void ClockWallpaper::circleOutline(cpoint_t center,float r,color_t color,Renderer& renderer)
{
    renderer.drawCircleOutline(
        center.x,
        center.y,
        r,
        color.r,
        color.g,
        color.b,
        color.a,
        360
    );
}

void ClockWallpaper::circle(cpoint_t center,float r,color_t color,Renderer& renderer)
{
    renderer.drawCircle(
        center.x,
        center.y,
        r,
        color.r,
        color.g,
        color.b,
        color.a,
        360
    );
}


void ClockWallpaper::render(Renderer& renderer)
{
    color_t white {1,1,1,1};
    color_t red   {1,0,0,1};
    color_t blue  {0,0,1,0.5};

    renderer.drawCircleOutline(
        center.x,
        center.y,
        rad_out,
        white.r,
        white.g,
        white.b,
        white.a,
        360);

    // Hour marks
    for(int i = 0; i < 12; i++)
    {
        float a =
            i * 2.0f * M_PI / 12.0f - M_PI/2.0f;

        cirDial(center,
                rad_out,
                rad_in,
                a,
                white,
                renderer);
    }

    // Minute marks
    for(int i = 0; i < 60; i++)
    {
        if(i % 5 == 0)
            continue;

        float a =
            i * 2.0f * M_PI / 60.0f - M_PI/2.0f;

        cirDial(center,
                rad_out,
                rad_out - 20,
                a,
                white,
                renderer);
    }

    // Hour hand
    float hAng =
        m_hour * 2.0f * M_PI / 12.0f - M_PI/2.0f;

    cirDial(center,250,15,hAng,white,renderer);
    circleOutline(center,15,white,renderer);

    // Minute hand
    
    float mAng =
        m_minute * 2.0f * M_PI / 60.0f - M_PI/2.0f;

    cirDial(center,350,12,mAng,white,renderer);
    circle(center,12,white,renderer);


    // Second hand
    float sAng =
        m_second * 2.0f * M_PI / 60.0f - M_PI/2.0f;

    cirDial(center,400,10,sAng,red,renderer);
    circle(center,10,blue,renderer);

    char timeStr[32];

    std::snprintf(
        timeStr,
        sizeof(timeStr),
        "%02d:%02d:%02d",
        (int)m_hour24,
        (int)m_minute,
        (int)m_second);
        
    
    renderer.drawText(
        center.x - 100,
        center.y + 80,
        timeStr,
        1,
        1,
        1,
        1);
        
}

#include "Core/RegisterWallpaper.h"
LIVEWALL_REGISTER_DEFAULT_WALLPAPER(
    ClockWallpaper)