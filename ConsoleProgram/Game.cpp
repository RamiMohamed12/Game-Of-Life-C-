#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <memory>
#include "Grid.h"
#include "FileHandler.h"



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFolder = inputFile + "_out";
    
    // Check if output folder exists, and create it if necessary
    if (!FileHandler::directoryExists(outputFolder)) {
    FileHandler::createDirectory(outputFolder); 
    }


    int width, height;
    std::cout << "Enter grid width: ";
    std::cin >> width;
    std::cout << "Enter grid height: ";
    std::cin >> height;

    // Initialize Grid object
    Grid grid(width, height);

    try {
        // Load grid state from input file
        grid.loadFromFile(inputFile);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    int generation = 1;

    // Main loop for the simulation
    while (true) {
        std::cout << "Generation " << generation << ":\n";
        grid.display(); // Display the grid's current state
        
        // Save the grid's current state to a file
        try {
            grid.saveToFile(outputFolder, generation,width,height);
        } catch (const std::exception& e) {
            std::cerr << "Cannot open file for saving grid state: " << e.what() << '\n';
        }

        // Update grid state based on the rules (e.g., Conway's Game of Life)
        grid.update();

        generation++;  // Increment generation
        
        // Optional: Add a pause for visualization or any other condition to break the loop
        char continueSimulation;
        std::cout << "Do you want to continue to the next generation? (y/n): ";
        std::cin >> continueSimulation;
        if (continueSimulation != 'y') {
            break;
        }
    }

    return 0;
}

