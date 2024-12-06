#ifndef GRID_SIZE_SELECTOR_H
#define GRID_SIZE_SELECTOR_H

#include <SFML/Graphics.hpp>

class GridSizeSelector {
public:
    GridSizeSelector(sf::RenderWindow& window, const sf::Font& font);
    void draw();
    void handleEvents(const sf::Event& event);
    bool isConfirmed() const { return confirmed; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    void reset() { confirmed = false; }

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    int rows;
    int cols;
    bool confirmed;
    bool focusRows;

    const int MIN_SIZE = 10;
    const int MAX_SIZE = 200;
    const sf::Color TEXT_COLOR = sf::Color(203, 213, 225);
    const sf::Color SELECTED_COLOR = sf::Color(129, 140, 248);
    const sf::Color BACKGROUND_COLOR = sf::Color(18, 18, 24);

    void drawText(const std::string& text, float y, bool selected);
    void updateValue(bool increase);
};

#endif