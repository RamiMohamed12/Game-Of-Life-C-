#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(sf::RenderWindow& window, const sf::Font& font);
    void draw();
    void handleEvents(const sf::Event& event);
    int getSelectedItem() const { return selectedMenuItem; }
    bool isItemSelected() const { return itemSelected; }
    void resetSelection() { itemSelected = false; }

private:
    static const float MENU_ITEM_HEIGHT;
    static const int MENU_FONT_SIZE;
    static const int STATUS_FONT_SIZE;

    sf::RenderWindow& window;
    const sf::Font& font;
    std::vector<std::string> menuItems;
    int selectedMenuItem;
    bool itemSelected;

    const sf::Color MENU_SELECTED_COLOR = sf::Color(129, 140, 248);
    const sf::Color MENU_NORMAL_COLOR = sf::Color(203, 213, 225);
    const sf::Color BACKGROUND_COLOR = sf::Color(18, 18, 24);

    void drawTitle();
    void drawMenuItems();
    void drawInstructions();
};

#endif