#include "Game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <fstream>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Game of Life", sf::Style::Fullscreen)
    , font()
    , menu(window, font)
    , grid(calculateOptimalGridSize().x, calculateOptimalGridSize().y)
    , backgroundTexture()
    , backgroundSprite()
    , renderTexture()
    , musicPlaylist()
    , currentMusic()
    , currentTrackIndex(0)        // First music system members
    , musicVolume(50.0f)
    , isMusicPlaying(false)
    , musicInfo()
    , musicDisplayClock()
    , gameState(GameState::Menu)
    , isRunning(false)
    , selectedPattern("Glider")   // Now after the music system members
    , manualMode(false)
    , currentTutorialPage(1)
    , zoomLevel(1.0f)
    , viewOffset(0.0f, 0.0f)
    , simulationSpeed(1.0f)
    , generation(0)
    , livingCells(0)
    , fps(0.0f)
    , clock()
    , fpsClock()
    , saveMessage()
    , saveMessageClock()
    , patterns()
    , lastMousePos()
{
    window.setFramerateLimit(60);
    
    if (!font.loadFromFile("/home/ramimohamed/.local/share/fonts/Ubuntu-M.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        throw std::runtime_error("Failed to load font");
    }

    // Initialize systems
    loadPatterns();
    initMusicSystem();
    initBackground();

    // Initialize music info text
    musicInfo.setFont(font);
    musicInfo.setCharacterSize(20);
    musicInfo.setFillColor(MENU_NORMAL_COLOR);
}


void Game::initBackground() {
    if (!renderTexture.create(window.getSize().x, window.getSize().y)) {
        throw std::runtime_error("Failed to create render texture");
    }
    backgroundSprite.setTexture(renderTexture.getTexture());
}

void Game::initMusicSystem() {
    loadMusicPlaylist();
    currentMusic.setVolume(musicVolume);
    if (!musicPlaylist.empty() && currentMusic.openFromFile(musicPlaylist[0])) {
        currentMusic.setLoop(true);
    }
}

void Game::loadMusicPlaylist() {
    musicPlaylist = {
        "assets/music/track1.ogg",
        "assets/music/track2.ogg",
        "assets/music/track3.ogg"
    };
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        updateBackground();
        updateMusicInfo();

        if (gameState == GameState::Menu) {
            menu.draw();
            drawMusicInfo();
            if (menu.isItemSelected()) {
                processMenuSelection();
                menu.resetSelection();
            }
        }
        else if (gameState == GameState::Tutorial) {
            drawTutorial();
            drawMusicInfo();
        }
        else if (gameState == GameState::Playing) {
            update();
            draw();
            drawMusicInfo();
        }
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        // Global music controls
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::M:
                    toggleMusic();
                    break;
                case sf::Keyboard::N:
                    playNextTrack();
                    break;
                case sf::Keyboard::P:
                    playPreviousTrack();
                    break;
                case sf::Keyboard::PageUp:
                    updateMusicVolume(5.0f);
                    break;
                case sf::Keyboard::PageDown:
                    updateMusicVolume(-5.0f);
                    break;
                default:
                    break;
            }
        }

        // State-specific controls
        switch (gameState) {
            case GameState::Menu:
                menu.handleEvents(event);
                break;

            case GameState::Tutorial:
                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            gameState = GameState::Menu;
                            break;
                        case sf::Keyboard::Tab:
                            currentTutorialPage = (currentTutorialPage % TOTAL_TUTORIAL_PAGES) + 1;
                            break;
                        default:
                            break;
                    }
                }
                break;

            case GameState::Playing:
                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            gameState = GameState::Menu;
                            break;
                        case sf::Keyboard::Space:
                            isRunning = !isRunning;
                            break;
                        case sf::Keyboard::S:
                            saveGame();
                            break;
                        case sf::Keyboard::R:
                            grid.reset();
                            generation = 0;
                            break;
                        case sf::Keyboard::Z:
                            handleZoom(1.0f);
                            break;
                        case sf::Keyboard::X:
                            handleZoom(-1.0f);
                            break;
                        case sf::Keyboard::Left:
                            handlePan(sf::Vector2f(-20.0f, 0.0f));
                            break;
                        case sf::Keyboard::Right:
                            handlePan(sf::Vector2f(20.0f, 0.0f));
                            break;
                        case sf::Keyboard::Up:
                            handlePan(sf::Vector2f(0.0f, -20.0f));
                            break;
                        case sf::Keyboard::Down:
                            handlePan(sf::Vector2f(0.0f, 20.0f));
                            break;
                        case sf::Keyboard::Add:
                        case sf::Keyboard::Equal:
                            simulationSpeed = clamp(simulationSpeed + 0.1f, MIN_SPEED, MAX_SPEED);
                            break;
                        case sf::Keyboard::Subtract:
                        case sf::Keyboard::Dash:
                            simulationSpeed = clamp(simulationSpeed - 0.1f, MIN_SPEED, MAX_SPEED);
                            break;
                        case sf::Keyboard::Num1:
                        case sf::Keyboard::Num2:
                        case sf::Keyboard::Num3:
                        case sf::Keyboard::Num4:
                            {
                                const std::vector<std::string> patternNames = {
                                    "Glider", "Small Exploder", "Spaceship", "Pulsar"
                                };
                                size_t index = static_cast<size_t>(event.key.code - sf::Keyboard::Num1);
                                if (index < patternNames.size()) {
                                    selectedPattern = patternNames[index];
                                    manualMode = false;
                                }
                            }
                            break;
                        case sf::Keyboard::Num5:
                            manualMode = true;
                            break;
                        case sf::Keyboard::Num6:
                            loadCustomPattern();
                            break;
                        default:
                            break;
                    }
                }

                // Mouse controls (including zoom and pan) only active during gameplay
                handleMouseEvents(event);
                break;
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

