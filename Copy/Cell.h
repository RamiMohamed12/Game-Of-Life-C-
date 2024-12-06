#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>

class Cell {
public:
    virtual ~Cell() = default;
    virtual bool isAlive() const = 0;
    virtual void draw(sf::RenderWindow& window, float x, float y, float size) const = 0;
};

#endif