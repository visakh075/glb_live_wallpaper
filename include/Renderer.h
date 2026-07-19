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
    // Filled Primitives
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

    void drawCircle(
        float x,
        float y,
        float radius,
        float r,
        float g,
        float b,
        float a = 1.0f,
        int segments = 32);

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

    void drawTriangleRotated(
        float x,
        float y,
        float angle,
        float size,
        float r,
        float g,
        float b,
        float a = 1.0f);

    //------------------------------------
    // Outlines
    //------------------------------------

    void drawLine(
        float x1,
        float y1,
        float x2,
        float y2,
        float r,
        float g,
        float b,
        float a = 1.0f);

    void drawRectangleOutline(
        float x,
        float y,
        float width,
        float height,
        float r,
        float g,
        float b,
        float a = 1.0f);

    void drawCircleOutline(
        float x,
        float y,
        float radius,
        float r,
        float g,
        float b,
        float a = 1.0f,
        int segments = 32);

    //------------------------------------
    // Debug
    //------------------------------------

    void drawArrow(
        float x,
        float y,
        float angle,
        float length,
        float r,
        float g,
        float b,
        float a = 1.0f);

    //------------------------------------
    // State
    //------------------------------------

    int width() const;
    int height() const;

    //------------------------------------
    // Transform
    //------------------------------------

    void pushMatrix();

    void popMatrix();

    void translate(
        float x,
        float y);

    void rotate(
        float angleDegrees);

    void scale(
        float x,
        float y);

private:

    int m_width = 0;
    int m_height = 0;
};