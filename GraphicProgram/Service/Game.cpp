#include "Game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cstdlib>  
#include <ctime>    
#include <unistd.h>
// Static member definitions
const sf::Color Game::STATS_PANEL_BG = sf::Color(20, 20, 35, 230);
const int Game::STATS_LABEL_SIZE = 14;
const int Game::STATS_VALUE_SIZE = 16;


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

    if (!font.loadFromFile("assets/fonts/Ubuntu-M.ttf")) {
    if (!font.loadFromFile("../assets/fonts/Ubuntu-M.ttf")) {
        std::cerr << "Error loading font from: " << std::endl;
        std::cerr << "  - assets/fonts/Ubuntu-M.ttf" << std::endl;
        std::cerr << "  - ../assets/fonts/Ubuntu-M.ttf" << std::endl;
        throw std::runtime_error("Failed to load font");
    }
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

    // Set initial volume
    currentMusic.setVolume(musicVolume);

    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Select random track
    if (!musicPlaylist.empty()) {
        currentTrackIndex = std::rand() % musicPlaylist.size();
        if (currentMusic.openFromFile(musicPlaylist[currentTrackIndex])) {
            currentMusic.setLoop(true);
            currentMusic.play();
            isMusicPlaying = true;
            musicDisplayClock.restart();
        }
    }
}

void Game::loadMusicPlaylist() {
    // Use relative path
    std::string basePath = "assets/music/";
    // Alternative path if the above doesn't work
    std::string altPath = "../assets/music/";
    
    std::vector<std::string> paths;
    
    // Try both paths to see which one works
    std::ifstream test(basePath + "output.ogg");
    if (test.good()) {
        paths = {
            basePath + "output.ogg",
            basePath + "output2.ogg",
            basePath + "moments.ogg",
            basePath + "optimism.ogg",
            basePath + "rai.ogg",
            basePath + "rapchill.ogg"
        };
    } else {
        paths = {
            altPath + "output.ogg",
            altPath + "output2.ogg",
            altPath + "moments.ogg",
            altPath + "optimism.ogg",
            altPath + "rai.ogg",
            altPath + "rapchill.ogg"
        };
    }
    
    // Clear existing playlist
    musicPlaylist.clear();
    
    // Only add files that exist to the playlist
    for (const auto& path : paths) {
        std::ifstream fileTest(path);
        if (fileTest.good()) {
            std::cout << "Adding to playlist: " << path << std::endl;
            musicPlaylist.push_back(path);
        } else {
            std::cerr << "Cannot access: " << path << std::endl;
        }
        fileTest.close();
    }

    musicTrackNames = {
        "Track 1 - Walk Away",
        "Track 2 - DUALSHOCK",
        "Track 3 - Moments",
        "Track 4 - Optimism",
        "Track 5 - A Night In Oran",
        "Track 6 - Chilling In Rain"
    };

    std::cout << "Successfully loaded " << musicPlaylist.size() << " tracks" << std::endl;
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        updateBackground();
        updateMusicInfo();

        switch (gameState) {
        case GameState::Menu:
            menu.draw();
            drawMusicInfo();
            if (menu.isItemSelected()) {
                processMenuSelection();
                menu.resetSelection();
            }
            break;
        case GameState::MusicSettings:
            drawMusicSettings();
            break;
        case GameState::Tutorial:
            drawTutorial();
            drawMusicInfo();
            break;
        case GameState::Playing:
            update();
            draw();
            drawMusicInfo();
            break;
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

        case GameState::MusicSettings:
            handleMusicSettingsEvents(event);
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
                    int index = event.key.code - sf::Keyboard::Num1;
                    std::vector<std::string> patternNames = { "Glider", "Small Exploder", "Spaceship", "Pulsar" };
                    if (index >= 0 && index < static_cast<int>(patternNames.size())) {
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

            // Mouse controls for Playing state
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    int gridX = static_cast<int>(worldPos.x / CELL_SIZE);
                    int gridY = static_cast<int>(worldPos.y / CELL_SIZE);

                    if (manualMode) {
                        if (gridX >= 0 && gridX < grid.getWidth() &&
                            gridY >= 0 && gridY < grid.getHeight()) {
                            grid.setCell(gridX, gridY, !grid.getCellState(gridX, gridY));
                        }
                    }
                    else if (patterns.find(selectedPattern) != patterns.end()) {
                        Pattern::insertPattern(grid, patterns[selectedPattern], gridX, gridY);
                    }
                }
            }

            // Zoom and pan controls
            if (event.type == sf::Event::MouseWheelScrolled) {
                handleZoom(event.mouseWheelScroll.delta);
            }

            if (event.type == sf::Event::MouseMoved) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f delta = sf::Vector2f(
                        static_cast<float>(lastMousePos.x - currentMousePos.x),
                        static_cast<float>(lastMousePos.y - currentMousePos.y)
                    );
                    handlePan(delta);
                }
                lastMousePos = sf::Mouse::getPosition(window);
            }
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
    // Create main stats panel
    sf::RectangleShape statsPanel;
    statsPanel.setSize(sf::Vector2f(window.getSize().x * 0.4f, 40.0f));
    statsPanel.setPosition(10, 10);
    statsPanel.setFillColor(sf::Color(20, 20, 35, 230));  // Using direct color value
    statsPanel.setOutlineColor(ACCENT_COLOR);
    statsPanel.setOutlineThickness(1.0f);
    window.draw(statsPanel);

    // Create individual stat boxes
    struct StatInfo {
        std::string label;
        std::string value;
        sf::Color color;
    };

    std::vector<StatInfo> stats = {
        {"FPS", std::to_string(static_cast<int>(fps)), 
            sf::Color(86, 195, 255)},  // Blue
        {"GEN", std::to_string(generation), 
            sf::Color(255, 171, 76)},  // Orange
        {"CELLS", std::to_string(livingCells), 
            sf::Color(130, 255, 157)}, // Green
        {"SPEED", "x" + std::to_string(static_cast<int>(simulationSpeed * 100) / 100.0f), 
            sf::Color(255, 110, 110)}, // Red
        {"ZOOM", "x" + std::to_string(static_cast<int>(zoomLevel * 100) / 100.0f), 
            sf::Color(206, 147, 255)}  // Purple
    };

    float startX = 20;
    float spacing = statsPanel.getSize().x / stats.size();

    for (size_t i = 0; i < stats.size(); i++) {
        // Label
        sf::Text labelText;
        labelText.setFont(font);
        labelText.setString(stats[i].label);
        labelText.setCharacterSize(STATS_LABEL_SIZE);
        labelText.setFillColor(sf::Color(180, 180, 180));
        labelText.setPosition(startX + (spacing * i), 15);
        window.draw(labelText);

        // Value
        sf::Text valueText;
        valueText.setFont(font);
        valueText.setString(stats[i].value);
        valueText.setCharacterSize(STATS_VALUE_SIZE);
        valueText.setFillColor(stats[i].color);
        valueText.setPosition(startX + (spacing * i), 30);
        window.draw(valueText);
    }

    // Draw pattern name if not in manual mode
    if (!manualMode) {
        sf::Text patternText;
        patternText.setFont(font);
        patternText.setString("PATTERN: " + selectedPattern);
        patternText.setCharacterSize(STATS_VALUE_SIZE);
        patternText.setFillColor(MENU_SELECTED_COLOR);
        patternText.setPosition(window.getSize().x - 200, 20);
        window.draw(patternText);
    }
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
    }

    sf::FloatRect headerRect = headerText.getLocalBounds();
    headerText.setOrigin(headerRect.width / 2.0f, headerRect.height / 2.0f);
    headerText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.15f);

    window.draw(headerText);
}

