#pragma once
#ifndef GRID_SELECTOR_H
#define GRID_SELECTOR_H

#include <SFML/Graphics.hpp>

class GridSelector {
public:
    GridSelector(sf::RenderWindow& window, const sf::Font& font)
        : window(window)
        , font(font)
        , rows(50)
        , cols(50)
        , confirmed(false) {
    }

    void draw();
    void handleEvents(const sf::Event& event);
    bool isConfirmed() const { return confirmed; }
    void reset() { confirmed = false; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    int rows;
    int cols;
    bool confirmed;
};

#endif