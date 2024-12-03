#include<iostream>
#include"Grid.h"
#include<sstream>
#include<fstream> 
#include<cstdlib> 
#include"Cell.h"
#include"DeadCell.h"
#include"AliveCell.h"
#include"FileHandler.h" 

Grid::Grid(int w, int h) : width(w), height(h), cells(h, std::vector<std::shared_ptr<Cell>>(w)) {}


int Grid::toroidalIndex(int index,int maxSize) const {

	if(index < 0) {
	
	return maxSize - 1; 

	} else if (index >=maxSize) {
	
	return 0; 
	
	}	
	
	return index;
}

int Grid::countLiveNeighbors(int x, int y) const {

	int count = 0; 
	for (int dx = -1; dx <= 1;dx++) {
	
		for (int dy = -1; dy <= 1; ++dy) {
		
		if (dx==0 && dy == 0 ) continue; 
		
		int nx = toroidalIndex(x + dx, width);
		int ny = toroidalIndex(y + dy, height); 
		
			if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
			
				count += cells [ny][nx] -> Alive(); 

			}	
			
		}

	}
		return count; 
}


void Grid::loadFromFile(const string &filename) {
    FileHandler::loadGridFromFile(filename, cells, width, height);
}

void Grid::saveToFile(const string &outputFolder, int iteration,int width,int height) const {
    FileHandler::saveToFile(outputFolder, iteration, cells,width,height);
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
   	std::cout << std::flush;
      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cout << (cells[y][x]->Alive() ? '1' : '0') << ' ';
        }
        cout << '\n';
    }
}
