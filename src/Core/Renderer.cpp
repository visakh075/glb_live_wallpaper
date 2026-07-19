#include "Core/Renderer.h"

#include <GL/gl.h>
#include <cmath>

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
#include <cmath>

void Renderer::drawCircle(
    float x,
    float y,
    float radius,
    float r,
    float g,
    float b,
    float a,
    int segments)
{
    glColor4f(r,g,b,a);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x,y);

    for(int i=0;i<=segments;i++)
    {
        float angle =
            (2.0f * M_PI * i) / segments;

        glVertex2f(
            x + std::cos(angle) * radius,
            y + std::sin(angle) * radius);
    }

    glEnd();
}

void Renderer::drawCircleOutline(
    float x,
    float y,
    float radius,
    float r,
    float g,
    float b,
    float a,
    int segments)
{
    glColor4f(r,g,b,a);

    glBegin(GL_LINE_LOOP);

    for(int i=0;i<segments;i++)
    {
        float angle =
            (2.0f * M_PI * i) / segments;

        glVertex2f(
            x + std::cos(angle) * radius,
            y + std::sin(angle) * radius);
    }

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
    glColor4f(r,g,b,a);

    glBegin(GL_TRIANGLES);

    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glVertex2f(x3,y3);

    glEnd();
}

void Renderer::drawTriangleRotated(
    float x,
    float y,
    float angle,
    float size,
    float r,
    float g,
    float b,
    float a)
{
    float c = std::cos(angle);
    float s = std::sin(angle);

    float x1 = x + c * size;
    float y1 = y + s * size;

    float x2 =
        x - c * size * 0.5f
          - s * size * 0.5f;

    float y2 =
        y - s * size * 0.5f
          + c * size * 0.5f;

    float x3 =
        x - c * size * 0.5f
          + s * size * 0.5f;

    float y3 =
        y - s * size * 0.5f
          - c * size * 0.5f;

    drawTriangle(
        x1,y1,
        x2,y2,
        x3,y3,
        r,g,b,a);
}

void Renderer::drawArrow(
    float x,
    float y,
    float angle,
    float length,
    float r,
    float g,
    float b,
    float a)
{
    float ex =
        x + std::cos(angle) * length;

    float ey =
        y + std::sin(angle) * length;

    drawLine(
        x,y,
        ex,ey,
        r,g,b,a);

    float head = length * 0.25f;

    drawLine(
        ex,
        ey,
        ex + std::cos(angle + 2.6f) * head,
        ey + std::sin(angle + 2.6f) * head,
        r,g,b,a);

    drawLine(
        ex,
        ey,
        ex + std::cos(angle - 2.6f) * head,
        ey + std::sin(angle - 2.6f) * head,
        r,g,b,a);
}

void Renderer::pushMatrix()
{
    glPushMatrix();
}

void Renderer::popMatrix()
{
    glPopMatrix();
}

void Renderer::translate(
    float x,
    float y)
{
    glTranslatef(
        x,
        y,
        0.0f);
}

void Renderer::rotate(
    float angleDegrees)
{
    glRotatef(
        angleDegrees,
        0.0f,
        0.0f,
        1.0f);
}

void Renderer::scale(
    float x,
    float y)
{
    glScalef(
        x,
        y,
        1.0f);
}