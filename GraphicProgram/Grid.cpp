#include "Grid.h"
#include "AliveCell.h"
#include "DeadCell.h"

Grid::Grid(int width, int height) : width(width), height(height) {
    cells.resize(height);
    for (auto& row : cells) {
        row.resize(width);
        for (auto& cell : row) {
            cell = std::make_shared<DeadCell>();
        }
    }
}

void Grid::reset() {
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell = std::make_shared<DeadCell>();
        }
    }
}

void Grid::setCell(int x, int y, bool alive) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        cells[y][x] = alive ? std::shared_ptr<Cell>(std::make_shared<AliveCell>())
            : std::shared_ptr<Cell>(std::make_shared<DeadCell>());
    }
}

bool Grid::getCellState(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return cells[y][x]->isAlive();
    }
    return false;
}

int Grid::countLiveNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + width) % width;   // Wrap around horizontally
            int ny = (y + dy + height) % height; // Wrap around vertically
            if (cells[ny][nx]->isAlive()) {
                ++count;
            }
        }
    }
    return count;
}


void Grid::updateGrid() {
    auto newCells = cells;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int neighbors = countLiveNeighbors(x, y);
            bool currentState = cells[y][x]->isAlive();

            if (currentState) {
                newCells[y][x] = (neighbors == 2 || neighbors == 3) ?
                    std::shared_ptr<Cell>(std::make_shared<AliveCell>())
                    : std::shared_ptr<Cell>(std::make_shared<DeadCell>());
            }
            else {
                newCells[y][x] = (neighbors == 3) ?
                    std::shared_ptr<Cell>(std::make_shared<AliveCell>())
                    : std::shared_ptr<Cell>(std::make_shared<DeadCell>());
            }
        }
    }
    cells = std::move(newCells);
}

void Grid::draw(sf::RenderWindow& window, float cellSize) const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cells[y][x]->draw(window, x * cellSize, y * cellSize, cellSize);
        }
    }
}
