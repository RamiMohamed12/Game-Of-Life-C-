#include "Game.h"
#include <iostream>
#include <stdexcept>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Game of Life", sf::Style::Fullscreen)
    , gridSize(50)
    , grid(50, 50)
    , isRunning(false)
    , selectedPattern("Glider")
    , manualMode(false)
    , gameState(GameState::Menu)
    , menu(window, font)
    , gridSelector(window, font)
    , currentTutorialPage(1) {

    window.setFramerateLimit(60);
    loadPatterns();

    // Chargement de la police
    if (!font.loadFromFile("C:\\Users\\THINKPAD\\AppData\\Local\\Microsoft\\Windows\\Fonts\\SourceCodePro-VariableFont_wght.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        throw std::runtime_error("Failed to load font");
    }
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();

        if (gameState == GameState::Menu) {
            menu.draw();
            if (menu.isItemSelected()) {
                processMenuSelection();
                menu.resetSelection();
            }
        }
        else if (gameState == GameState::GridSelection) {
            gridSelector.draw();
            if (gridSelector.isConfirmed()) {
                gridSize = gridSelector.getCols();
                grid = Grid(gridSelector.getCols(), gridSelector.getRows());
                gameState = GameState::Playing;
                gridSelector.reset();
                reset();
            }
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

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (gameState == GameState::Menu) {
            menu.handleEvents(event);
        }
        else if (gameState == GameState::GridSelection) {
            gridSelector.handleEvents(event);
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                gameState = GameState::Menu;
            }
        }
        else if (gameState == GameState::Tutorial) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    gameState = GameState::Menu;
                }
                else if (event.key.code == sf::Keyboard::Left && currentTutorialPage > 1) {
                    currentTutorialPage--;
                }
                else if (event.key.code == sf::Keyboard::Right && currentTutorialPage < TOTAL_TUTORIAL_PAGES) {
                    currentTutorialPage++;
                }
            }
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
                    selectedPattern = "Pulsar";
                    manualMode = false;
                }
                if (event.key.code == sf::Keyboard::Num5) {
                    manualMode = true;
                }
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
                    simulationSpeed = std::min(simulationSpeed * 1.2f, MAX_SPEED);
                }
                if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash) {
                    simulationSpeed = std::max(simulationSpeed / 1.2f, MIN_SPEED);
                }
                if (event.key.code == sf::Keyboard::Z) {
                    handleZoom(1.1f);
                }
                if (event.key.code == sf::Keyboard::X) {
                    handleZoom(0.9f);
                }
                if (event.key.code == sf::Keyboard::Left) {
                    handlePan(sf::Vector2f(50.0f, 0.0f));
                }
                if (event.key.code == sf::Keyboard::Right) {
                    handlePan(sf::Vector2f(-50.0f, 0.0f));
                }
                if (event.key.code == sf::Keyboard::Up) {
                    handlePan(sf::Vector2f(0.0f, 50.0f));
                }
                if (event.key.code == sf::Keyboard::Down) {
                    handlePan(sf::Vector2f(0.0f, -50.0f));
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f worldPos = window.mapPixelToCoords(
                        sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
                    );
                    int x = static_cast<int>(worldPos.x / CELL_SIZE);
                    int y = static_cast<int>(worldPos.y / CELL_SIZE);

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

void Game::update() {
    if (isRunning && clock.getElapsedTime().asMilliseconds() > 100 / simulationSpeed) {
        grid.updateGrid();
        generation++;
        clock.restart();
    }
    updateStats();
}

void Game::draw() {
    window.clear(BACKGROUND_COLOR);

    // Appliquer le zoom et le déplacement
    sf::View view = window.getDefaultView();
    view.zoom(1.0f / zoomLevel);
    view.move(viewOffset);
    window.setView(view);

    grid.draw(window, CELL_SIZE);

    // Revenir à la vue par défaut pour l'interface
    window.setView(window.getDefaultView());

    drawStats();

    // Barre de statut
    sf::RectangleShape statusBar(sf::Vector2f(window.getSize().x, 40));
    statusBar.setFillColor(sf::Color(24, 24, 29));
    statusBar.setPosition(0, window.getSize().y - 40);
    window.draw(statusBar);

    // Texte de statut
    sf::Text statusText;
    statusText.setFont(font);
    std::string statusString = manualMode ? "Mode: Manual" : "Pattern: " + selectedPattern;
    statusString += "    |    Space: Play/Pause    |    S: Save    |    Esc: Menu";
    statusText.setString(statusString);
    statusText.setCharacterSize(STATUS_FONT_SIZE);
    statusText.setFillColor(MENU_NORMAL_COLOR);
    statusText.setPosition(10, window.getSize().y - 30);
    window.draw(statusText);

    // Message de sauvegarde
    if (saveMessageClock.getElapsedTime() < SAVE_MESSAGE_DURATION) {
        sf::Text saveText;
        saveText.setFont(font);
        saveText.setString(saveMessage);
        saveText.setCharacterSize(STATUS_FONT_SIZE);
        saveText.setFillColor(MENU_SELECTED_COLOR);
        sf::FloatRect textRect = saveText.getLocalBounds();
        saveText.setPosition(
            (window.getSize().x - textRect.width) / 2,
            window.getSize().y * 0.1f
        );
        window.draw(saveText);
    }

    window.display();
}

void Game::drawStats() {
    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(16);
    statsText.setFillColor(MENU_NORMAL_COLOR);

    std::stringstream ss;
    ss << "FPS: " << static_cast<int>(fps)
        << " | Generation: " << generation
        << " | Living Cells: " << livingCells
        << " | Speed: x" << std::fixed << std::setprecision(2) << simulationSpeed
        << " | Zoom: x" << std::fixed << std::setprecision(2) << zoomLevel;

    statsText.setString(ss.str());
    statsText.setPosition(10, 10);
    window.draw(statsText);
}

void Game::drawTutorialPage1() {
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Game of Life - Basic Controls");
    titleText.setCharacterSize(50);
    titleText.setFillColor(MENU_SELECTED_COLOR);
    titleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.width / 2.0f, titleRect.height / 2.0f);
    titleText.setPosition(window.getSize().x / 2.0f, 200);
    window.draw(titleText);

    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setString(
        "SPACE BAR: Pause/Simulate\n"
        "1-4: Select Patterns\n"
        "5: Manual Mode\n"
        "S: Save Game\n"
        "R: Reset Grid\n"
        "+/-: Adjust Speed\n"
        "Z/X: Zoom In/Out\n"
        "Arrow Keys: Pan View"
    );
    controlsText.setCharacterSize(32);
    controlsText.setFillColor(MENU_NORMAL_COLOR);

    sf::FloatRect controlsRect = controlsText.getLocalBounds();
    controlsText.setOrigin(controlsRect.width / 2.0f, controlsRect.height / 2.0f);
    controlsText.setPosition(window.getSize().x / 2.0f, 400);
    window.draw(controlsText);
}

