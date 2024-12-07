#ifndef ALIVE_CELL_H
#define ALIVE_CELL_H

#include "Cell.h"

class AliveCell : public Cell {
public:
    bool isAlive() const override { return true; }
    void draw(sf::RenderWindow& window, float x, float y, float size) const override;
};

#endif