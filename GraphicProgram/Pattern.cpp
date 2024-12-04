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
            {1, 0, 1}
        }},
        {"Spaceship", {
            {0, 1, 1, 1, 0},
            {1, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 0, 0, 1, 0}
        }}
    };
}

void Pattern::insertPattern(Grid& grid, const std::vector<std::vector<bool>>& pattern, int startX, int startY) {
    for (size_t y = 0; y < pattern.size(); ++y) {
        for (size_t x = 0; x < pattern[y].size(); ++x) {
            grid.setCell(startX + x, startY + y, pattern[y][x]);
        }
    }
}