void Game::updateBackground() {
    static sf::Clock animClock;
    float time = animClock.getElapsedTime().asSeconds();

    renderTexture.clear(BACKGROUND_COLOR);

    // Effet de dégradé dynamique
    for (int i = 0; i < 5; i++) {
        sf::RectangleShape gradient(sf::Vector2f(window.getSize().x, window.getSize().y / 5.f));
        float alpha = 255.f * (1.f - (i / 5.f));
        gradient.setFillColor(sf::Color(
            ACCENT_COLOR.r,
            ACCENT_COLOR.g,
            ACCENT_COLOR.b,
            static_cast<sf::Uint8>(alpha * 0.05f)
        ));
        gradient.setPosition(0, i * window.getSize().y / 5.f);
        renderTexture.draw(gradient);
    }

    // Cercles animés
    for (int i = 0; i < 3; i++) {
        sf::CircleShape circle(100.f + i * 50.f);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setPosition(
            window.getSize().x * 0.8f + std::cos(time + i) * 20.f,
            window.getSize().y * 0.2f + std::sin(time + i) * 20.f
        );
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(2.f);
        circle.setOutlineColor(sf::Color(MENU_SELECTED_COLOR.r, MENU_SELECTED_COLOR.g, MENU_SELECTED_COLOR.b, 30));
        renderTexture.draw(circle);
    }

    renderTexture.display();
}

void Game::draw() {
    window.clear(BACKGROUND_COLOR);
    window.draw(backgroundSprite);

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
    statusBar.setFillColor(sf::Color(24, 24, 29, 200));
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
        saveText.setOrigin(textRect.width / 2.0f, 0);
        saveText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.1f);
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
void Game::drawTutorial() {
    window.clear(BACKGROUND_COLOR);
    window.draw(backgroundSprite);

    drawTutorialHeader();

    switch (currentTutorialPage) {
    case 1: drawTutorialPage1(); break;
    case 2: drawTutorialPage2(); break;
    case 3: drawTutorialPage3(); break;
    case 4: drawTutorialPage4(); break;
    case 5: drawTutorialPage5(); break;
    }

    drawTutorialNavigation();
    drawTutorialProgress();
    window.display();
}

void Game::drawTutorialHeader() {
    sf::Text headerText;
    headerText.setFont(font);
    headerText.setCharacterSize(TUTORIAL_TITLE_SIZE);
    headerText.setStyle(sf::Text::Bold);
    headerText.setFillColor(MENU_SELECTED_COLOR);

    switch (currentTutorialPage) {
    case 1: headerText.setString("Welcome to Game of Life"); break;
    case 2: headerText.setString("Basic Controls"); break;
    case 3: headerText.setString("Game Rules"); break;
    case 4: headerText.setString("Patterns"); break;
    case 5: headerText.setString("Advanced Features"); break;
    }

    sf::FloatRect headerRect = headerText.getLocalBounds();
    headerText.setOrigin(headerRect.width / 2.0f, headerRect.height / 2.0f);
    headerText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.15f);

    window.draw(headerText);
}

