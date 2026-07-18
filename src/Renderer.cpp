#include "Renderer.h"

#include <GL/gl.h>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::initialize()
{
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Renderer::shutdown()
{
}

void Renderer::resize(
    int width,
    int height)
{
    m_width = width;
    m_height = height;

    glViewport(
        0,
        0,
        width,
        height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        0,
        width,
        height,
        0,
        -1,
        1);

    glMatrixMode(GL_MODELVIEW);
}

void Renderer::beginFrame()
{
    glClearColor(
        0.05f,
        0.05f,
        0.08f,
        1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
}

void Renderer::endFrame()
{
}

void Renderer::drawRectangle(
    float x,
    float y,
    float width,
    float height,
    float r,
    float g,
    float b,
    float a)
{
    glColor4f(r,g,b,a);

    glBegin(GL_QUADS);

    glVertex2f(x,y);

    glVertex2f(x+width,y);

    glVertex2f(x+width,y+height);

    glVertex2f(x,y+height);

    glEnd();
}

void Renderer::drawLine(
    float x1,
    float y1,
    float x2,
    float y2,
    float r,
    float g,
    float b,
    float a)
{
    glColor4f(r,g,b,a);

    glBegin(GL_LINES);

    glVertex2f(x1,y1);

    glVertex2f(x2,y2);

    glEnd();
}

int Renderer::width() const
{
    return m_width;
}

int Renderer::height() const
{
    return m_height;
}

void Renderer::drawRectangleOutline(
    float x,
    float y,
    float w,
    float h,
    float r,
    float g,
    float b,
    float a)
{
    glColor4f(r,g,b,a);

    glBegin(GL_LINE_LOOP);

    glVertex2f(x,y);
    glVertex2f(x+w,y);
    glVertex2f(x+w,y+h);
    glVertex2f(x,y+h);

    glEnd();
}

void Renderer::drawTriangle(
    float x1,
    float y1,
    float x2,
    float y2,
    float x3,
    float y3,
    float r,
    float g,
    float b,
    float a)
{
    glColor4f(r, g, b, a);

    glBegin(GL_TRIANGLES);

    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);

    glEnd();
}