#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <memory>
#include "Grid.h"
#include "FileHandler.h"
#ifdef _WIN32
	#include<windows.h> 
#else 
	#include<cstdlib> 
	#include<unistd.h> 
#endif
#include<thread> 
#include<chrono> 
using namespace std;


void clearScreen() {

	cout << "\033[2J\033[1;1H";
}

void hideCursor() {

    	cout << "\033[?25l";
}

void showCursor() {
    
	cout << "\033[?25h";

}


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
    } else {
        // Clear the output folder before starting new simulation runs
        try {
            FileHandler::clearDirectory(outputFolder);
        } catch (const std::exception& e) {
            std::cerr << "Error clearing directory: " << e.what() << '\n';
            return 1;
        }
    }
    clearScreen();
    hideCursor();
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
        std::cerr << "Error loading grid from file: " << e.what() << '\n';
        return 1;
    }

    int generation = 1;

	 while(true) {		
		cout << "\rGeneration " << generation << ":\n"; 
		grid.display(); 
		try {
		
		grid.saveToFile(outputFolder, generation, width, height); 

		} catch (const exception& e) {
		
		cerr << "Cannot open file for saving grid state: " << e.what() << '\n'; 
		return 1; 
		}		
		
		grid.update(); 
		generation++; 
		cout << "Press SPACEBAR to continue, ESC to stop" << '\n'; 

		while(true) {
		
		// This is for windows !
		#ifdef _WIN32
			if(GetAsyncKeyState(VK_SPACE) & 0x8000) {
			
			break;
			
			} if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			
			return 0; 
			
			}
		
		}
		// This is for Unix like systems ! 
		#else 
			system("stty raw");
			char input = getchar(); 
			system("stty cooked");
		
		if(input == ' ' ) {
		
			break; 
		
		} if (input == 27) {
		
		return 0; 

		}
		#endif 
		
		this_thread::sleep_for(chrono::milliseconds(50)); 	
	  }
     }
}