void Game::drawTutorialPage1() {
    sf::Text introText;
    introText.setFont(font);
    introText.setCharacterSize(TUTORIAL_TEXT_SIZE);
    introText.setFillColor(MENU_NORMAL_COLOR);
    introText.setString(
        "Conway's Game of Life is a cellular automaton\n"
        "that simulates the evolution of cells based on\n"
        "simple rules. Watch as patterns emerge and\n"
        "evolve in fascinating ways!\n\n"
        "Press TAB to navigate through the tutorial pages."
    );

    sf::FloatRect textRect = introText.getLocalBounds();
    introText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    introText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.45f);

    window.draw(introText);
}

void Game::drawTutorialPage2() {
    std::vector<std::pair<std::string, std::string>> controls = {
        {"SPACE", "Play/Pause Simulation"},
        {"R", "Reset Grid"},
        {"ESC", "Return to Menu"},
        {"1-4", "Select Patterns"},
        {"5", "Manual Cell Mode"},
        {"M", "Toggle Music"},
        {"N/P", "Next/Previous Track"},
        {"Page Up/Down", "Adjust Volume"},
        {"Z/X", "Zoom In/Out"},
        {"Arrows", "Pan View"}
    };

    float startY = window.getSize().y * 0.35f;
    float spacing = 45.0f;

    for (size_t i = 0; i < controls.size(); i++) {
        sf::Text controlText;
        controlText.setFont(font);
        controlText.setString(controls[i].first + " - " + controls[i].second);
        controlText.setCharacterSize(TUTORIAL_TEXT_SIZE);
        controlText.setFillColor(MENU_NORMAL_COLOR);

        sf::FloatRect textRect = controlText.getLocalBounds();
        controlText.setOrigin(textRect.width / 2.0f, 0);
        controlText.setPosition(window.getSize().x / 2.0f, startY + i * spacing);

        window.draw(controlText);
    }
}

void Game::drawTutorialPage3() {
    std::vector<std::string> rules = {
        "1. Any live cell with fewer than two live",
        "   neighbors dies (underpopulation)",
        "",
        "2. Any live cell with two or three live",
        "   neighbors survives",
        "",
        "3. Any live cell with more than three live",
        "   neighbors dies (overpopulation)",
        "",
        "4. Any dead cell with exactly three live",
        "   neighbors becomes alive (reproduction)"
    };

    float startY = window.getSize().y * 0.35f;
    float spacing = 35.0f;

    for (size_t i = 0; i < rules.size(); i++) {
        sf::Text ruleText;
        ruleText.setFont(font);
        ruleText.setString(rules[i]);
        ruleText.setCharacterSize(TUTORIAL_TEXT_SIZE);
        ruleText.setFillColor(MENU_NORMAL_COLOR);

        sf::FloatRect textRect = ruleText.getLocalBounds();
        ruleText.setOrigin(textRect.width / 2.0f, 0);
        ruleText.setPosition(window.getSize().x / 2.0f, startY + i * spacing);

        window.draw(ruleText);
    }
}

void Game::drawTutorialPage4() {
    sf::Text patternsText;
    patternsText.setFont(font);
    patternsText.setCharacterSize(TUTORIAL_TEXT_SIZE);
    patternsText.setFillColor(MENU_NORMAL_COLOR);
    patternsText.setString(
        "Available Patterns (Press 1-6 to select):\n\n"
        "1. Glider - A simple moving pattern\n"
        "2. Small Exploder - Creates interesting chaos\n"
        "3. Spaceship - Another moving pattern\n"
        "4. Pulsar - A complex oscillating pattern\n"
        "5. Manual Mode - Place cells individually\n"
        "6. Custom Pattern - Load from custom_pattern.txt"
    );

    sf::FloatRect textRect = patternsText.getLocalBounds();
    patternsText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    patternsText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.45f);

    window.draw(patternsText);
}


void Game::drawTutorialPage5() {
    sf::Text advancedText;
    advancedText.setFont(font);
    advancedText.setCharacterSize(TUTORIAL_TEXT_SIZE);
    advancedText.setFillColor(MENU_NORMAL_COLOR);
    advancedText.setString(
        "Advanced Features:\n\n"
        "• Save your creations with 'S' key\n"
        "• Load previous saves from the menu\n"
        "• Adjust simulation speed with +/-\n"
        "• Use manual mode for custom patterns\n"
        "• Zoom and pan to explore large patterns\n"
        "• Control music playback with M, N, P\n"
        "• Adjust volume with Page Up/Down"
    );

    sf::FloatRect textRect = advancedText.getLocalBounds();
    advancedText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    advancedText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.45f);

    window.draw(advancedText);
}

