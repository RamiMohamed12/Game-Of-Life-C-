#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {
public:
    Grid(int width, int height);
    void reset();
    void setCell(int x, int y, bool state);
    bool getCell(int x, int y) const;
    void updateGrid();
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width, height;
    std::vector<std::vector<bool>> grid;
};

#endif
