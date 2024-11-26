#ifndef GRID_H 
#define GRID_H 

#include"Cell.h" 
#include<iostream>
#include<vector> 
#include<memory> 
#include<string> 

using namespace std; 

class Grid {

	private : 
	int width, height; 
	vector<vector<shared_ptr<Cell>>> cells; 
	int countLiveNeighbors( int x , int y ) const ; 
	
	public : 
	Grid(int x, int y); 
	void randomize();
	void update (); 
	void display() const; 






};










#endif