void Game::drawTutorialPage1() {
    // Futuristic background panel
    sf::RectangleShape mainPanel;
    mainPanel.setSize(sf::Vector2f(window.getSize().x * 0.8f, window.getSize().y * 0.7f));
    mainPanel.setPosition(window.getSize().x * 0.1f, window.getSize().y * 0.15f);
    mainPanel.setFillColor(sf::Color(20, 20, 35, 230));
    mainPanel.setOutlineColor(ACCENT_COLOR);
    mainPanel.setOutlineThickness(2.0f);
    window.draw(mainPanel);

    // Animated hexagonal patterns in background
    static float time = 0;
    time += 0.016f;
    
    // Draw subtle hexagonal grid in background
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            sf::CircleShape hex(15, 6);
            float x = window.getSize().x * (0.15f + i * 0.1f);
            float y = window.getSize().y * (0.25f + j * 0.1f);
            hex.setPosition(x, y);
            hex.setRotation(30 + std::sin(time + i * 0.5f + j * 0.3f) * 5);
            hex.setFillColor(sf::Color::Transparent);
            hex.setOutlineColor(sf::Color(ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, 20));
            hex.setOutlineThickness(1.0f);
            window.draw(hex);
        }
    }

    // Main title with sci-fi effect
    sf::Text title;
    title.setFont(font);
    title.setString("GAME OF LIFE");
    title.setCharacterSize(48);
    title.setFillColor(MENU_SELECTED_COLOR);
    title.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.25f);
    title.setOrigin(title.getLocalBounds().width / 2.0f, 0);
    title.setOutlineColor(sf::Color(ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, 
        100 + static_cast<int>(50 * std::sin(time * 2.0f))));
    title.setOutlineThickness(2.0f + std::sin(time * 2.0f));
    window.draw(title);

    // Futuristic subtitle
    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setString("< CELLULAR AUTOMATION SYSTEM v2.0 >");
    subtitle.setCharacterSize(24);
    subtitle.setFillColor(ACCENT_COLOR);
    subtitle.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.35f);
    subtitle.setOrigin(subtitle.getLocalBounds().width / 2.0f, 0);
    window.draw(subtitle);

    // Description panels with hover effect
    std::vector<std::string> descriptions = {
        "INITIALIZE: Complex life patterns emerge from simple rules",
        "OBSERVE: Real-time cellular evolution simulation",
        "CREATE: Design and experiment with custom patterns",
        "EXPLORE: Discover mathematical beauty in chaos"
    };

    float startY = window.getSize().y * 0.45f;
    for (size_t i = 0; i < descriptions.size(); i++) {
        // Panel background
        sf::RectangleShape panel;
        panel.setSize(sf::Vector2f(window.getSize().x * 0.6f, 35.0f));
        panel.setPosition(window.getSize().x * 0.2f, startY);
        panel.setFillColor(sf::Color(MENU_SELECTED_COLOR.r, MENU_SELECTED_COLOR.g, MENU_SELECTED_COLOR.b, 20));
        panel.setOutlineColor(sf::Color(ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, 50));
        panel.setOutlineThickness(1.0f);
        window.draw(panel);

        // Description text
        sf::Text descText;
        descText.setFont(font);
        descText.setString(descriptions[i]);
        descText.setCharacterSize(22);
        descText.setFillColor(MENU_NORMAL_COLOR);
        descText.setPosition(window.getSize().x * 0.22f, startY + 5);
        
        // Pulse effect for text
        float pulse = std::sin(time * 2.0f + i * 0.5f) * 0.2f + 0.8f;
        descText.setFillColor(sf::Color(
            MENU_NORMAL_COLOR.r,
            MENU_NORMAL_COLOR.g,
            MENU_NORMAL_COLOR.b,
            static_cast<sf::Uint8>(255 * pulse)));
        
        window.draw(descText);
        startY += 45;
    }

    // Navigation prompt with scanning effect
    sf::Text navText;
    navText.setFont(font);
    navText.setString("[ TAB ] NEXT PAGE");
    navText.setCharacterSize(24);
    float alpha = 128 + static_cast<int>(127 * std::sin(time * 3.0f));
    navText.setFillColor(sf::Color(MENU_SELECTED_COLOR.r, MENU_SELECTED_COLOR.g, MENU_SELECTED_COLOR.b, alpha));
    navText.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.8f);
    navText.setOrigin(navText.getLocalBounds().width / 2.0f, 0);
    window.draw(navText);
}

