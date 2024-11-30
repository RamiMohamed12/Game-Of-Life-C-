#ifndef GAME_H 
#define GAME_H 

#include<string> 
#include"Grid.h" 
#include<memory> 
using namespace std;

class Game {

        public : 
                Game(const string& inputFile); 
                void run(); 
        private : 
                string inputFile;
                string outputFolder; 
                int generation;
                int width,height; 
                unique_ptr<Grid> grid;
                void clearScreen();
                void  hideCursor();
                void showCursor();
                void setupOutputFolder();
                void initializeGrid();
                void gameLoop();


};

#endif
