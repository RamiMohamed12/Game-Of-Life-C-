#include "GridSelector.h"
#include <sstream>

void GridSelector::draw() {
    window.clear(sf::Color(18, 18, 24));

    // Titre
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Select Grid Size");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color(129, 140, 248));
    titleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.width / 2.0f, titleRect.height / 2.0f);
    titleText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 4.0f);
    window.draw(titleText);

    // Affichage de la taille actuelle
    std::stringstream ss;
    ss << rows << "x" << cols;

    sf::Text sizeText;
    sizeText.setFont(font);
    sizeText.setString(ss.str());
    sizeText.setCharacterSize(72);
    sizeText.setFillColor(sf::Color(203, 213, 225));

    sf::FloatRect sizeRect = sizeText.getLocalBounds();
    sizeText.setOrigin(sizeRect.width / 2.0f, sizeRect.height / 2.0f);
    sizeText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    window.draw(sizeText);

    // Instructions
    sf::Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setString(
        "Use Arrow Keys to adjust size\n"
        "Press Enter to confirm\n"
        "Press Escape to cancel"
    );
    instructionsText.setCharacterSize(24);
    instructionsText.setFillColor(sf::Color(128, 128, 128));

    sf::FloatRect instrRect = instructionsText.getLocalBounds();
    instructionsText.setOrigin(instrRect.width / 2.0f, instrRect.height / 2.0f);
    instructionsText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 3.0f / 4.0f);
    window.draw(instructionsText);
}

void GridSelector::handleEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            rows = std::min(rows + 10, 200);
            cols = rows;
        }
        else if (event.key.code == sf::Keyboard::Down) {
            rows = std::max(rows - 10, 10);
            cols = rows;
        }
        else if (event.key.code == sf::Keyboard::Left) {
            cols = std::max(cols - 10, 10);
            rows = cols;
        }
        else if (event.key.code == sf::Keyboard::Right) {
            cols = std::min(cols + 10, 200);
            rows = cols;
        }
        else if (event.key.code == sf::Keyboard::Return) {
            confirmed = true;
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            rows = 50;
            cols = 50;
            confirmed = true;
        }
    }
}