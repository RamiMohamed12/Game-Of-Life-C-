#include "Game.h"
#include <iostream>
#include <stdexcept>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Game of Life", sf::Style::Fullscreen)
    , gridSize(window.getSize().x / CELL_SIZE)
    , grid(gridSize, gridSize)
    , isRunning(false)
    , selectedPattern("Glider")
    , manualMode(false)
    , gameState(GameState::Menu)
    , selectedMenuItem(0) {

    window.setFramerateLimit(60);
    loadPatterns();

    if (!font.loadFromFile("/usr/share/fonts/adobe-source-code-pro-fonts/SourceCodePro-Medium.otf")) {
        std::cerr << "Error loading font!" << std::endl;
        throw std::runtime_error("Failed to load font");
    }

    menuItems = { "Start Game", "Load Game", "Tutorial", "Exit" };
}

void Game::loadPatterns() {
    patterns["Glider"] = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1}
    };
    patterns["Small Exploder"] = {
        {0, 1, 0},
        {1, 1, 1},
        {1, 0, 1}
    };
    patterns["Spaceship"] = {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0}
    };
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();

        if (gameState == GameState::Menu) {
            drawMenu();
        }
        else if (gameState == GameState::Tutorial) {
            drawTutorial();
        }
        else {
            update();
            draw();
        }
    }
}

void Game::update() {
    if (isRunning && clock.getElapsedTime().asMilliseconds() > 100) {
        grid.updateGrid();
        clock.restart();
    }
}

void Game::drawMenu() {
    window.clear(BACKGROUND_COLOR);

    // Draw title
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Game of Life");
    titleText.setCharacterSize(MENU_FONT_SIZE * 1.5);
    titleText.setFillColor(MENU_SELECTED_COLOR);
    titleText.setStyle(sf::Text::Bold);

    // Center the title
    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setPosition(
        (gridSize * CELL_SIZE - titleRect.width) / 2,
        gridSize * CELL_SIZE * 0.2f
    );

    window.draw(titleText);

    // Draw menu items
    float startY = (gridSize * CELL_SIZE - menuItems.size() * MENU_ITEM_HEIGHT) / 2;

    for (size_t i = 0; i < menuItems.size(); ++i) {
        sf::Text menuText;
        menuText.setFont(font);
        menuText.setString(menuItems[i]);
        menuText.setCharacterSize(MENU_FONT_SIZE);

        // Create selection indicator
        if (static_cast<int>(i) == selectedMenuItem) {
            menuText.setFillColor(MENU_SELECTED_COLOR);

            // Draw selection indicator
            sf::RectangleShape indicator(sf::Vector2f(4, MENU_FONT_SIZE));
            indicator.setFillColor(MENU_SELECTED_COLOR);
            indicator.setPosition(
                (gridSize * CELL_SIZE - menuText.getLocalBounds().width) / 2 - 20,
                startY + i * MENU_ITEM_HEIGHT + MENU_FONT_SIZE / 2
            );
            window.draw(indicator);
        }
        else {
            menuText.setFillColor(MENU_NORMAL_COLOR);
        }

        // Center the text
        sf::FloatRect textRect = menuText.getLocalBounds();
        menuText.setPosition(
            (gridSize * CELL_SIZE - textRect.width) / 2,
            startY + i * MENU_ITEM_HEIGHT
        );

        window.draw(menuText);
    }

    // Draw instructions
    sf::Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Use arrow keys or mouse to navigate, Enter to select");
    instructionText.setCharacterSize(STATUS_FONT_SIZE);
    instructionText.setFillColor(sf::Color(128, 128, 128));

    // Position at bottom
    sf::FloatRect instrRect = instructionText.getLocalBounds();
    instructionText.setPosition(
        (gridSize * CELL_SIZE - instrRect.width) / 2,
        gridSize * CELL_SIZE * 0.8f
    );

    window.draw(instructionText);
    window.display();
}