void Game::drawTutorialPage2() {
    // Main panel with sci-fi border
    sf::RectangleShape mainPanel;
    mainPanel.setSize(sf::Vector2f(window.getSize().x * 0.8f, window.getSize().y * 0.7f));
    mainPanel.setPosition(window.getSize().x * 0.1f, window.getSize().y * 0.15f);
    mainPanel.setFillColor(sf::Color(20, 20, 35, 230));
    mainPanel.setOutlineColor(ACCENT_COLOR);
    mainPanel.setOutlineThickness(2.0f);
    window.draw(mainPanel);

    // Title with tech effect
    sf::Text title;
    title.setFont(font);
    title.setString("SYSTEM CONTROLS");
    title.setCharacterSize(36);
    title.setFillColor(MENU_SELECTED_COLOR);
    title.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.25f);
    title.setOrigin(title.getLocalBounds().width / 2.0f, 0);
    title.setOutlineColor(ACCENT_COLOR);
    title.setOutlineThickness(1.0f);
    window.draw(title);

    struct Control {
        std::string key;
        std::string description;
        bool isPrimary;
    };

    std::vector<Control> controls = {
        {"[SPACE]", "Initialize/Pause Simulation", true},
        {"[R]", "Reset Grid Matrix", true},
        {"[ESC]", "Return to Command Center", true},
        {"[1-4]", "Select Pattern Algorithms", false},
        {"[5]", "Manual Cell Configuration", false},
        {"[SCROLL]", "Adjust Viewport Scale", false},
        {"[R-CLICK]", "Navigate Viewport", false},
        {"[L-CLICK]", "Deploy Cells/Patterns", true},
        {"[M]", "Toggle Audio System", false},
        {"[N/P]", "Audio Track Selection", false}
    };

    float startY = window.getSize().y * 0.35f;
    float spacing = 35.0f;

    static float time = 0;
    time += 0.016f;

    for (size_t i = 0; i < controls.size(); i++) {
        // Control panel with hover effect
        sf::RectangleShape controlPanel;
        controlPanel.setSize(sf::Vector2f(window.getSize().x * 0.6f, 30.0f));
        controlPanel.setPosition(window.getSize().x * 0.2f, startY);
        
        // Animated highlight for primary controls
        float alpha = controls[i].isPrimary ? 
            40 + static_cast<int>(20 * std::sin(time * 2.0f + i * 0.5f)) : 20;
        
        controlPanel.setFillColor(sf::Color(MENU_SELECTED_COLOR.r, 
                                          MENU_SELECTED_COLOR.g, 
                                          MENU_SELECTED_COLOR.b, 
                                          static_cast<sf::Uint8>(alpha)));
        controlPanel.setOutlineColor(sf::Color(ACCENT_COLOR.r, 
                                             ACCENT_COLOR.g, 
                                             ACCENT_COLOR.b, 30));
        controlPanel.setOutlineThickness(1.0f);
        window.draw(controlPanel);

        // Key binding
        sf::Text keyText;
        keyText.setFont(font);
        keyText.setString(controls[i].key);
        keyText.setCharacterSize(22);
        keyText.setFillColor(controls[i].isPrimary ? MENU_SELECTED_COLOR : ACCENT_COLOR);
        keyText.setPosition(window.getSize().x * 0.22f, startY + 5);
        window.draw(keyText);

        // Command description
        sf::Text descText;
        descText.setFont(font);
        descText.setString(controls[i].description);
        descText.setCharacterSize(20);
        descText.setFillColor(MENU_NORMAL_COLOR);
        descText.setPosition(window.getSize().x * 0.35f, startY + 5);
        window.draw(descText);

        startY += spacing;
    }
}