void Game::drawTutorialProgress() {
    float dotSpacing = 20.0f;
    float totalWidth = (TOTAL_TUTORIAL_PAGES - 1) * dotSpacing;
    float startX = (window.getSize().x - totalWidth) / 2.0f;

    for (int i = 0; i < TOTAL_TUTORIAL_PAGES; i++) {
        sf::CircleShape dot(4.0f);
        dot.setOrigin(4.0f, 4.0f);
        dot.setPosition(startX + i * dotSpacing, window.getSize().y * 0.85f);

        if (i + 1 == currentTutorialPage) {
            dot.setFillColor(MENU_SELECTED_COLOR);
            dot.setRadius(6.0f);
            dot.setOrigin(6.0f, 6.0f);
        }
        else {
            dot.setFillColor(sf::Color(128, 128, 128));
        }

        window.draw(dot);
    }
}

void Game::drawTutorialNavigation() {
    sf::Text escText;
    escText.setFont(font);
    escText.setString("Press ESC to return to menu  |  TAB to change page");
    escText.setCharacterSize(20);
    escText.setFillColor(sf::Color(128, 128, 128));
    sf::FloatRect escRect = escText.getLocalBounds();
    escText.setOrigin(escRect.width / 2.0f, escRect.height / 2.0f);
    escText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.95f);
    window.draw(escText);
}

// Music System Functions
void Game::updateMusicInfo() {
    if (musicDisplayClock.getElapsedTime() < MUSIC_INFO_DISPLAY_TIME) {
        std::string musicStatus = isMusicPlaying ? "Playing: " : "Paused: ";
        if (!musicPlaylist.empty()) {
            size_t lastSlash = musicPlaylist[currentTrackIndex].find_last_of("/\\");
            std::string trackName = musicPlaylist[currentTrackIndex].substr(lastSlash + 1);
            musicInfo.setString(musicStatus + trackName + "\nVolume: " + std::to_string(static_cast<int>(musicVolume)) + "%");
        }
    }
}

void Game::drawMusicInfo() {
    if (musicDisplayClock.getElapsedTime() < MUSIC_INFO_DISPLAY_TIME) {
        sf::FloatRect textRect = musicInfo.getLocalBounds();
        musicInfo.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
        musicInfo.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.05f);
        window.draw(musicInfo);
    }
}

void Game::playNextTrack() {
    if (musicPlaylist.empty()) return;
    currentTrackIndex = (currentTrackIndex + 1) % musicPlaylist.size();
    if (currentMusic.openFromFile(musicPlaylist[currentTrackIndex])) {
        currentMusic.setVolume(musicVolume);
        currentMusic.play();
        isMusicPlaying = true;
        musicDisplayClock.restart();
    }
}

void Game::playPreviousTrack() {
    if (musicPlaylist.empty()) return;
    currentTrackIndex = (currentTrackIndex == 0) ? musicPlaylist.size() - 1 : currentTrackIndex - 1;
    if (currentMusic.openFromFile(musicPlaylist[currentTrackIndex])) {
        currentMusic.setVolume(musicVolume);
        currentMusic.play();
        isMusicPlaying = true;
        musicDisplayClock.restart();
    }
}

void Game::toggleMusic() {
    if (isMusicPlaying) {
        currentMusic.pause();
        isMusicPlaying = false;
    }
    else {
        currentMusic.play();
        isMusicPlaying = true;
    }
    musicDisplayClock.restart();
}

void Game::updateMusicVolume(float delta) {
    musicVolume = clamp(musicVolume + delta, 0.0f, 100.0f);
    currentMusic.setVolume(musicVolume);
    musicDisplayClock.restart();
}

sf::Vector2i Game::calculateOptimalGridSize() const {
    const int targetCellSize = 15;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int cols = desktop.width / targetCellSize;
    int rows = desktop.height / targetCellSize;
    return sf::Vector2i(cols, rows);
}
void Game::updateStats() {
    float currentTime = fpsClock.restart().asSeconds();
    fps = 1.0f / currentTime;

    livingCells = 0;
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            if (grid.getCellState(x, y)) {
                livingCells++;
            }
        }
    }
}

