#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Pattern.h"
#include <string>
#include <map>
#include <fstream>

enum class GameState {
    Menu,
    Playing,
    Tutorial  // Add Tutorial state
};

class Game {
public:
    Game();
    void run();

private:
    static const int CELL_SIZE = 15;

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

    // Member variables in initialization order
    sf::RenderWindow window;
    int gridSize;  // Must be before grid
    Grid grid;
    bool isRunning;
    std::string selectedPattern;
    bool manualMode;
    sf::Clock clock;
    sf::Font font;
    std::map<std::string, std::vector<std::vector<bool>>> patterns;
    GameState gameState;
    std::vector<std::string> menuItems;
    int selectedMenuItem;
    std::string saveMessage;
    sf::Clock saveMessageClock;

    // Member functions
    void handleEvents();
    void update();
    void draw();
    void drawMenu();
    void drawTutorial();  // Add this declaration
    void reset();
    void loadPatterns();
    void handleMenuEvents(const sf::Event& event);
    void saveGame();
    void loadGame();
    void selectMenuItem();
};

#endif
