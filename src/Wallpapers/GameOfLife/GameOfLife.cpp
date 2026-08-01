#include "Wallpapers/GameOfLife/GameOfLife.h"
#include "Core/Renderer.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <X11/Xlib.h>
#include <X11/keysym.h>

BrushMode m_brushMode = BrushMode::Random;

GameOfLife::GameOfLife()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

bool GameOfLife::initialize(Renderer& renderer)
{
        renderer.setBackgroundColor(
        {
            0,0,0,.5
        }
    );
    (void)renderer;

    randomize();

    return true;
}

void GameOfLife::shutdown()
{
    m_cells.clear();
    m_nextCells.clear();
}

void GameOfLife::resize(
    int width,
    int height)
{
    m_width = width / m_cellSize;
    m_height = height / m_cellSize;

    if(m_width < 1)
        m_width = 1;

    if(m_height < 1)
        m_height = 1;

    m_cells.resize(
        m_width * m_height);

    m_nextCells.resize(
        m_width * m_height);

    randomize();
}

int GameOfLife::index(
    int x,
    int y) const
{
    return y * m_width + x;
}

bool GameOfLife::inside(
    int x,
    int y) const
{
    return x >= 0 &&
           y >= 0 &&
           x < m_width &&
           y < m_height;
}

Cell& GameOfLife::cell(
    int x,
    int y)
{
    return m_cells[index(x,y)];
}

const Cell& GameOfLife::cell(
    int x,
    int y) const
{
    return m_cells[index(x,y)];
}

void GameOfLife::clear()
{
    for(auto& c : m_cells)
    {
        c.alive = false;
        c.alpha = 0.0f;
        c.scale = 0.0f;
        c.age = 0;
    }

    m_nextCells = m_cells;
}

void GameOfLife::randomize()
{
    clear();

    for(auto& c : m_cells)
    {
        c.alive =
            (std::rand() % 5) == 0;

        c.age = 0;

        if(c.alive)
        {
            c.alpha = 0.0f;
            c.scale = 0.0f;
        }
    }

    m_nextCells = m_cells;
}

void GameOfLife::update(float dt)
{
    m_simTimer += dt;

    if(m_simTimer >= m_simInterval)
    {
        m_simTimer -= m_simInterval;

        updateSimulation();
    }

    updateAnimation(dt);
}

void GameOfLife::updateSimulation()
{
    for(int y=0; y<m_height; y++)
    {
        for(int x=0; x<m_width; x++)
        {
            int neighbours = 0;

            for(int yy=-1; yy<=1; yy++)
            {
                for(int xx=-1; xx<=1; xx++)
                {
                    if(xx==0 && yy==0)
                        continue;

                    int nx =
                        (x + xx + m_width) % m_width;

                    int ny =
                        (y + yy + m_height) % m_height;

                    if(cell(nx,ny).alive)
                        neighbours++;
                }
            }

            Cell next = cell(x,y);

            if(cell(x,y).alive)
            {
                next.alive =
                    neighbours == 2 ||
                    neighbours == 3;

                if(next.alive)
                    next.age++;
                else
                    next.age = 0;
            }
            else
            {
                next.alive =
                    neighbours == 3;

                next.age = 0;
            }

            m_nextCells[index(x,y)] = next;
        }
    }

    m_cells.swap(m_nextCells);
}

void GameOfLife::updateAnimation(float dt)
{
    const float fadeIn  = 7.0f * dt;
    const float fadeOut = 3.0f * dt;

    for(auto& c : m_cells)
    {
        if(c.alive)
        {
            c.alpha += fadeIn;
            c.scale += fadeIn;

            if(c.alpha > 1.0f)
                c.alpha = 1.0f;

            if(c.scale > 1.0f)
                c.scale = 1.0f;
        }
        else
        {
            c.alpha -= fadeOut;
            c.scale -= fadeOut;

            if(c.alpha < 0.0f)
                c.alpha = 0.0f;

            if(c.scale < 0.0f)
                c.scale = 0.0f;
        }
    }
}

