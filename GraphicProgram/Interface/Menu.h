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
    static const int TITLE_FONT_SIZE;
    static const int SUBTITLE_FONT_SIZE;

    sf::RenderWindow& window;
    const sf::Font& font;
    std::vector<std::string> menuItems;
    int selectedMenuItem;
    bool itemSelected;
    float menuItemAlpha;
    sf::Clock animationClock;

    // Couleurs modernes
    const sf::Color BACKGROUND_COLOR = sf::Color(18, 18, 24);
    const sf::Color MENU_SELECTED_COLOR = sf::Color(129, 140, 248);
    const sf::Color MENU_HOVER_COLOR = sf::Color(99, 110, 218);
    const sf::Color MENU_NORMAL_COLOR = sf::Color(203, 213, 225);
    const sf::Color ACCENT_COLOR = sf::Color(244, 114, 182);
    const sf::Color SUBTITLE_COLOR = sf::Color(148, 163, 184);

    void drawTitle();
    void drawSubtitle();
    void drawMenuItems();
    void drawBackground();
    void drawDecorations();
    void updateAnimation();
    bool isMouseOverItem(int index, sf::Vector2i mousePos) const;
};

#endif
