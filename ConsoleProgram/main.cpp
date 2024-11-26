#include "Grid.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFolder = inputFile + "_out";
    int maxIterations = 100;

    int width, height;
    std::cout << "Enter grid width: ";
    std::cin >> width;
    std::cout << "Enter grid height: ";
    std::cin >> height;

    Grid grid(width, height);

    try {
        grid.loadFromFile(inputFile);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    for (int i = 0; i < maxIterations; ++i) {
        std::cout << "Generation " << i + 1 << ":\n";
        grid.display();
        try {
            grid.saveToFile(outputFolder, i);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            return 1;
        }
        if (!grid.hasChanged()) {
            std::cout << "No more changes, stopping simulation.\n";
            break;
        }
        grid.update();
        std::cout << "\n";
    }

    return 0;
}

