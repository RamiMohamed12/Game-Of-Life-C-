#include<iostream>
#include <unistd.h>
#include "FileHandler.h"
#ifdef _WIN32
    #include<windows.h> 
#else 
    #include<cstdlib> 
    #include<unistd.h> 
#endif
#include<thread> 
#include<chrono> 
#include"Game.h" 
using namespace std;

Game::Game(const std::string& inputFile) : inputFile(inputFile), generation(1) {
    outputFolder = inputFile + "_out";
}

int Game::getSimulationSpeed() {

	int speed;
	cout << "Enter simulation speed(milliseconds between generations, 1-1000): ";
	cin >> speed;
	return max(1, min(speed,1000));
}



void Game::run() {
    setupOutputFolder();
    clearScreen();
    hideCursor();
    initializeGrid();
    int numGenerations = getNumberOfgenerations();
    if (numGenerations > 0) {
   	int simulationSpeed = getSimulationSpeed(); 
    	gameLoop(numGenerations,simulationSpeed); 
    
    }	else {
    
    	manualGameLoop(); 

    }
}

void Game::clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void Game::hideCursor() {
    cout << "\033[?25l";
}

void Game::showCursor() {
    cout << "\033[?25h";
}

void Game::setupOutputFolder() {
    if (!FileHandler::directoryExists(outputFolder)) {
        FileHandler::createDirectory(outputFolder);
    } else {
        try {
            FileHandler::clearDirectory(outputFolder);
        } catch (const std::exception& e) {
            std::cerr << "Error clearing directory: " << e.what() << '\n';
            exit(1);
        }
    }
}

void Game::initializeGrid() {
	try {
	FileHandler::readGridDimensions(inputFile,width,height);
	grid= make_unique<Grid>(width,height);
	grid->loadFromFile(inputFile); 
	} catch(const exception& e) {
	
		cerr<<"Error initializing grid:"<< e.what() << '\n'; 
		exit(1); 
	
	}


}


int Game::getNumberOfgenerations(){

	int numGenerations; 
	cout << "Enter number of generations to simulate (or 0 for manual mode): "; 
	cin >> numGenerations; 
	return numGenerations; 
}

void Game::gameLoop(int numGenerations,int simulationSpeed) {
    while (generation <= numGenerations) {
        hideCursor();
        cout << "\rGeneration " << generation << ":\n"; 
        grid->display(); 
        try {
            grid->saveToFile(outputFolder, generation, width, height); 
        } catch (const exception& e) {
            cerr << "Cannot open file for saving grid state: " << e.what() << '\n'; 
            exit(1); 
        }

        grid->update(); 
        generation++; 
        this_thread::sleep_for(chrono::milliseconds(simulationSpeed)); // Adjust delay as needed
    }
    showCursor(); // Ensure cursor is shown after loop ends
    cout << "Simulation complete.\n";
}

void Game::manualGameLoop() {
    while (true) {
        hideCursor();
        cout << "\rGeneration " << generation << ":\n";
        grid->display();
        try {
            grid->saveToFile(outputFolder, generation, width, height);
        } catch (const exception& e) {
            cerr << "Cannot open file for saving grid state: " << e.what() << '\n';
            exit(1);
        }

        grid->update();
        generation++;
        cout << "Press SPACEBAR to continue, ESC to stop" << '\n';

        while (true) {
            #ifdef _WIN32
                if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                    break;
                }
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    return;
                }
            #else
                system("stty raw");
                char input = getchar();
                system("stty cooked");

                if (input == ' ') {
                    break;
                }
                if (input == 27) {
                    return;
                }
            #endif

            this_thread::sleep_for(chrono::milliseconds(50));
        }
    }
}


