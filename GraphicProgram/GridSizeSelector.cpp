#include "GridSizeSelector.h"

GridSizeSelector::GridSizeSelector(sf::RenderWindow& window, const sf::Font& font)
    : window(window)
    , font(font)
    , rows(50)
    , cols(50)
    , confirmed(false)
    , focusRows(true)
{
}

void GridSizeSelector::draw() {
    window.clear(BACKGROUND_COLOR);

    // Titre
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Grid Size Selection");
    titleText.setCharacterSize(50);
    titleText.setFillColor(SELECTED_COLOR);
    titleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.width / 2.0f, titleRect.height / 2.0f);
    titleText.setPosition(window.getSize().x / 2.0f, 200);
    window.draw(titleText);

    // Instructions
    sf::Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setString(
        "Use Up/Down arrows to change values\n"
        "Tab to switch between rows and columns\n"
        "Enter to confirm\n"
        "ESC to return to menu"
    );
    instructionsText.setCharacterSize(24);
    instructionsText.setFillColor(TEXT_COLOR);

    sf::FloatRect instrRect = instructionsText.getLocalBounds();
    instructionsText.setOrigin(instrRect.width / 2.0f, instrRect.height / 2.0f);
    instructionsText.setPosition(window.getSize().x / 2.0f, 300);
    window.draw(instructionsText);

    // Valeurs
    std::string rowsStr = "Rows: " + std::to_string(rows);
    std::string colsStr = "Columns: " + std::to_string(cols);

    drawText(rowsStr, 400, focusRows);
    drawText(colsStr, 460, !focusRows);

    window.display();
}

void GridSizeSelector::drawText(const std::string& text, float y, bool selected) {
    sf::Text textObj;
    textObj.setFont(font);
    textObj.setString(text);
    textObj.setCharacterSize(32);
    textObj.setFillColor(selected ? SELECTED_COLOR : TEXT_COLOR);

    sf::FloatRect textRect = textObj.getLocalBounds();
    textObj.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    textObj.setPosition(window.getSize().x / 2.0f, y);

    window.draw(textObj);
}

void GridSizeSelector::handleEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Tab) {
            focusRows = !focusRows;
        }
        else if (event.key.code == sf::Keyboard::Up) {
            updateValue(true);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            updateValue(false);
        }
        else if (event.key.code == sf::Keyboard::Return) {
            confirmed = true;
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            confirmed = false;
        }
    }
}

void GridSizeSelector::updateValue(bool increase) {
    if (focusRows) {
        rows += increase ? 1 : -1;
        rows = std::max(MIN_SIZE, std::min(MAX_SIZE, rows));
    }
    else {
        cols += increase ? 1 : -1;
        cols = std::max(MIN_SIZE, std::min(MAX_SIZE, cols));
    }
}