#include "Menu.h"
#include <cmath>

const float Menu::MENU_ITEM_HEIGHT = 70.0f;
const int Menu::MENU_FONT_SIZE = 32;
const int Menu::TITLE_FONT_SIZE = 72;
const int Menu::SUBTITLE_FONT_SIZE = 24;

Menu::Menu(sf::RenderWindow& window, const sf::Font& font)
    : window(window)
    , font(font)
    , selectedMenuItem(0)
    , itemSelected(false)
    , menuItemAlpha(0) {
    menuItems = { "Start Game", "Load Game", "Tutorial", "Music Settings", "Exit" };
}


void Menu::draw() {
    updateAnimation();
    window.clear(BACKGROUND_COLOR);

    drawBackground();
    drawDecorations();
    drawTitle();
    drawSubtitle();
    drawMenuItems();

    window.display();
}

void Menu::drawBackground() {
    // Effet de dégradé avec plusieurs rectangles
    for (int i = 0; i < 5; i++) {
        sf::RectangleShape gradient(sf::Vector2f(window.getSize().x, window.getSize().y / 5.f));
        float alpha = 255.f * (1.f - (i / 5.f));
        gradient.setFillColor(sf::Color(ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, static_cast<sf::Uint8>(alpha * 0.1f)));
        gradient.setPosition(0, i * window.getSize().y / 5.f);
        window.draw(gradient);
    }
}

void Menu::drawDecorations() {
    // Cercles décoratifs animés
    float time = animationClock.getElapsedTime().asSeconds();
    for (int i = 0; i < 3; i++) {
        sf::CircleShape circle(50.f + i * 30.f);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setPosition(
            window.getSize().x * 0.1f + std::cos(time + i) * 20.f,
            window.getSize().y * 0.3f + std::sin(time + i) * 20.f
        );
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(2.f);
        circle.setOutlineColor(sf::Color(ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, 50));
        window.draw(circle);
    }
}

void Menu::drawTitle() {
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Game of Life");
    titleText.setCharacterSize(TITLE_FONT_SIZE);
    titleText.setFillColor(MENU_SELECTED_COLOR);
    titleText.setStyle(sf::Text::Bold);

    // Effet d'ombre
    sf::Text shadowText = titleText;
    shadowText.setFillColor(sf::Color(0, 0, 0, 100));
    shadowText.setPosition(window.getSize().x * 0.5f + 3, window.getSize().y * 0.2f + 3);

    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.width / 2.0f, titleRect.height / 2.0f);
    titleText.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.2f);

    window.draw(shadowText);
    window.draw(titleText);
}

void Menu::drawSubtitle() {
    sf::Text subtitleText;
    subtitleText.setFont(font);
    subtitleText.setString("A cellular automaton simulation");
    subtitleText.setCharacterSize(SUBTITLE_FONT_SIZE);
    subtitleText.setFillColor(SUBTITLE_COLOR);
    subtitleText.setStyle(sf::Text::Regular);

    sf::FloatRect subtitleRect = subtitleText.getLocalBounds();
    subtitleText.setOrigin(subtitleRect.width / 2.0f, subtitleRect.height / 2.0f);
    subtitleText.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.3f);

    window.draw(subtitleText);
}

void Menu::drawMenuItems() {
    float startY = window.getSize().y * 0.5f;

    for (size_t i = 0; i < menuItems.size(); ++i) {
        sf::RectangleShape itemBackground;
        sf::Text menuText;

        // Configuration du texte
        menuText.setFont(font);
        menuText.setString(menuItems[i]);
        menuText.setCharacterSize(MENU_FONT_SIZE);

        // Couleur et effet de survol
        sf::Color itemColor = (static_cast<int>(i) == selectedMenuItem) ? MENU_SELECTED_COLOR : MENU_NORMAL_COLOR;
        if (isMouseOverItem(i, sf::Mouse::getPosition(window))) {
            itemColor = MENU_HOVER_COLOR;
        }

        menuText.setFillColor(itemColor);

        // Position et alignement
        sf::FloatRect textRect = menuText.getLocalBounds();
        menuText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
        float itemY = startY + i * MENU_ITEM_HEIGHT;
        menuText.setPosition(window.getSize().x * 0.5f, itemY);

        // Fond du bouton
        if (static_cast<int>(i) == selectedMenuItem) {
            itemBackground.setSize(sf::Vector2f(textRect.width + 40, MENU_ITEM_HEIGHT - 10));
            itemBackground.setFillColor(sf::Color(itemColor.r, itemColor.g, itemColor.b, 20));
            itemBackground.setOrigin(itemBackground.getSize().x / 2.f, itemBackground.getSize().y / 2.f);
            itemBackground.setPosition(window.getSize().x * 0.5f, itemY);
            window.draw(itemBackground);
        }

        window.draw(menuText);
    }
}

void Menu::updateAnimation() {
    float time = animationClock.getElapsedTime().asSeconds();
    menuItemAlpha = (std::sin(time * 2.f) + 1.f) * 0.5f * 255.f;
}

bool Menu::isMouseOverItem(int index, sf::Vector2i mousePos) const {
    float startY = window.getSize().y * 0.5f;
    float itemY = startY + index * MENU_ITEM_HEIGHT;

    sf::Text tempText;
    tempText.setFont(font);
    tempText.setString(menuItems[index]);
    tempText.setCharacterSize(MENU_FONT_SIZE);

    sf::FloatRect textRect = tempText.getLocalBounds();
    sf::FloatRect bounds(
        window.getSize().x * 0.5f - textRect.width / 2.f - 20,
        itemY - MENU_ITEM_HEIGHT / 2.f,
        textRect.width + 40,
        MENU_ITEM_HEIGHT
    );

    return bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
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
            for (size_t i = 0; i < menuItems.size(); i++) {
                if (isMouseOverItem(i, sf::Mouse::getPosition(window))) {
                    selectedMenuItem = i;
                    itemSelected = true;
                    break;
                }
            }
        }
    }
    else if (event.type == sf::Event::MouseMoved) {
        for (size_t i = 0; i < menuItems.size(); i++) {
            if (isMouseOverItem(i, sf::Mouse::getPosition(window))) {
                selectedMenuItem = i;
                break;
            }
        }
    }
}