void Game::drawTutorialPage3() {
    // Main panel with sci-fi border
    sf::RectangleShape mainPanel;
    mainPanel.setSize(sf::Vector2f(window.getSize().x * 0.8f, window.getSize().y * 0.7f));
    mainPanel.setPosition(window.getSize().x * 0.1f, window.getSize().y * 0.15f);
    mainPanel.setFillColor(sf::Color(20, 20, 35, 230));
    mainPanel.setOutlineColor(ACCENT_COLOR);
    mainPanel.setOutlineThickness(2.0f);
    window.draw(mainPanel);

    // Title
    sf::Text title;
    title.setFont(font);
    title.setString("SYSTEM PROTOCOLS");
    title.setCharacterSize(36);
    title.setFillColor(MENU_SELECTED_COLOR);
    title.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.25f);
    title.setOrigin(title.getLocalBounds().width / 2.0f, 0);
    title.setOutlineColor(ACCENT_COLOR);
    title.setOutlineThickness(1.0f);
    window.draw(title);

    static float time = 0;
    time += 0.016f;

    struct Rule {
        std::string title;
        std::string description;
    };

    std::vector<Rule> rules = {
        {"PROTOCOL 1: ISOLATION", "Cell termination if neighbors < 2"},
        {"PROTOCOL 2: STABILITY", "Cell sustains with 2-3 neighbors"},
        {"PROTOCOL 3: OVERCROWDING", "Cell termination if neighbors > 3"},
        {"PROTOCOL 4: GENESIS", "Cell creation with exactly 3 neighbors"}
    };

    float startY = window.getSize().y * 0.35f;
    float spacing = 80.0f;

    for (size_t i = 0; i < rules.size(); i++) {
        // Rule panel
        sf::RectangleShape rulePanel;
        rulePanel.setSize(sf::Vector2f(window.getSize().x * 0.6f, 60.0f));
        rulePanel.setPosition(window.getSize().x * 0.2f, startY);
        
        // Animated highlight
        float alpha = 20 + static_cast<int>(10 * std::sin(time * 2.0f + i * 0.5f));
        rulePanel.setFillColor(sf::Color(MENU_SELECTED_COLOR.r, MENU_SELECTED_COLOR.g, MENU_SELECTED_COLOR.b, alpha));
        rulePanel.setOutlineColor(sf::Color(ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, 50));
        rulePanel.setOutlineThickness(1.0f);
        window.draw(rulePanel);

        // Rule title
        sf::Text titleText;
        titleText.setFont(font);
        titleText.setString(rules[i].title);
        titleText.setCharacterSize(24);
        titleText.setFillColor(MENU_SELECTED_COLOR);
        titleText.setPosition(window.getSize().x * 0.22f, startY + 5);
        window.draw(titleText);

        // Rule description
        sf::Text descText;
        descText.setFont(font);
        descText.setString(rules[i].description);
        descText.setCharacterSize(20);
        descText.setFillColor(MENU_NORMAL_COLOR);
        descText.setPosition(window.getSize().x * 0.22f, startY + 35);
        window.draw(descText);

        startY += spacing;
    }
}