void Game::draw() {
    window.clear(BACKGROUND_COLOR);

    // Draw the grid
    grid.draw(window, CELL_SIZE);

    // Draw status bar background
    sf::RectangleShape statusBar(sf::Vector2f(gridSize * CELL_SIZE, 40));
    statusBar.setFillColor(sf::Color(24, 24, 29));
    statusBar.setPosition(0, grid.getHeight() * CELL_SIZE - 40);
    window.draw(statusBar);

    // Display the mode or selected pattern
    sf::Text statusText;
    statusText.setFont(font);

    std::string statusString;
    if (manualMode) {
        statusString = "Mode: Manual";
    }
    else {
        statusString = "Pattern: " + selectedPattern;
    }

    // Add controls hint
    statusString += "    |    Space: Play/Pause    |    S: Save    |    Esc: Menu";

    statusText.setString(statusString);
    statusText.setCharacterSize(STATUS_FONT_SIZE);
    statusText.setFillColor(MENU_NORMAL_COLOR);
    statusText.setPosition(10, grid.getHeight() * CELL_SIZE - 30);
    window.draw(statusText);

    // Display save message if recent
    if (saveMessageClock.getElapsedTime() < SAVE_MESSAGE_DURATION) {
        sf::Text saveText;
        saveText.setFont(font);
        saveText.setString(saveMessage);
        saveText.setCharacterSize(STATUS_FONT_SIZE);
        saveText.setFillColor(MENU_SELECTED_COLOR);

        sf::FloatRect textRect = saveText.getLocalBounds();
        saveText.setPosition(
            (gridSize * CELL_SIZE - textRect.width) / 2,
            grid.getHeight() * CELL_SIZE * 0.1f
        );

        window.draw(saveText);
    }

    window.display();
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (gameState == GameState::Menu) {
            handleMenuEvents(event);
        }
        else {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    isRunning = !isRunning;
                }
                if (event.key.code == sf::Keyboard::R) {
                    reset();
                }
                if (event.key.code == sf::Keyboard::S) {
                    saveGame();
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    gameState = GameState::Menu;
                }
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
                    manualMode = true;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x / CELL_SIZE;
                    int y = event.mouseButton.y / CELL_SIZE;
                    if (x >= 0 && x < gridSize && y >= 0 && y < gridSize) {
                        if (manualMode) {
                            grid.setCell(x, y, !grid.getCellState(x, y));
                        }
                        else {
                            Pattern::insertPattern(grid, patterns[selectedPattern], x, y);
                        }
                    }
                }
            }
        }
    }
}

void Game::handleMenuEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedMenuItem = (selectedMenuItem - 1 + menuItems.size()) % menuItems.size();
        }
        if (event.key.code == sf::Keyboard::Down) {
            selectedMenuItem = (selectedMenuItem + 1) % menuItems.size();
        }
        if (event.key.code == sf::Keyboard::Return) {
            selectMenuItem();
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            float startY = (gridSize * CELL_SIZE - menuItems.size() * MENU_ITEM_HEIGHT) / 2;
            for (size_t i = 0; i < menuItems.size(); ++i) {
                sf::FloatRect menuItemRect(
                    (gridSize * CELL_SIZE - MENU_FONT_SIZE * menuItems[i].size()) / 2,
                    startY + i * MENU_ITEM_HEIGHT,
                    MENU_FONT_SIZE * menuItems[i].size(),
                    MENU_ITEM_HEIGHT
                );
                if (menuItemRect.contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectedMenuItem = i;
                    selectMenuItem();
                }
            }
        }
    }
}

void Game::selectMenuItem() {
    switch (selectedMenuItem) {
    case 0: // Start Game
        gameState = GameState::Playing;
        reset();
        break;
    case 1: // Load Game
        loadGame();
        gameState = GameState::Playing;
        break;
    case 2: // Tutorial
        gameState = GameState::Tutorial;
        break;
    case 3: // Exit
        window.close();
        break;
    }
}

