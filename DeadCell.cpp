#include "DeadCell.h"
#include <SFML/Graphics.hpp>

void DeadCell::draw(sf::RenderWindow& window, float x, float y, float size) const {
    sf::RectangleShape cell(sf::Vector2f(size - 1, size - 1));
    cell.setPosition(x, y);
    cell.setFillColor(sf::Color(30, 30, 40));  // Dark background for dead cells
    window.draw(cell);
}