#include "GameOfLife.h"
#include "Renderer.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

GameOfLife::GameOfLife()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

bool GameOfLife::initialize(Renderer& renderer)
{
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

        if(c.alive)
        {
            c.alpha = 1.0f;
            c.scale = 1.0f;
            c.age = 0;
        }
    }

    m_nextCells = m_cells;
}
void GameOfLife::update(float dt)
{
    (void)dt;

    updateSimulation();

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
    const float fadeIn  = 6.0f * dt;
    const float fadeOut = 2.5f * dt;

    for(auto& cell : m_cells)
    {
        if(cell.alive)
        {
            cell.alpha += fadeIn;

            if(cell.alpha > 1.0f)
                cell.alpha = 1.0f;

            cell.scale += fadeIn;

            if(cell.scale > 1.0f)
                cell.scale = 1.0f;
        }
        else
        {
            cell.alpha -= fadeOut;

            if(cell.alpha < 0.0f)
                cell.alpha = 0.0f;

            cell.scale -= fadeOut;

            if(cell.scale < 0.0f)
                cell.scale = 0.0f;
        }
    }
}

void GameOfLife::render(Renderer& renderer)
{
    const float cellSize =
        static_cast<float>(m_cellSize);

    //---------------------------------------
    // Draw Cells
    //---------------------------------------

    for(int y = 0; y < m_height; y++)
    {
        for(int x = 0; x < m_width; x++)
        {
            const Cell& c =
                cell(x,y);

            if(c.alpha <= 0.01f)
                continue;

            float size =
                cellSize * c.scale;

            float px =
                x * cellSize +
                (cellSize - size) * 0.5f;

            float py =
                y * cellSize +
                (cellSize - size) * 0.5f;

            renderer.drawRectangle(
                px,
                py,
                size,
                size,
                0.2f,
                0.55f,
                1.0f,
                c.alpha);
        }
    }

    //---------------------------------------
    // Drag Selection
    //---------------------------------------

    if(m_dragging)
    {
        renderer.drawRectangleOutline(
        m_dragStartX * m_cellSize,
        m_dragStartY * m_cellSize,
        (m_dragEndX-m_dragStartX+1)*m_cellSize,
        (m_dragEndY-m_dragStartY+1)*m_cellSize,
        1,1,1,1);

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
    if(button != Button3)
        return;

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
    int x,
    int y)
{
    if(button != Button3)
        return;

    (void)x;
    (void)y;

    m_dragging = false;

    paintRectangle(
        m_dragStartX,
        m_dragStartY,
        m_dragEndX,
        m_dragEndY);
}

void GameOfLife::paintRectangle(
    int x0,
    int y0,
    int x1,
    int y1)
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

            Cell& c =
                cell(x,y);

            c.alive =
                rand()%2;

            if(c.alive)
            {
                c.alpha = 1.0f;
                c.scale = 1.0f;
                c.age = 0;
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

