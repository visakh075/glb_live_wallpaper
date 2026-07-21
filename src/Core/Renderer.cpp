#include "Core/Renderer.h"

#include <GL/gl.h>
#include <cmath>
#include <ft2build.h>
#include FT_FREETYPE_H

Renderer::Renderer(){}
Renderer::~Renderer(){ unloadFont(); }

bool Renderer::initialize()
{
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Renderer::shutdown(){}

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


bool Renderer::loadFont(
    const std::string& path,
    int pixelSize)
{
    unloadFont();

    FT_Library ft;
    if(FT_Init_FreeType(&ft))
        return false;

    FT_Face face;
    if(FT_New_Face(ft,path.c_str(),0,&face))
    {
        FT_Done_FreeType(ft);
        return false;
    }

    FT_Set_Pixel_Sizes(face,0,pixelSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    for(unsigned char c=0;c<128;c++)
    {
        if(FT_Load_Char(face,c,FT_LOAD_RENDER))
            continue;

        GLuint tex;
        glGenTextures(1,&tex);
        glBindTexture(GL_TEXTURE_2D,tex);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

        Glyph g;
        g.texture=tex;
        g.width=face->glyph->bitmap.width;
        g.height=face->glyph->bitmap.rows;
        g.bearingX=face->glyph->bitmap_left;
        g.bearingY=face->glyph->bitmap_top;
        g.advance=face->glyph->advance.x;

        m_fontGlyphs[c]=g;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    m_fontLoaded=true;
    return true;
}

void Renderer::unloadFont()
{
    for(auto& [c,g] : m_fontGlyphs)
        if(g.texture)
            glDeleteTextures(1,&g.texture);

    m_fontGlyphs.clear();
    m_fontLoaded=false;
}

void Renderer::drawText(
    float x,float y,
    const std::string& text,
    float r,float g,float b,
    float a)
{
    if(!m_fontLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glColor4f(r,g,b,a);

    for(char c : text)
    {
        auto it=m_fontGlyphs.find(c);
        if(it==m_fontGlyphs.end())
            continue;

        auto& g=it->second;

        float xpos=x+g.bearingX;
        float ypos=y-g.bearingY;

        float w=g.width;
        float h=g.height;

        glBindTexture(GL_TEXTURE_2D,g.texture);

        glBegin(GL_QUADS);

        glTexCoord2f(0,0); glVertex2f(xpos,ypos);
        glTexCoord2f(1,0); glVertex2f(xpos+w,ypos);
        glTexCoord2f(1,1); glVertex2f(xpos+w,ypos+h);
        glTexCoord2f(0,1); glVertex2f(xpos,ypos+h);

        glEnd();

        x += (g.advance >> 6);
    }

    glDisable(GL_TEXTURE_2D);
}

void Renderer::cdrawText(
    float x,
    float y,
    const std::string& text,
    float r,
    float g,
    float b,
    float a)
{
    if(!m_fontLoaded)
        return;

    float width = 0.f;

    float ascent = 0.f;
    float descent = 0.f;

    //
    // Measure text bounds
    //
    for(char c : text)
    {
        auto it =
            m_fontGlyphs.find(c);

        if(it ==
           m_fontGlyphs.end())
        {
            continue;
        }

        auto& glyph =
            it->second;

        width +=
            (glyph.advance >> 6);

        if(glyph.bearingY >
           ascent)
        {
            ascent =
                glyph.bearingY;
        }

        float d =
            glyph.height -
            glyph.bearingY;

        if(d >
           descent)
        {
            descent = d;
        }
    }

    float height =
        ascent +
        descent;

    //
    // Convert center
    // position into
    // baseline position
    //
    x -= width * 0.5f;

    y += ascent;
    y -= height * 0.5f;

    glEnable(
        GL_TEXTURE_2D);

    glColor4f(
        r,
        g,
        b,
        a);

    for(char c : text)
    {
        auto it =
            m_fontGlyphs.find(c);

        if(it ==
           m_fontGlyphs.end())
        {
            continue;
        }

        auto& glyph =
            it->second;

        float xpos =
            x +
            glyph.bearingX;

        float ypos =
            y -
            glyph.bearingY;

        float w =
            glyph.width;

        float h =
            glyph.height;

        glBindTexture(
            GL_TEXTURE_2D,
            glyph.texture);

        glBegin(
            GL_QUADS);

        glTexCoord2f(
            0.f,
            0.f);
        glVertex2f(
            xpos,
            ypos);

        glTexCoord2f(
            1.f,
            0.f);
        glVertex2f(
            xpos + w,
            ypos);

        glTexCoord2f(
            1.f,
            1.f);
        glVertex2f(
            xpos + w,
            ypos + h);

        glTexCoord2f(
            0.f,
            1.f);
        glVertex2f(
            xpos,
            ypos + h);

        glEnd();

        x +=
            (glyph.advance >> 6);
    }

    glDisable(
        GL_TEXTURE_2D);
}

void Renderer::drawArcDial(
    float cx,
    float cy,
    float innerRadius,
    float outerRadius,
    float startAngle,
    float endAngle,
    float r,
    float g,
    float b,
    float a)
{
    constexpr int segments = 360;

    glColor4f(r, g, b, a);

    glBegin(GL_TRIANGLE_STRIP);

    for(int i = 0;
        i <= segments;
        ++i)
    {
        float t =
            float(i) /
            float(segments);

        float ang =
            startAngle +
            t *
            (endAngle -
             startAngle);

        float cs = cosf(ang);
        float sn = sinf(ang);

        //
        // outer vertex
        //
        glVertex2f(
            cx +
            outerRadius * cs,
            cy +
            outerRadius * sn);

        //
        // inner vertex
        //
        glVertex2f(
            cx +
            innerRadius * cs,
            cy +
            innerRadius * sn);
    }

    glEnd();
}