void Game::reset() {
    grid.reset();
    isRunning = false;
}

void Game::saveGame() {
    std::ofstream file("gamestate.save");
    if (file.is_open()) {
        for (int y = 0; y < gridSize; ++y) {
            for (int x = 0; x < gridSize; ++x) {
                file << grid.getCellState(x, y) << " ";
            }
            file << "\n";
        }
        file.close();
        saveMessage = "Game saved successfully!";
        saveMessageClock.restart();
    }
    else {
        saveMessage = "Unable to save game!";
        saveMessageClock.restart();
    }
}

void Game::loadGame() {
    std::ifstream file("gamestate.save");
    if (file.is_open()) {
        bool state;
        for (int y = 0; y < gridSize; ++y) {
            for (int x = 0; x < gridSize; ++x) {
                file >> state;
                grid.setCell(x, y, state);
            }
        }
        file.close();
        saveMessage = "Game loaded successfully!";
        saveMessageClock.restart();
    }
    else {
        saveMessage = "No saved game found!";
        saveMessageClock.restart();
    }
}

void Game::drawTutorial() {
    window.clear(BACKGROUND_COLOR);

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Game of Life Concept");
    titleText.setCharacterSize(50);
    titleText.setFillColor(MENU_SELECTED_COLOR);
    titleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.width / 2.0f, titleRect.height / 2.0f);
    titleText.setPosition(1920 / 2.0f, 200);

    sf::Text conceptText;
    conceptText.setFont(font);
    conceptText.setString(
        "A cellular automaton devised by John Conway.\n"
        "Cells live, die, or multiply based on neighbors."
    );
    conceptText.setCharacterSize(32);
    conceptText.setFillColor(MENU_NORMAL_COLOR);

    sf::FloatRect conceptRect = conceptText.getLocalBounds();
    conceptText.setOrigin(conceptRect.width / 2.0f, conceptRect.height / 2.0f);
    conceptText.setPosition(1920 / 2.0f, 300);

    sf::Text controlsTitle;
    controlsTitle.setFont(font);
    controlsTitle.setString("Controls");
    controlsTitle.setCharacterSize(40);
    controlsTitle.setFillColor(MENU_SELECTED_COLOR);
    controlsTitle.setStyle(sf::Text::Bold);

    sf::FloatRect controlsTitleRect = controlsTitle.getLocalBounds();
    controlsTitle.setOrigin(controlsTitleRect.width / 2.0f, controlsTitleRect.height / 2.0f);
    controlsTitle.setPosition(1920 / 2.0f, 450);

    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setString(
        "SPACE BAR: Pause/Simulate\n"
        "1: Glider Pattern\n"
        "2: Small Exploder Pattern\n"
        "3: Spaceship Pattern\n"
        "4: Manual Mode\n"
        "S: Save Game\n"
        "ESC: Back to Menu"
    );
    controlsText.setCharacterSize(32);
    controlsText.setFillColor(MENU_NORMAL_COLOR);

    sf::FloatRect controlsRect = controlsText.getLocalBounds();
    controlsText.setOrigin(controlsRect.width / 2.0f, controlsRect.height / 2.0f);
    controlsText.setPosition(1920 / 2.0f, 650);

    window.draw(titleText);
    window.draw(conceptText);
    window.draw(controlsTitle);
    window.draw(controlsText);

    sf::Text escText;
    escText.setFont(font);
    escText.setString("Press ESC to return to menu");
    escText.setCharacterSize(24);
    escText.setFillColor(sf::Color(128, 128, 128));

    sf::FloatRect escRect = escText.getLocalBounds();
    escText.setOrigin(escRect.width / 2.0f, escRect.height / 2.0f);
    escText.setPosition(1920 / 2.0f, 900);

    window.draw(escText);
    window.display();
}