void Game::drawTutorialPage4() {
    // Main panel
    sf::RectangleShape mainPanel;
    mainPanel.setSize(sf::Vector2f(window.getSize().x * 0.8f, window.getSize().y * 0.7f));
    mainPanel.setPosition(window.getSize().x * 0.1f, window.getSize().y * 0.15f);
    mainPanel.setFillColor(sf::Color(20, 20, 35, 230));
    mainPanel.setOutlineColor(ACCENT_COLOR);
    mainPanel.setOutlineThickness(2.0f);
    window.draw(mainPanel);

    // Title
    sf::Text title;
    title.setFont(font);
    title.setString("PATTERN DATABASE");
    title.setCharacterSize(36);
    title.setFillColor(MENU_SELECTED_COLOR);
    title.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.25f);
    title.setOrigin(title.getLocalBounds().width / 2.0f, 0);
    title.setOutlineColor(ACCENT_COLOR);
    title.setOutlineThickness(1.0f);
    window.draw(title);

    static float time = 0;
    time += 0.016f;

    struct Pattern {
        std::string key;
        std::string name;
        std::string description;
    };

    std::vector<Pattern> patterns = {
        {"[1]", "GLIDER", "Basic self-propelling unit"},
        {"[2]", "SMALL EXPLODER", "Chaotic expansion pattern"},
        {"[3]", "SPACESHIP", "Advanced movement algorithm"},
        {"[4]", "PULSAR", "Complex oscillation sequence"},
        {"[5]", "MANUAL MODE", "Direct cell manipulation"},
        {"[6]", "CUSTOM PATTERN", "Import external configuration"}
    };

    float startY = window.getSize().y * 0.35f;
    float spacing = 50.0f;

    for (size_t i = 0; i < patterns.size(); i++) {
        // Pattern panel
        sf::RectangleShape patternPanel;
        patternPanel.setSize(sf::Vector2f(window.getSize().x * 0.6f, 40.0f));
        patternPanel.setPosition(window.getSize().x * 0.2f, startY);
        
        float alpha = 20 + static_cast<int>(10 * std::sin(time * 2.0f + i * 0.5f));
        patternPanel.setFillColor(sf::Color(MENU_SELECTED_COLOR.r, MENU_SELECTED_COLOR.g, MENU_SELECTED_COLOR.b, alpha));
        patternPanel.setOutlineColor(sf::Color(ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, 30));
        patternPanel.setOutlineThickness(1.0f);
        window.draw(patternPanel);

        // Key binding
        sf::Text keyText;
        keyText.setFont(font);
        keyText.setString(patterns[i].key);
        keyText.setCharacterSize(22);
        keyText.setFillColor(ACCENT_COLOR);
        keyText.setPosition(window.getSize().x * 0.22f, startY + 10);
        window.draw(keyText);

        // Pattern name
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(patterns[i].name);
        nameText.setCharacterSize(22);
        nameText.setFillColor(MENU_SELECTED_COLOR);
        nameText.setPosition(window.getSize().x * 0.28f, startY + 10);
        window.draw(nameText);

        // Pattern description
        sf::Text descText;
        descText.setFont(font);
        descText.setString(patterns[i].description);
        descText.setCharacterSize(20);
        descText.setFillColor(MENU_NORMAL_COLOR);
        descText.setPosition(window.getSize().x * 0.45f, startY + 10);
        window.draw(descText);

        startY += spacing;
    }
}