void Game::drawTutorialPage2() {
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Game of Life - Grid Controls");
    titleText.setCharacterSize(50);
    titleText.setFillColor(MENU_SELECTED_COLOR);
    titleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.width / 2.0f, titleRect.height / 2.0f);
    titleText.setPosition(window.getSize().x / 2.0f, 200);
    window.draw(titleText);

    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setString(
        "Grid Size Selection:\n"
        "Tab: Switch between Rows/Columns\n"
        "Up/Down: Adjust Values\n"
        "Enter: Confirm Size\n\n"
        "Mouse Controls:\n"
        "Left Click: Place Pattern/Toggle Cell\n"
        "ESC: Back to Menu"
    );
    controlsText.setCharacterSize(32);
    controlsText.setFillColor(MENU_NORMAL_COLOR);

    sf::FloatRect controlsRect = controlsText.getLocalBounds();
    controlsText.setOrigin(controlsRect.width / 2.0f, controlsRect.height / 2.0f);
    controlsText.setPosition(window.getSize().x / 2.0f, 400);
    window.draw(controlsText);
}

void Game::drawTutorialArrows() {
    sf::Text arrowText;
    arrowText.setFont(font);
    arrowText.setCharacterSize(32);
    arrowText.setFillColor(MENU_NORMAL_COLOR);

    // Flèche gauche
    if (currentTutorialPage > 1) {
        arrowText.setString("< Previous");
        arrowText.setPosition(50, window.getSize().y / 2.0f);
        window.draw(arrowText);
    }

    // Flèche droite
    if (currentTutorialPage < TOTAL_TUTORIAL_PAGES) {
        arrowText.setString("Next >");
        sf::FloatRect textRect = arrowText.getLocalBounds();
        arrowText.setPosition(window.getSize().x - textRect.width - 50, window.getSize().y / 2.0f);
        window.draw(arrowText);
    }
}

void Game::drawTutorial() {
    window.clear(BACKGROUND_COLOR);

    switch (currentTutorialPage) {
    case 1:
        drawTutorialPage1();
        break;
    case 2:
        drawTutorialPage2();
        break;
    }

    drawTutorialArrows();

    sf::Text escText;
    escText.setFont(font);
    escText.setString("Press ESC to return to menu");
    escText.setCharacterSize(24);
    escText.setFillColor(sf::Color(128, 128, 128));

    sf::FloatRect escRect = escText.getLocalBounds();
    escText.setOrigin(escRect.width / 2.0f, escRect.height / 2.0f);
    escText.setPosition(window.getSize().x / 2.0f, 900);

    window.draw(escText);
    window.display();
}

void Game::handleZoom(float delta) {
    float newZoom = zoomLevel * delta;
    if (newZoom >= MIN_ZOOM && newZoom <= MAX_ZOOM) {
        zoomLevel = newZoom;
    }
}

void Game::handlePan(const sf::Vector2f& delta) {
    viewOffset += delta;
}

void Game::updateStats() {
    float currentTime = fpsClock.restart().asSeconds();
    fps = 1.0f / currentTime;

    livingCells = 0;
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            if (grid.getCellState(x, y)) {
                livingCells++;
            }
        }
    }
}

void Game::reset() {
    grid.reset();
    isRunning = false;
    generation = 0;
    viewOffset = sf::Vector2f(0.0f, 0.0f);
    zoomLevel = 1.0f;
    simulationSpeed = 1.0f;
}

void Game::saveGame() {
    std::ofstream file("gamestate.save");
    if (file.is_open()) {
        file << gridSize << " " << gridSize << "\n";
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
        int rows, cols;
        file >> rows >> cols;
        if (rows > 0 && cols > 0) {
            gridSize = cols;
            grid = Grid(cols, rows);
            bool state;
            for (int y = 0; y < rows; ++y) {
                for (int x = 0; x < cols; ++x) {
                    file >> state;
                    grid.setCell(x, y, state);
                }
            }
            saveMessage = "Game loaded successfully!";
        }
        file.close();
        saveMessageClock.restart();
    }
    else {
        saveMessage = "No saved game found!";
        saveMessageClock.restart();
    }
}

void Game::loadPatterns() {
    patterns = Pattern::getPatterns();
}

void Game::processMenuSelection() {
    switch (menu.getSelectedItem()) {
    case 0: // Start Game
        gameState = GameState::GridSelection;
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
