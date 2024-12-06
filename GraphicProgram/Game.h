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
    GridSelection,
    Playing,
    Tutorial
};

class Game {
public:
    Game();
    void run();

private:
    static const int CELL_SIZE = 15;
    static const int TOTAL_TUTORIAL_PAGES = 2;

    // Colors
    const sf::Color BACKGROUND_COLOR = sf::Color(18, 18, 24);
    const sf::Color MENU_SELECTED_COLOR = sf::Color(129, 140, 248);  // Indigo
    const sf::Color MENU_NORMAL_COLOR = sf::Color(203, 213, 225);    // Slate
    const sf::Color GRID_LINE_COLOR = sf::Color(30, 30, 40);
    const sf::Color CELL_ALIVE_COLOR = sf::Color(129, 140, 248);     // Indigo
    const sf::Color CELL_DEAD_COLOR = sf::Color(30, 30, 40);

    // Menu styling
    const float MENU_ITEM_HEIGHT = 60.0f;
    const int MENU_FONT_SIZE = 32;
    const int STATUS_FONT_SIZE = 20;
    const sf::Time SAVE_MESSAGE_DURATION = sf::seconds(2);

    // Simulation control
    float simulationSpeed = 1.0f;
    const float MIN_SPEED = 0.25f;
    const float MAX_SPEED = 4.0f;
    float zoomLevel = 1.0f;
    const float MIN_ZOOM = 0.25f;
    const float MAX_ZOOM = 4.0f;
    sf::Vector2f viewOffset{ 0.0f, 0.0f };
    int generation = 0;
    sf::Clock fpsClock;
    float fps = 0.0f;
    int livingCells = 0;
    int currentTutorialPage = 1;

    sf::RenderWindow window;
    sf::Font font;
    int gridSize;
    Grid grid;
    bool isRunning;
    std::string selectedPattern;
    bool manualMode;
    sf::Clock clock;
    std::map<std::string, std::vector<std::vector<bool>>> patterns;
    GameState gameState;
    Menu menu;
    GridSizeSelector gridSelector;
    std::string saveMessage;
    sf::Clock saveMessageClock;

    void handleEvents();
    void update();
    void draw();
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
    void drawStats();
};

#endif
