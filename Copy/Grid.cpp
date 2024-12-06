#include "Grid.h"

Grid::Grid(int width, int height)
    : width(width)
    , height(height)
    , cells(height, std::vector<bool>(width, false))
    , nextCells(height, std::vector<bool>(width, false)) {
}

void Grid::reset() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cells[y][x] = false;
            nextCells[y][x] = false;
        }
    }
}

void Grid::setCell(int x, int y, bool alive) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        cells[y][x] = alive;
    }
}

bool Grid::getCellState(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return cells[y][x];
    }
    return false;
}

int Grid::countLiveNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int nx = (x + dx + width) % width;
            int ny = (y + dy + height) % height;

            if (cells[ny][nx]) count++;
        }
    }
    return count;
}

void Grid::updateGrid() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int neighbors = countLiveNeighbors(x, y);
            bool currentState = cells[y][x];

            if (currentState) {
                nextCells[y][x] = (neighbors == 2 || neighbors == 3);
            }
            else {
                nextCells[y][x] = (neighbors == 3);
            }
        }
    }
    cells.swap(nextCells);
}

void Grid::draw(sf::RenderWindow& window, float cellSize) const {
    const sf::Color ALIVE_COLOR(129, 140, 248);  // Indigo
    const sf::Color DEAD_COLOR(30, 30, 40);      // Dark background

    sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cell.setPosition(x * cellSize, y * cellSize);
            cell.setFillColor(cells[y][x] ? ALIVE_COLOR : DEAD_COLOR);
            window.draw(cell);
        }
    }
}