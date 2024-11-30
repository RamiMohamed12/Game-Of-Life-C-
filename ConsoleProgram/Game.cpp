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
    // Grid object will be initialized later in initializeGrid()
}

void Game::run() {
    setupOutputFolder();
    clearScreen();
    hideCursor();
    initializeGrid();
    gameLoop();
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
    std::cout << "Enter grid width: ";
    std::cin >> width;
    std::cout << "Enter grid height: ";
    std::cin >> height;
    grid = std::make_unique<Grid>(width, height);  // Initialize Grid object here

    try {
        grid->loadFromFile(inputFile);
    } catch (const std::exception& e) {
        std::cerr << "Error loading grid from file: " << e.what() << '\n';
        exit(1);
    }
}

void Game::gameLoop() {
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
