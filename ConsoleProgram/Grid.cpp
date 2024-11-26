#include<iostream>
#include"Grid.h"
#include<sstream>
#include<fstream> 
#include<cstdlib> 

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


