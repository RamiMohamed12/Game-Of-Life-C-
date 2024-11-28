#include "FileHandler.h"
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "AliveCell.h"
#include "DeadCell.h"
#include "Cell.h"

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

void FileHandler::loadGridFromFile(const string &filename, vector<vector<shared_ptr<Cell>>> &cells, int width,int height) {

	ifstream inputFile(filename);
    	
	if (!inputFile) {
        throw runtime_error("Can't open input file!");
    	}

    	string line;
   	int y = 0;

    	while (getline(inputFile, line) && y < height) {
        
		stringstream ss(line);
        
		for (int x = 0; x < width && !ss.eof(); ++x) {
            
			int state;
			ss >> state;
       	    		cells[y][x] = state ? static_pointer_cast<Cell>(make_shared<AliveCell>()) : static_pointer_cast<Cell>(make_shared<DeadCell>());

       	}
        
		++y;
    }

}

void FileHandler::saveToFile(const string &outputFolder, int iteration, const vector<vector<shared_ptr<Cell>>> &cells,int width,int height){

	stringstream filename; 
	filename <<outputFolder << "/generation_" << iteration << ".txt";
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

