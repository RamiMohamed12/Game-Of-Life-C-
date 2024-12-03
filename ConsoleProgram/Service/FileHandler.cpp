#include "FileHandler.h"
#include <exception>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "AliveCell.h"
#include "DeadCell.h"
#include "Cell.h"
#include<iomanip>
#include<experimental/filesystem> 
namespace fs = std::experimental::filesystem; 

using namespace std; 

#ifdef _WIN32
#include<direct.h> 
#else 
#include<unistd.h>
#endif


bool FileHandler::directoryExists(const string &dir) {

	struct stat info;
	return stat(dir.c_str(),&info) == 0 && (info.st_mode & S_IFDIR);


}

void FileHandler::createDirectory(const string& dir) {

	#ifdef _WIN32 
	
	if(mkdir(dir.c_str()) != 0) {
	
	#else
	
	if( mkdir(dir.c_str(),0777) !=0) {
					
			
	#endif	

		cerr <<"Error creating directory"<<dir << '\n'; 	

	       }
}

void FileHandler::clearDirectory(const string &dir) {

	try {
		if(!fs::exists(dir)){
		
			cerr<<"Directory does not exist: " << dir << '\n';
			return; 

		}	

		for (const auto &entry : fs::directory_iterator(dir)) {
		
		fs::remove(entry);
		
		}

		cout <<"Directory cleared : " << dir << '\n';
	} catch (const std::exception &e) {
	
		cerr<<"Error clearing directory " << dir << ": " << e.what() << '\n';

	}	
}


void FileHandler::readGridDimensions(const string &filename, int& width, int& height) {

	  ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("Can't open input file!");
    }
    
    inputFile >> height >> width;
    if (inputFile.fail()) {
        throw runtime_error("Error reading grid dimensions from file!");
    }
}



void FileHandler::loadGridFromFile(const string &filename, vector<vector<shared_ptr<Cell>>> &cells, int width, int height) {
    ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("Can't open input file!");
    }

    // Skip the dimensions line
    string dimensionsLine;
    getline(inputFile, dimensionsLine);

    // Read the grid state
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int state;
            if (!(inputFile >> state)) {
                throw runtime_error("Error reading grid state from file! File may be corrupted or incomplete.");
            }
            if (state != 0 && state != 1) {
                throw runtime_error("Invalid cell state in file! Only 0 or 1 are allowed.");
            }
            // Fix: Cast to shared_ptr<Cell> for consistent types
            cells[y][x] = state ? static_pointer_cast<Cell>(make_shared<AliveCell>()) 
                               : static_pointer_cast<Cell>(make_shared<DeadCell>());
        }
        
        // Skip any remaining characters until end of line
        string dummy;
        getline(inputFile, dummy);
    }

    // Fix: Corrected variable name extraLine
    string extraLine;
    if (getline(inputFile, extraLine) && !extraLine.empty()) {
        throw runtime_error("Extra data found in file after grid!");
    }
}			



void FileHandler::saveToFile(const string &outputFolder, int iteration, const vector<vector<shared_ptr<Cell>>> &cells,int width,int height){

	stringstream filename; 
	filename <<outputFolder << "/generation_" << setw(3) <<setfill('0') << iteration << ".txt";
	ofstream outFile(filename.str()); 
	
		if (!outFile) {
	
			throw runtime_error ("Cannot open file for saving grid state");

		}
	
		for( int y =0; y < height; y++) {
		
			for (int x=0; x< width; x++) {
			
				outFile<<(cells[y][x]->Alive() ? '1' : '0') << " ";  
			
			}
			outFile <<'\n'; 
		}
}

