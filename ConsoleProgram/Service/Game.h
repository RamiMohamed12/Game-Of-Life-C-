#ifndef GAME_H 
#define GAME_H 

#include<string> 
#include"Grid.h" 
#include<memory> 
using namespace std;

class Game {
public: 
    Game(const string& inputFile); 
    void run();
    // Add these methods to public section
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isCellAlive(int x, int y) const;
    void toggleCell(int x, int y);
    void update() { grid->update(); generation++; }
    static const int STABILITY_THRESHOLD = 5; 
    int stableCount = 0; 
    bool isStable() const; 
    void initializeGrid(); 
private: 
    string inputFile;
    string outputFolder; 
    int generation;
    int width, height; 
    unique_ptr<Grid> grid;
    void clearScreen();
    void hideCursor();
    void showCursor();
    void setupOutputFolder();
    void gameLoop(int numGenerations, int simulationSpeed); 
    int getNumberOfgenerations(); 
    void manualGameLoop(); 
    int getSimulationSpeed();
};

#endif
