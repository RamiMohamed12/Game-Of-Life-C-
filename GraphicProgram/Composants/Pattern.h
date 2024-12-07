#ifndef PATTERN_H
#define PATTERN_H

#include "Grid.h"
#include <vector>
#include <map>
#include <string>

class Pattern {
public:
    static std::map<std::string, std::vector<std::vector<bool>>> getPatterns();
    static void insertPattern(Grid& grid, const std::vector<std::vector<bool>>& pattern, int startX, int startY);
};

#endif