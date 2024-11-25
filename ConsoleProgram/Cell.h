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

class DeadCell : public Cell {
	
	public:
    		bool Alive() const override;
    		shared_ptr<Cell> nextState(int liveNeighbors) const override;
};

class AliveCell : public Cell {
	
	public:
    		bool Alive() const override;
    		shared_ptr<Cell> nextState(int liveNeighbors) const override;
};

#endif 

