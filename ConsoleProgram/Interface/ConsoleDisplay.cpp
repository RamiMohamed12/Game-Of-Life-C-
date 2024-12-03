#include <iostream>
#include "Game.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    Game game(inputFile);
    game.run();

    return 0;
}
