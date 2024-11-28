#ifndef CELL_H
#define CELL_H

#include <memory>

using namespace std; 

class Cell {
	
	public:
    		virtual bool Alive() const = 0;
    		virtual shared_ptr<Cell> nextState(int liveNeighbors) const = 0;
    		virtual ~Cell() = default;

};

	

#endif 

