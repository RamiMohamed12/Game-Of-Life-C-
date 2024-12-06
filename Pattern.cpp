#include "Pattern.h"

std::map<std::string, std::vector<std::vector<bool>>> Pattern::getPatterns() {
    return {
        {"Glider", {
            {0, 1, 0},
            {0, 0, 1},
            {1, 1, 1}
        }},
        {"Small Exploder", {
            {0, 1, 0},
            {1, 1, 1},
            {1, 0, 1},
            {0, 1, 0}
        }},
        {"Spaceship", {
            {0, 1, 1, 1, 0},
            {1, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 0, 0, 1, 0}
        }},
        {"Pulsar", {
            {0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0}
        }}
    };
}

void Pattern::insertPattern(Grid& grid, const std::vector<std::vector<bool>>& pattern, int startX, int startY) {
    for (size_t y = 0; y < pattern.size(); ++y) {
        for (size_t x = 0; x < pattern[y].size(); ++x) {
            int gridX = startX + x;
            int gridY = startY + y;
            if (gridX >= 0 && gridX < grid.getWidth() && gridY >= 0 && gridY < grid.getHeight()) {
                grid.setCell(gridX, gridY, pattern[y][x]);
            }
        }
    }
}