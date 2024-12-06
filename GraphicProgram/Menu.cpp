#include "Menu.h"

const float Menu::MENU_ITEM_HEIGHT = 60.0f;
const int Menu::MENU_FONT_SIZE = 32;
const int Menu::STATUS_FONT_SIZE = 20;

Menu::Menu(sf::RenderWindow& window, const sf::Font& font)
    : window(window)
    , font(font)
    , selectedMenuItem(0)
    , itemSelected(false) {
    menuItems = { "New Game", "Load Game", "Tutorial", "Exit" };
}

void Menu::draw() {
    window.clear(BACKGROUND_COLOR);
    drawTitle();
    drawMenuItems();
    drawInstructions();
    window.display();
}

void Menu::drawTitle() {
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Game of Life");
    titleText.setCharacterSize(MENU_FONT_SIZE * 1.5);
    titleText.setFillColor(MENU_SELECTED_COLOR);
    titleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.width / 2.0f, titleRect.height / 2.0f);
    titleText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.2f);

    window.draw(titleText);
}

void Menu::drawMenuItems() {
    float startY = (window.getSize().y - menuItems.size() * MENU_ITEM_HEIGHT) / 2;

    for (size_t i = 0; i < menuItems.size(); ++i) {
        sf::Text menuText;
        menuText.setFont(font);
        menuText.setString(menuItems[i]);
        menuText.setCharacterSize(MENU_FONT_SIZE);
        menuText.setFillColor(static_cast<int>(i) == selectedMenuItem ? MENU_SELECTED_COLOR : MENU_NORMAL_COLOR);

        sf::FloatRect textRect = menuText.getLocalBounds();
        menuText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
        menuText.setPosition(window.getSize().x / 2.0f, startY + i * MENU_ITEM_HEIGHT);

        window.draw(menuText);
    }
}

void Menu::drawInstructions() {
    sf::Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Use arrow keys or mouse to navigate, Enter to select");
    instructionText.setCharacterSize(STATUS_FONT_SIZE);
    instructionText.setFillColor(sf::Color(128, 128, 128));

    sf::FloatRect instrRect = instructionText.getLocalBounds();
    instructionText.setOrigin(instrRect.width / 2.0f, instrRect.height / 2.0f);
    instructionText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.8f);

    window.draw(instructionText);
}

void Menu::handleEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedMenuItem = (selectedMenuItem - 1 + menuItems.size()) % menuItems.size();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedMenuItem = (selectedMenuItem + 1) % menuItems.size();
        }
        else if (event.key.code == sf::Keyboard::Return) {
            itemSelected = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            float startY = (window.getSize().y - menuItems.size() * MENU_ITEM_HEIGHT) / 2;
            for (size_t i = 0; i < menuItems.size(); ++i) {
                sf::FloatRect menuItemRect(
                    0,
                    startY + i * MENU_ITEM_HEIGHT - MENU_ITEM_HEIGHT / 2,
                    window.getSize().x,
                    MENU_ITEM_HEIGHT
                );
                if (menuItemRect.contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectedMenuItem = i;
                    itemSelected = true;
                }
            }
        }
    }
    else if (event.type == sf::Event::MouseMoved) {
        float startY = (window.getSize().y - menuItems.size() * MENU_ITEM_HEIGHT) / 2;
        for (size_t i = 0; i < menuItems.size(); ++i) {
            sf::FloatRect menuItemRect(
                0,
                startY + i * MENU_ITEM_HEIGHT - MENU_ITEM_HEIGHT / 2,
                window.getSize().x,
                MENU_ITEM_HEIGHT
            );
            if (menuItemRect.contains(event.mouseMove.x, event.mouseMove.y)) {
                selectedMenuItem = i;
            }
        }
    }
}