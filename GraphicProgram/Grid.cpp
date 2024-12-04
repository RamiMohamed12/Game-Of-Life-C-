#include "Grid.h"

Grid::Grid(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<bool>(width, false));
}

void Grid::reset() {
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), false);
    }
}

void Grid::setCell(int x, int y, bool state) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = state;
    }
}

bool Grid::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return false;
}

void Grid::updateGrid() {
    // Mise à jour de la grille selon les règles du Jeu de la Vie
    std::vector<std::vector<bool>> nextGrid = grid;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int liveNeighbors = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int ny = y + dy;
                    int nx = x + dx;
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                        liveNeighbors += grid[ny][nx];
                    }
                }
            }
            if (grid[y][x]) {
                nextGrid[y][x] = (liveNeighbors == 2 || liveNeighbors == 3);
            }
            else {
                nextGrid[y][x] = (liveNeighbors == 3);
            }
        }
    }
    grid = nextGrid;
}