void GameOfLife::render(Renderer& renderer)
{
    const float cellSize =
        static_cast<float>(m_cellSize);
    // renderer.
    //---------------------------------------
    // Grid
    //---------------------------------------

    for(int x = 0; x <= m_width; x++)
    {
        renderer.drawLine(
            x * cellSize,
            0,
            x * cellSize,
            m_height * cellSize,
            1,1,1,0.04f);
    }

    for(int y = 0; y <= m_height; y++)
    {
        renderer.drawLine(
            0,
            y * cellSize,
            m_width * cellSize,
            y * cellSize,
            1,1,1,0.04f);
    }

    //---------------------------------------
    // Cells
    //---------------------------------------

    for(int y = 0; y < m_height; y++)
    {
        for(int x = 0; x < m_width; x++)
        {
            const Cell& c = cell(x,y);

            if(c.alpha <= 0.01f)
                continue;

            float size =
                cellSize * c.scale;

            float px =
                x * cellSize +
                (cellSize-size)*0.5f;

            float py =
                y * cellSize +
                (cellSize-size)*0.5f;

            float t =
                std::min(
                    c.age / 25.0f,
                    1.0f);

            float r =
                0.2f + t * 0.3f;

            float g =
                0.55f + t * 0.25f;

            float b = 1.0f;

            renderer.drawRectangle(
                px,
                py,
                size,
                size,
                r,
                g,
                b,
                c.alpha);
        }
    }

    //---------------------------------------
    // Selection rectangle
    //---------------------------------------

    if(m_dragging)
    {
        int x0 =
            std::min(
                m_dragStartX,
                m_dragEndX);

        int y0 =
            std::min(
                m_dragStartY,
                m_dragEndY);

        int x1 =
            std::max(
                m_dragStartX,
                m_dragEndX);

        int y1 =
            std::max(
                m_dragStartY,
                m_dragEndY);

        renderer.drawRectangleOutline(
            x0 * m_cellSize,
            y0 * m_cellSize,
            (x1-x0+1)*m_cellSize,
            (y1-y0+1)*m_cellSize,
            1,1,1,1);
    }
}

void GameOfLife::paintRectangle(
    int x0,
    int y0,
    int x1,
    int y1,
    BrushMode mode)
{
    if(x0 > x1)
        std::swap(x0,x1);

    if(y0 > y1)
        std::swap(y0,y1);

    for(int y = y0; y <= y1; y++)
    {
        for(int x = x0; x <= x1; x++)
        {
            if(!inside(x,y))
                continue;

            Cell& c = cell(x,y);

            switch(mode)
            {
                case BrushMode::Random:
                {
                    c.alive = rand() & 1;

                    if(c.alive)
                    {
                        c.alpha = 1.0f;
                        c.scale = 1.0f;
                        c.age = 0;
                    }
                    else
                    {
                        c.alpha = 0.0f;
                        c.scale = 0.0f;
                        c.age = 0;
                    }

                    break;
                }

                case BrushMode::Clear:
                {
                    c.alive = false;
                    c.alpha = 0.0f;
                    c.scale = 0.0f;
                    c.age = 0;

                    break;
                }
            }
        }
    }
}

int GameOfLife::pixelToGridX(
    int x) const
{
    return x / m_cellSize;
}

int GameOfLife::pixelToGridY(
    int y) const
{
    return y / m_cellSize;
}

void GameOfLife::onEvent(
    const Event& e)
{
    switch(e.type)
    {
        case Event::Type::MouseButtonPress:
        {
            mousePress(
                e.button,
                e.x,
                e.y);

            break;
        }

        case Event::Type::MouseMotion:
        {
            mouseMove(
                e.x,
                e.y);

            break;
        }

        case Event::Type::MouseButtonRelease:
        {
            mouseRelease(
                e.button,
                e.x,
                e.y);

            break;
        }

        case Event::Type::KeyboardPress:
        {
            keyPress(
                static_cast<KeySym>(e.key));

            break;
        }

        // case Event::Type::KeyboardRelease:
        // {
        //     keyRelease(
        //         static_cast<KeySym>(e.key));

        //     break;
        // }

        case Event::Type::WindowResize:
        {
            resize(
                e.width,
                e.height);

            break;
        }

        default:
        {
            break;
        }
    }
}

void GameOfLife::keyPress(KeySym key)
{
    switch(key)
    {
        case XK_r:

            randomize();

            break;

        case XK_c:

            clear();

            break;

        default:
            break;
    }
}

void GameOfLife::mousePress(
    int button,
    int x,
    int y)
{
    switch(button)
    {
        case Button1:

            m_brushMode = BrushMode::Random;

            break;

        case Button3:

            m_brushMode = BrushMode::Clear;

            break;

        case Button2:

            randomize();

            return;

        default:

            return;
    }

    m_dragging = true;

    m_dragStartX = pixelToGridX(x);
    m_dragStartY = pixelToGridY(y);

    m_dragEndX = m_dragStartX;
    m_dragEndY = m_dragStartY;
}

void GameOfLife::mouseMove(
    int x,
    int y)
{
    if(!m_dragging)
        return;

    m_dragEndX = pixelToGridX(x);
    m_dragEndY = pixelToGridY(y);
}

void GameOfLife::mouseRelease(
    int button,
    int,
    int)
{
    if(button != Button1 &&
       button != Button3)
        return;

    m_dragging = false;

    paintRectangle(
        m_dragStartX,
        m_dragStartY,
        m_dragEndX,
        m_dragEndY,
        m_brushMode);
}

// #include "Core/RegisterWallpaper.h"
// LIVEWALL_REGISTER_DEFAULT_WALLPAPER(
//     GameOfLife)

