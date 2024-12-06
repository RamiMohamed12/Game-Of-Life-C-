#ifndef GRID_SIZE_SELECTOR_H
#define GRID_SIZE_SELECTOR_H

#include <SFML/Graphics.hpp>

class GridSizeSelector {
public:
    GridSizeSelector(sf::RenderWindow& window, sf::Font& font);
    void draw();
    void handleEvents(const sf::Event& event);
    bool isConfirmed() const { return confirmed; }
    void reset() { confirmed = false; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }

private:
    sf::RenderWindow& window;
    sf::Font& font;
    int rows;
    int cols;
    bool confirmed;
};

#endif