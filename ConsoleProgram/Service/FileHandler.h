#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <vector>
#include <memory>
#include <string>
#include "Cell.h"

using namespace std; 
class FileHandler {

	public : 
	static bool directoryExists(const string& dir);
	static void createDirectory(const string& dir); 
	static void saveToFile(const string &outputFolder,int iteration,const vector<vector<shared_ptr<Cell>>>& cells,int width,int height); 
	static void loadGridFromFile(const string& filename,vector<vector<shared_ptr<Cell>>>& cells,int width, int height);
	static void clearDirectory(const string& dir); 
};


#endif 
