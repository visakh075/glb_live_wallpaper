#pragma once

#include <vector>

#include "Effect.h"
#include "Cell.h"

class Renderer;

class GameOfLife : public Effect
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

    void keyPress(
        KeySym key) override;

    void mousePress(
        int button,
        int x,
        int y) override;

    void mouseRelease(
        int button,
        int x,
        int y) override;

    void mouseMove(
        int x,
        int y) override;

private:

    //------------------------------------
    // Grid
    //------------------------------------

    int m_width = 0;
    int m_height = 0;

    int m_cellSize = 4;

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
        int y1);

    int pixelToGridX(
        int x) const;

    int pixelToGridY(
        int y) const;
};