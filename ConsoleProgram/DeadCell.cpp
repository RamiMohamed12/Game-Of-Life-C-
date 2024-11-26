#include<iostream> 
#include "Cell.h" 
#include <memory> 
using namespace std; 


bool DeadCell::Alive() const {

return false; 

}

shared_ptr<Cell> DeadCell :: nextState(int liveNeighbors) const {

	if(liveNeighbors == 3) {
	
		return make_shared<AliveCell> ();
	
	}
	
		return make_shared<DeadCell> (); 
}

