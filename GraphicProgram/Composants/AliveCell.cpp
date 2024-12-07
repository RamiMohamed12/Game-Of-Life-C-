#include "AliveCell.h"
#include <SFML/Graphics.hpp>

void AliveCell::draw(sf::RenderWindow& window, float x, float y, float size) const {
    sf::RectangleShape cell(sf::Vector2f(size - 1, size - 1));
    cell.setPosition(x, y);
    cell.setFillColor(sf::Color(129, 140, 248));  // Indigo color for living cells
    window.draw(cell);
}