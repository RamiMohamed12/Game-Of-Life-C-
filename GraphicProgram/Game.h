#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Pattern.h"

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update();
    void draw();
    void reset();

    sf::RenderWindow window;
    Grid grid;
    bool isRunning;
    std::string selectedPattern;
    bool manualMode;
    sf::Clock clock;
    std::map<std::string, std::vector<std::vector<bool>>> patterns;
};

#endif
