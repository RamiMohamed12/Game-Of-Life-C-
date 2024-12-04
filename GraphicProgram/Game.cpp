#include "Game.h"

Game::Game()
    : window(sf::VideoMode(750, 750), "Jeu de la Vie - SFML"),
    grid(50, 50),
    isRunning(false),
    selectedPattern("Glider"),
    manualMode(false) {
    window.setFramerateLimit(60);
    patterns = Pattern::getPatterns();
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        draw();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                isRunning = !isRunning; // Basculer entre pause et simulation
            }
            if (event.key.code == sf::Keyboard::R) {
                reset(); // Réinitialiser la grille
            }
            // Sélection des motifs
            if (event.key.code == sf::Keyboard::Num1) {
                selectedPattern = "Glider";
                manualMode = false;
            }
            if (event.key.code == sf::Keyboard::Num2) {
                selectedPattern = "Small Exploder";
                manualMode = false;
            }
            if (event.key.code == sf::Keyboard::Num3) {
                selectedPattern = "Spaceship";
                manualMode = false;
            }
            if (event.key.code == sf::Keyboard::Num4) {
                manualMode = true; // Activer le mode manuel
            }
        }

        // Interaction avec la souris
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / 15;
                int y = event.mouseButton.y / 15;
                if (x >= 0 && x < 50 && y >= 0 && y < 50) {
                    if (manualMode) {
                        grid.setCell(x, y, !grid.getCell(x, y)); // Changer l'état de la cellule
                    }
                    else {
                        Pattern::insertPattern(grid, patterns[selectedPattern], x, y); // Insérer le motif
                    }
                }
            }
        }
    }
}

void Game::update() {
    if (isRunning && clock.getElapsedTime().asMilliseconds() > 100) {
        grid.updateGrid();
        clock.restart();
    }
}

void Game::draw() {
    window.clear(sf::Color(10, 10, 30));

    // Dessiner la grille
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            sf::RectangleShape cell(sf::Vector2f(15 - 2, 15 - 2));
            cell.setPosition(x * 15, y * 15);
            if (grid.getCell(x, y)) {
                cell.setFillColor(sf::Color(0, 255, 150)); // Couleur des cellules vivantes
            }
            else {
                cell.setFillColor(sf::Color(30, 30, 30)); // Couleur des cellules mortes
            }
            window.draw(cell);
        }
    }

    // Afficher le mode ou motif sélectionné
    sf::Font font;
    if (font.loadFromFile("arial.ttf")) {
        sf::Text modeText;
        if (manualMode) {
            modeText.setString("Mode: Manuel");
        }
        else {
            modeText.setString("Motif: " + selectedPattern);
        }
        modeText.setFont(font);
        modeText.setCharacterSize(20);
        modeText.setFillColor(sf::Color::White);
        modeText.setPosition(10, grid.getHeight() * 15 - 30);
        window.draw(modeText);
    }

    window.display();
}

void Game::reset() {
    grid.reset(); // Réinitialiser la grille
}
