#ifndef DEADCELL_H
#define DEADCELL_H 
#include"Cell.h" 

class DeadCell : public Cell {
	
	public:
    		bool Alive() const override;
    		shared_ptr<Cell> nextState(int liveNeighbors) const override;
};



#endif 