void Game::saveGame() {
    std::ofstream file("gamestate.save");
    if (file.is_open()) {
        file << grid.getWidth() << " " << grid.getHeight() << "\n";
        for (int y = 0; y < grid.getHeight(); ++y) {
            for (int x = 0; x < grid.getWidth(); ++x) {
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

void Game::reset() {
    grid.reset();
    isRunning = false;
    generation = 0;
    viewOffset = sf::Vector2f(0.0f, 0.0f);
    zoomLevel = 1.0f;
    simulationSpeed = 1.0f;
}


void Game::processMenuSelection() {
    switch (menu.getSelectedItem()) {
    case 0: // Start Game
        reset();  // Reset the game state before starting
        grid = Grid(calculateOptimalGridSize().x, calculateOptimalGridSize().y);  // Create a new grid
        gameState = GameState::Playing;
        break;
    case 1: // Load Game
        loadGame();
        gameState = GameState::Playing;
        break;
    case 2: // Tutorial
        gameState = GameState::Tutorial;
        currentTutorialPage = 1;
        break;
    case 3: // Exit
        window.close();
        break;
    }
}


void Game::handleZoom(float delta) {
    // Calculate new zoom level
    float zoomFactor = (delta > 0) ? 1.1f : 0.9f;
    float newZoom = zoomLevel * zoomFactor;
    
    // Clamp zoom level between MIN_ZOOM and MAX_ZOOM
    if (newZoom >= MIN_ZOOM && newZoom <= MAX_ZOOM) {
        // Get current mouse position
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f beforeCoord = window.mapPixelToCoords(mousePos);
        
        zoomLevel = newZoom;
        
        // Apply zoom to view
        sf::View view = window.getDefaultView();
        view.zoom(1.0f / zoomLevel);
        view.move(viewOffset);
        window.setView(view);
        
        // Get new world coordinate of mouse
        sf::Vector2f afterCoord = window.mapPixelToCoords(mousePos);
        
        // Adjust view offset to zoom towards mouse position
        viewOffset += beforeCoord - afterCoord;
    }
}




void Game::loadPatterns() {
    patterns = Pattern::getPatterns();  // Utilise votre classe Pattern existante
}

void Game::loadGame() {
    std::ifstream file("gamestate.save");
    if (file.is_open()) {
        int width, height;
        file >> width >> height;

        if (width > 0 && height > 0) {
            grid = Grid(width, height);
            bool state;
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
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


void Game::loadCustomPattern() {
    std::string filename = "custom_pattern.txt";
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        saveMessage = "Failed to open custom pattern file!";
        saveMessageClock.restart();
        return;
    }
    
    int height, width;
    if (!(file >> height >> width)) {
        saveMessage = "Invalid custom pattern format!";
        saveMessageClock.restart();
        file.close();
        return;
    }
    
    if (height <= 0 || width <= 0 || height > 100 || width > 100) {
        saveMessage = "Invalid pattern dimensions!";
        saveMessageClock.restart();
        file.close();
        return;
    }
    
    std::vector<std::vector<bool>> customPattern(height, std::vector<bool>(width));
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int cell;
            if (!(file >> cell) || (cell != 0 && cell != 1)) {
                saveMessage = "Invalid pattern data!";
                saveMessageClock.restart();
                file.close();
                return;
            }
            customPattern[i][j] = cell == 1;
        }
    }
    
    file.close();
    patterns["Custom"] = customPattern;
    selectedPattern = "Custom";
    manualMode = false;
    
    saveMessage = "Custom pattern loaded successfully!";
    saveMessageClock.restart();
}

void Game::handleMouseEvents(const sf::Event& event) {
    static bool isDragging = false;  // Add this to track if we're currently dragging

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Left click handling for cells/patterns
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

            int gridX = static_cast<int>(worldPos.x / CELL_SIZE);
            int gridY = static_cast<int>(worldPos.y / CELL_SIZE);

            if (manualMode) {
                if (gridX >= 0 && gridX < grid.getWidth() &&
                    gridY >= 0 && gridY < grid.getHeight()) {
                    grid.setCell(gridX, gridY, !grid.getCellState(gridX, gridY));
                }
            } else if (patterns.find(selectedPattern) != patterns.end()) {
                Pattern::insertPattern(grid, patterns[selectedPattern], gridX, gridY);
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            isDragging = true;
            lastMousePos = sf::Mouse::getPosition(window);
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Right) {
            isDragging = false;
        }
    }

    if (event.type == sf::Event::MouseMoved) {
        if (isDragging) {
            sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
            sf::Vector2f delta = sf::Vector2f(
                static_cast<float>(currentMousePos.x - lastMousePos.x),
                static_cast<float>(currentMousePos.y - lastMousePos.y)
            );

            // Invert the delta for more intuitive dragging
            delta = -delta;

            // Scale the movement based on zoom level
            handlePan(delta);
            lastMousePos = currentMousePos;
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        handleZoom(event.mouseWheelScroll.delta);
    }
}

void Game::handlePan(const sf::Vector2f& delta) {
    // Scale the movement based on zoom level and cell size
    float moveSpeed = 1.0f / zoomLevel;
    viewOffset += delta * moveSpeed;

    // Update the view
    sf::View view = window.getDefaultView();
    view.zoom(1.0f / zoomLevel);
    view.move(viewOffset);
    window.setView(view);
}