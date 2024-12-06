#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include <vector>

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
    std::vector<std::vector<bool>> cells;
    std::vector<std::vector<bool>> nextCells;
    int countLiveNeighbors(int x, int y) const;
};

#endif