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
       		static const int STABILITY_THRESHOLD = 5; 
		int stableCount = 0; 
		bool isStable() const; 
       	private : 
                string inputFile;
                string outputFolder; 
                int generation;
                int width,height; 
                unique_ptr<Grid> grid;
                void clearScreen();
                void hideCursor();
                void showCursor();
                void setupOutputFolder();
                void initializeGrid();
                void gameLoop(int numGenerations,int simulationSpeed); 
		int getNumberOfgenerations(); 
		void manualGameLoop(); 
	        int  getSimulationSpeed();
};

#endif