void Game::drawTutorialPage5() {
    // Main panel with sci-fi border
    sf::RectangleShape mainPanel;
    mainPanel.setSize(sf::Vector2f(window.getSize().x * 0.8f, window.getSize().y * 0.7f));
    mainPanel.setPosition(window.getSize().x * 0.1f, window.getSize().y * 0.15f);
    mainPanel.setFillColor(sf::Color(20, 20, 35, 230));
    mainPanel.setOutlineColor(ACCENT_COLOR);
    mainPanel.setOutlineThickness(2.0f);
    window.draw(mainPanel);

    // Title
    sf::Text title;
    title.setFont(font);
    title.setString("ADVANCED OPERATIONS");
    title.setCharacterSize(36);
    title.setFillColor(MENU_SELECTED_COLOR);
    title.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.25f);
    title.setOrigin(title.getLocalBounds().width / 2.0f, 0);
    title.setOutlineColor(ACCENT_COLOR);
    title.setOutlineThickness(1.0f);
    window.draw(title);

    static float time = 0;
    time += 0.016f;

    struct AdvancedFeature {
        std::string key;
        std::string description;
    };

    std::vector<AdvancedFeature> features = {
        {"[S]", "Save current configuration to database"},
        {"[MENU]", "Access archived configurations"},
        {"[+/-]", "Modify simulation temporal coefficient"},
        {"[MODE 5]", "Enable direct pattern manipulation"},
        {"[SCROLL/DRAG]", "Navigate large-scale configurations"},
        {"[M/N/P]", "Audio subsystem control interface"},
        {"[PG UP/DOWN]", "Adjust audio amplitude levels"}
    };

    float startY = window.getSize().y * 0.35f;
    float spacing = 50.0f;

    for (size_t i = 0; i < features.size(); i++) {
        // Feature panel with animation
        sf::RectangleShape featurePanel;
        featurePanel.setSize(sf::Vector2f(window.getSize().x * 0.6f, 40.0f));
        featurePanel.setPosition(window.getSize().x * 0.2f, startY);
        
        // Animated highlight
        float alpha = 20 + static_cast<int>(10 * std::sin(time * 2.0f + i * 0.5f));
        featurePanel.setFillColor(sf::Color(MENU_SELECTED_COLOR.r, MENU_SELECTED_COLOR.g, MENU_SELECTED_COLOR.b, alpha));
        featurePanel.setOutlineColor(sf::Color(ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, 30));
        featurePanel.setOutlineThickness(1.0f);
        window.draw(featurePanel);

        // Command key
        sf::Text keyText;
        keyText.setFont(font);
        keyText.setString(features[i].key);
        keyText.setCharacterSize(22);
        keyText.setFillColor(ACCENT_COLOR);
        keyText.setPosition(window.getSize().x * 0.22f, startY + 10);
        window.draw(keyText);

        // Feature description
        sf::Text descText;
        descText.setFont(font);
        descText.setString(features[i].description);
        descText.setCharacterSize(20);
        descText.setFillColor(MENU_NORMAL_COLOR);
        descText.setPosition(window.getSize().x * 0.35f, startY + 10);
        window.draw(descText);

        startY += spacing;
    }

    // Navigation hint with pulsing effect
    sf::Text navText;
    navText.setFont(font);
    navText.setString("[ ESC ] RETURN TO COMMAND CENTER");
    navText.setCharacterSize(24);
    float alpha = 128 + static_cast<int>(127 * std::sin(time * 3.0f));
    navText.setFillColor(sf::Color(MENU_SELECTED_COLOR.r, MENU_SELECTED_COLOR.g, MENU_SELECTED_COLOR.b, alpha));
   navText.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.8f);
    navText.setOrigin(navText.getLocalBounds().width / 2.0f, 0);
    window.draw(navText);
}


void Game::drawTutorialProgress() {
    float dotSpacing = 20.0f;
    float totalWidth = (TOTAL_TUTORIAL_PAGES - 1) * dotSpacing;
    float startX = (window.getSize().x - totalWidth) / 2.0f;

    for (int i = 0; i < TOTAL_TUTORIAL_PAGES; i++) {
        sf::CircleShape dot(4.0f);
        dot.setOrigin(4.0f, 4.0f);
        dot.setPosition(startX + i * dotSpacing, window.getSize().y * 0.90f);

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
    
    escText.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.73f);  // Adjust this value to match your page indicators
    
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
    if (saveMessageClock.getElapsedTime() < SAVE_MESSAGE_DURATION) {
        musicInfo.setString(saveMessage);
        musicInfo.setPosition(10, window.getSize().y - 40);
        window.draw(musicInfo);
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
        saveMessage = "Music Paused";
        std::cout << "Music paused" << std::endl;
    }
    else {
        std::cout << "Attempting to play music..." << std::endl;
        // If music was never started or has stopped
        if (currentMusic.getStatus() == sf::Music::Status::Stopped) {
            std::cout << "Loading file: " << musicPlaylist[currentTrackIndex] << std::endl;
            if (!currentMusic.openFromFile(musicPlaylist[currentTrackIndex])) {
                std::cerr << "Failed to load music file!" << std::endl;
                return;
            }
            currentMusic.setVolume(musicVolume);
        }
        currentMusic.play();
        isMusicPlaying = true;
        saveMessage = "Music Playing";
        std::cout << "Music playing: " << musicPlaylist[currentTrackIndex] << std::endl;
    }
    saveMessageClock.restart();
    musicDisplayClock.restart();
}

