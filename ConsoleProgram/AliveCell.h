#ifndef ALIVECELL_H 
#define ALIVECELL_H 
#include"Cell.h" 

class AliveCell : public Cell {
	
	public:
    		bool Alive() const override;
    		shared_ptr<Cell> nextState(int liveNeighbors) const override;
};

#endif 
