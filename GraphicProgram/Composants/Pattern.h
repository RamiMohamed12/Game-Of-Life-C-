#ifndef PATTERNS_H
#define PATTERNS_H

#include <vector>
#include <map>
#include <string>

using Grid = std::vector<std::vector<bool>>;

class Patterns {
public:
    static const std::map<std::string, Grid>& getPatterns();
};

#endif
