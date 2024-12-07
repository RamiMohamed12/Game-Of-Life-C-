#ifndef DEAD_CELL_H
#define DEAD_CELL_H

#include "Cell.h"

class DeadCell : public Cell {
public:
    bool isAlive() const override { return false; }
    void draw(sf::RenderWindow& window, float x, float y, float size) const override;
};

#endif