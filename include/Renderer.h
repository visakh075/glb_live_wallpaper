#pragma once

class Renderer
{
public:

    Renderer();
    ~Renderer();

    bool initialize();

    void shutdown();

    void resize(
        int width,
        int height);

    void beginFrame();

    void endFrame();

    //------------------------------------
    // Primitive Drawing
    //------------------------------------

    void drawRectangle(
        float x,
        float y,
        float width,
        float height,
        float r,
        float g,
        float b,
        float a = 1.0f);

    void drawLine(
        float x1,
        float y1,
        float x2,
        float y2,
        float r,
        float g,
        float b,
        float a = 1.0f);

    //------------------------------------
    // State
    //------------------------------------

    int width() const;
    int height() const;

    void drawRectangleOutline(
    float x,
    float y,
    float w,
    float h,
    float r,
    float g,
    float b,
    float a);

    void drawTriangle(
    float x1,
    float y1,
    float x2,
    float y2,
    float x3,
    float y3,
    float r,
    float g,
    float b,
    float a = 1.0f);

private:

    int m_width = 0;
    int m_height = 0;
};