void Game::playNextTrack() {
    if (musicPlaylist.empty()) {
        std::cout << "Playlist is empty!" << std::endl;
        return;
    }
    
    currentTrackIndex = (currentTrackIndex + 1) % musicPlaylist.size();
    std::cout << "Attempting to play track " << currentTrackIndex + 1 << ": " 
              << musicPlaylist[currentTrackIndex] << std::endl;
    
    if (currentMusic.openFromFile(musicPlaylist[currentTrackIndex])) {
        currentMusic.setVolume(musicVolume);
        currentMusic.play();
        isMusicPlaying = true;
        musicDisplayClock.restart();
        std::cout << "Successfully started playing track " << currentTrackIndex + 1 << std::endl;
    } else {
        std::cerr << "Failed to open music file: " << musicPlaylist[currentTrackIndex] << std::endl;
    }
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
        reset();
        grid = Grid(calculateOptimalGridSize().x, calculateOptimalGridSize().y);
        // Only start music if it's not already playing
        if (!isMusicPlaying && !musicPlaylist.empty()) {
            currentTrackIndex = std::rand() % musicPlaylist.size();
            if (currentMusic.openFromFile(musicPlaylist[currentTrackIndex])) {
                currentMusic.setVolume(musicVolume);
                currentMusic.play();
                isMusicPlaying = true;
                musicDisplayClock.restart();
            }
        }
        gameState = GameState::Playing;
        break;

    case 1: // Load Game
        loadGame();
        // Only start music if it's not already playing
        if (!isMusicPlaying && !musicPlaylist.empty()) {
            currentTrackIndex = std::rand() % musicPlaylist.size();
            if (currentMusic.openFromFile(musicPlaylist[currentTrackIndex])) {
                currentMusic.setVolume(musicVolume);
                currentMusic.play();
                isMusicPlaying = true;
                musicDisplayClock.restart();
            }
        }
        gameState = GameState::Playing;
        break;

    case 2: // Tutorial
        gameState = GameState::Tutorial;
        currentTutorialPage = 1;
        break;

    case 3: // Music Settings
        gameState = GameState::MusicSettings;
        selectedMusicTrack = currentTrackIndex;
        break;

    case 4: // Exit
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
    // Try multiple possible locations for the file
    std::vector<std::string> possiblePaths = {
        "custom_pattern.txt",                    // Current directory
        "../custom_pattern.txt",                 // Parent directory
        "./custom_pattern.txt",                  // Explicit current directory
        "../Copy/custom_pattern.txt"             // Your specific directory
    };

    std::ifstream file;
    std::string usedPath;

    // Try each path until we find the file
    for (const auto& path : possiblePaths) {
        file.open(path);
        if (file.is_open()) {
            usedPath = path;
            break;
        }
    }

    if (!file.is_open()) {
        saveMessage = "Failed to open custom pattern file! Tried: " + possiblePaths[0];
        saveMessageClock.restart();
        return;
    }

    // Debug output
    std::cout << "Successfully opened pattern file at: " << usedPath << std::endl;

    int height, width;
    if (!(file >> height >> width)) {
        saveMessage = "Invalid custom pattern format!";
        saveMessageClock.restart();
        file.close();
        return;
    }

    std::cout << "Pattern dimensions: " << width << "x" << height << std::endl;

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
            if (!(file >> cell)) {
                saveMessage = "Failed to read cell at position (" + std::to_string(i) + "," + std::to_string(j) + ")";
                saveMessageClock.restart();
                file.close();
                return;
            }
            if (cell != 0 && cell != 1) {
                saveMessage = "Invalid cell value at (" + std::to_string(i) + "," + std::to_string(j) + "): " + std::to_string(cell);
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
            }
            else if (patterns.find(selectedPattern) != patterns.end()) {
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

void Game::updateMusicVolume(float delta) {
    musicVolume = clamp(musicVolume + delta, 0.0f, 100.0f);
    currentMusic.setVolume(musicVolume);
    saveMessage = "Volume: " + std::to_string(static_cast<int>(musicVolume)) + "%";
    saveMessageClock.restart();
    musicDisplayClock.restart();
}

void Game::drawMusicSettings() {
    window.clear(BACKGROUND_COLOR);
    window.draw(backgroundSprite);

    // Draw title
    sf::Text title;
    title.setFont(font);
    title.setString("Music Settings");
    title.setCharacterSize(48);
    title.setFillColor(MENU_SELECTED_COLOR);
    title.setStyle(sf::Text::Bold);

    // Center the title
    float titleX = (window.getSize().x - title.getGlobalBounds().width) / 2;
    title.setPosition(titleX, 50);
    window.draw(title);

    // Draw volume control
    sf::Text volumeText;
    volumeText.setFont(font);
    volumeText.setString("Volume: " + std::to_string(static_cast<int>(musicVolume)) + "%");
    volumeText.setCharacterSize(24);
    volumeText.setFillColor(MENU_NORMAL_COLOR);
    volumeText.setPosition(window.getSize().x * 0.3f, 150);
    window.draw(volumeText);

    // Volume bar
    sf::RectangleShape volumeBar;
    volumeBar.setSize(sf::Vector2f(400, 20));
    volumeBar.setPosition(window.getSize().x * 0.3f, 190);
    volumeBar.setFillColor(sf::Color(MENU_NORMAL_COLOR.r, MENU_NORMAL_COLOR.g, MENU_NORMAL_COLOR.b, 50));
    window.draw(volumeBar);

    // Volume level
    sf::RectangleShape volumeLevel;
    volumeLevel.setSize(sf::Vector2f(4 * musicVolume, 20));
    volumeLevel.setPosition(window.getSize().x * 0.3f, 190);
    volumeLevel.setFillColor(MENU_SELECTED_COLOR);
    window.draw(volumeLevel);

    // Draw track list
    float startY = 250;
    float spacing = 40;

    sf::Text playlistTitle;
    playlistTitle.setFont(font);
    playlistTitle.setString("Available Tracks:");
    playlistTitle.setCharacterSize(24);
    playlistTitle.setFillColor(MENU_NORMAL_COLOR);
    playlistTitle.setPosition(window.getSize().x * 0.3f, startY);
    window.draw(playlistTitle);

    startY += spacing + 20;

    for (size_t i = 0; i < musicTrackNames.size(); i++) {
        sf::RectangleShape trackBg;
        trackBg.setSize(sf::Vector2f(400, 30));
        trackBg.setPosition(window.getSize().x * 0.3f, startY + i * spacing);

        // Highlight current track
        if (i == currentTrackIndex) {
            trackBg.setFillColor(sf::Color(MENU_SELECTED_COLOR.r,
                MENU_SELECTED_COLOR.g,
                MENU_SELECTED_COLOR.b, 40));
        }
        else {
            trackBg.setFillColor(sf::Color::Transparent);
        }
        window.draw(trackBg);

        // Track name
        sf::Text trackText;
        trackText.setFont(font);
        trackText.setString(musicTrackNames[i]);
        trackText.setCharacterSize(20);
        trackText.setFillColor(i == selectedMusicTrack ? MENU_SELECTED_COLOR : MENU_NORMAL_COLOR);
        trackText.setPosition(window.getSize().x * 0.3f + 20, startY + i * spacing + 5);
        window.draw(trackText);

        // Playing indicator
        if (i == currentTrackIndex && isMusicPlaying) {
            sf::CircleShape playingIndicator(5);
            playingIndicator.setFillColor(ACCENT_COLOR);
            playingIndicator.setPosition(window.getSize().x * 0.3f + 380, startY + i * spacing + 10);
            window.draw(playingIndicator);
        }
    }

    // Draw controls help
    std::vector<std::string> controls = {
        "Space - Play/Pause",
        "Up/Down - Select Track",
        "Enter - Play Selected",
        "Left/Right - Adjust Volume",
        "M - Mute/Unmute",
        "Esc - Back to Menu"
    };

    float controlsY = startY + musicTrackNames.size() * spacing + 40;
    for (const auto& control : controls) {
        sf::Text controlText;
        controlText.setFont(font);
        controlText.setString(control);
        controlText.setCharacterSize(20);
        controlText.setFillColor(SUBTITLE_COLOR);
        controlText.setPosition(window.getSize().x * 0.3f, controlsY);
        window.draw(controlText);
        controlsY += 30;
    }

    window.display();
}

void Game::handleMusicSettingsEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Escape:
            gameState = GameState::Menu;
            break;
        case sf::Keyboard::Up:
            if (selectedMusicTrack > 0) selectedMusicTrack--;
            break;
        case sf::Keyboard::Down:
            if (selectedMusicTrack + 1 < musicTrackNames.size()) selectedMusicTrack++;
            break;
        case sf::Keyboard::Left:
            updateMusicVolume(-5.0f);
            break;
        case sf::Keyboard::Right:
            updateMusicVolume(5.0f);
            break;
        case sf::Keyboard::Space:
            toggleMusic();
            break;
        case sf::Keyboard::Return:
            if (selectedMusicTrack != currentTrackIndex) {
                currentTrackIndex = selectedMusicTrack;
                if (currentMusic.openFromFile(musicPlaylist[currentTrackIndex])) {
                    currentMusic.setVolume(musicVolume);
                    currentMusic.play();
                    isMusicPlaying = true;
                }
            }
            break;
        default:
            break;
        }
    }
}
