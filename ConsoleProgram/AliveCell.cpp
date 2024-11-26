#include"Cell.h"
#include<iostream> 


bool AliveCell::Alive() const {
    return true;
}

shared_ptr<Cell> AliveCell::nextState(int liveNeighbors) const {
    if (liveNeighbors == 2 || liveNeighbors == 3) {
        return make_shared<AliveCell>();
    }
    return make_shared<DeadCell>();
}

