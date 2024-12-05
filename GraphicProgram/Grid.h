#ifndef GRID_H
#define GRID_H

#include <vector>
#include <memory>
#include "Cell.h"

class Grid {
public:
    Grid(int width, int height);
    void reset();
    void setCell(int x, int y, bool alive);
    bool getCellState(int x, int y) const;
    void updateGrid();
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void draw(sf::RenderWindow& window, float cellSize) const;

private:
    int width, height;
    std::vector<std::vector<std::shared_ptr<Cell>>> cells; // Use shared_ptr<Cell>
    int countLiveNeighbors(int x, int y) const;
};

#endif
