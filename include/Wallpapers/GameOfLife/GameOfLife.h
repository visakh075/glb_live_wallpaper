#pragma once

#include <vector>

#include "Core/Wallpaper.h"
#include "Core/Event.h"

#include "Wallpapers/GameOfLife/Cell.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

class Renderer;
enum class BrushMode
{
    Random,
    Clear
};

class GameOfLife : public Wallpaper
{
public:

    GameOfLife();
    ~GameOfLife() override = default;

    bool initialize(Renderer& renderer) override;

    void shutdown() override;

    void resize(
        int width,
        int height) override;

    void update(
        float dt) override;

    void render(
        Renderer& renderer) override;

void onEvent(
    const Event& e) override;

private:

    //------------------------------------
    // Grid
    //------------------------------------

    int m_width = 0;
    int m_height = 0;

    int m_cellSize = 5;

    std::vector<Cell> m_cells;
    std::vector<Cell> m_nextCells;
    
    //------------------------------------
    // Mouse
    //------------------------------------

    bool m_dragging = false;

    int m_dragStartX = 0;
    int m_dragStartY = 0;

    int m_dragEndX = 0;
    int m_dragEndY = 0;

    //------------------------------------
    // Helpers
    //------------------------------------

    int index(
        int x,
        int y) const;

    bool inside(
        int x,
        int y) const;

    Cell& cell(
        int x,
        int y);

    const Cell& cell(
        int x,
        int y) const;

    //------------------------------------
    // Simulation
    //------------------------------------

    void randomize();

    void clear();

    void updateSimulation();

    void updateAnimation(
        float dt);

    //------------------------------------
    // Painting
    //------------------------------------

    void paintRectangle(
        int x0,
        int y0,
        int x1,
        int y1,
        BrushMode mode);

    int pixelToGridX(
        int x) const;

    int pixelToGridY(
        int y) const;

    void keyPress(
        KeySym key);

    void mousePress(
        int button,
        int x,
        int y);

    void mouseRelease(
        int button,
        int x,
        int y);

    void mouseMove(
        int x,
        int y);
};