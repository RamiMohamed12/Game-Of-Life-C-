#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Pattern.h"
#include "Menu.h"
#include "GridSizeSelector.h"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>

enum class GameState {
    Menu,
    Playing,
    Tutorial,
    GridSelection
};

class Game {
public:
    Game();
    void run();

private:
    // Constants
    static const int CELL_SIZE = 15;
    static const int STATUS_FONT_SIZE = 20;
    static const int TOTAL_TUTORIAL_PAGES = 2;
    const float MIN_ZOOM = 0.1f;
    const float MAX_ZOOM = 5.0f;
    const float MIN_SPEED = 0.1f;
    const float MAX_SPEED = 10.0f;
    const sf::Time SAVE_MESSAGE_DURATION = sf::seconds(2);

    // Colors
    const sf::Color BACKGROUND_COLOR = sf::Color(18, 18, 24);
    const sf::Color MENU_SELECTED_COLOR = sf::Color(129, 140, 248);
    const sf::Color MENU_NORMAL_COLOR = sf::Color(203, 213, 225);

    // Core components
    sf::RenderWindow window;
    sf::Font font;
    Menu menu;
    GridSizeSelector gridSelector;
    
    // Game state
    GameState gameState;
    int gridSize;
    Grid grid;
    bool isRunning;
    std::string selectedPattern;
    bool manualMode;
    int currentTutorialPage;
    
    // View control
    float zoomLevel = 1.0f;
    sf::Vector2f viewOffset = sf::Vector2f(0.0f, 0.0f);
    float simulationSpeed = 1.0f;
    
    // Statistics
    int generation = 0;
    int livingCells = 0;
    float fps = 0.0f;
    
    // Timing
    sf::Clock clock;
    sf::Clock fpsClock;
    
    // Save system
    std::string saveMessage;
    sf::Clock saveMessageClock;
    
    // Pattern storage
    std::map<std::string, std::vector<std::vector<bool>>> patterns;

    // Member functions
    void handleEvents();
    void update();
    void draw();
    void drawStats();
    void drawTutorial();
    void drawTutorialPage1();
    void drawTutorialPage2();
    void drawTutorialArrows();
    void reset();
    void loadPatterns();
    void saveGame();
    void loadGame();
    void processMenuSelection();
    void handleZoom(float delta);
    void handlePan(const sf::Vector2f& delta);
    void updateStats();
};

#endif
