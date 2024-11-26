#include<iostream>
#include"Grid.h"
#include<sstream>
#include<fstream> 
#include<cstdlib> 
#include"Cell.h" 
Grid::Grid(int w, int h) : width(w), height(h), cells(h, std::vector<std::shared_ptr<Cell>>(w)) {}

int Grid::countLiveNeighbors(int x, int y) const {

	int count = 0; 
	for (int dx = -1; dx <= 1;dx++) {
	
		for (int dy = -1; dy <= 1; ++dy) {
		
		if (dx==0 && dy == 0 ) continue; 
		
		int nx = x + dx;
		int ny = y + dy; 
		
			if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
			
				count += cells [ny][nx] -> Alive(); 

			}	
			
		}

	}
		return count; 
}

void Grid::loadFromFile(const string &filename) {

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


void Grid::update() {
    vector<vector<shared_ptr<Cell>>> newCells = cells;
    
    for (int y = 0; y < height; ++y) {
        
	    for (int x = 0; x < width; ++x) {
            
		    int liveNeighbors = countLiveNeighbors(x, y);
            	    newCells[y][x] = cells[y][x]->nextState(liveNeighbors);
        	}
    	}
    		cells = newCells;
}

bool Grid::hasChanged() const {
        for (int y = 0; y < height; ++y) {
        	for (int x = 0; x < width; ++x) {
            
			if (cells[y][x]->Alive() != cells[y][x]->nextState(countLiveNeighbors(x, y))->Alive()) {
			return true;
            	}
       	 }
    }

	return false;
}


void Grid::display() const {
 
	for (int y = 0; y < height; ++y) {

		for (int x = 0; x < width; ++x) {

		cout << (cells[y][x]->Alive() ? '1' : '0') << ' ';

	}
        	cout << '\n';
    }
}


