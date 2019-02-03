// Project 5

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"

using namespace std;

class maze
{
public:
	maze(ifstream &fin);
	void print(int, int, int, int);
	bool isLegal(int i, int j);

	void setMap(int i, int j, int n);
	int getMap(int i, int j) const;
	int mapToRow(int n);
	int mapToCol(int n);
	void mapMazeToGraph(graph &g);
	void printRecurPath(graph &g);
	void printShortestPath1(graph &g);
	void printShortestPath2(graph &g);

private:
	int rows; // number of rows in the maze
	int cols; // number of columns in the maze

	matrix<bool> value;
	matrix<int> map;      // Mapping from maze (i,j) values to node index values
};

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n. 
{
	map[i][j] = n;
}

int maze::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
	return map[i][j];
}

int maze::mapToRow(int n) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == n)
				return i;
		}
	}
	return -1;
}

int maze::mapToCol(int n) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == n)
				return j;
		}
	}
	return -1;
}

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
	fin >> rows;
	fin >> cols;

	char x;

	value.resize(rows, cols);
	for (int i = 0; i <= rows - 1; i++)
		for (int j = 0; j <= cols - 1; j++)
		{
			fin >> x;
			if (x == 'O')
				value[i][j] = true;
			else
				value[i][j] = false;
		}

	map.resize(rows, cols);
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
	cout << endl;

	if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols) {
		cout << "goalI: " << goalI << " goalJ: " << goalJ << " rows: " << rows << " cols: " << cols << endl;
		throw rangeError("Bad value in maze::print");
	}

	if (currI < 0 || currI > rows || currJ < 0 || currJ > cols) {
		cout << "currI: " << currI << " currJ: " << currJ << " rows: " << rows << " cols: " << cols << endl;
		throw rangeError("Bad value in maze::print");
	}

	for (int i = 0; i <= rows - 1; i++)
	{
		for (int j = 0; j <= cols - 1; j++)
		{
			if (i == goalI && j == goalJ)
				cout << "*";
			else
				if (i == currI && j == currJ)
					cout << "+";
				else
					if (value[i][j])
						cout << " ";
					else
						cout << "X";
		}
		cout << endl;
	}
	cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
{
	if (i < 0 || i > rows || j < 0 || j > cols)
		throw rangeError("Bad value in maze::isLegal");

	return value[i][j];
}

void maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (value[i][j]) {
				g.addNode();
				setMap(i, j, g.numNodes()-1);
			}
		}
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			if (value[i][j]) {
				if (i+1 < rows) {
					if (value[i+1][j]) {
						g.addEdge(getMap(i, j), getMap(i+1, j));
					}
				}
				if (i-1 >= 0) {
					if (value[i-1][j]) {
						g.addEdge(getMap(i, j), getMap(i-1, j));
					}
				}
				if (j+1 < cols) {
					if (value[i][j+1]) {
						g.addEdge(getMap(i, j), getMap(i, j+1));
					}
				}
				if (j-1 >= 0) {
					if (value[i][j-1]) {
						g.addEdge(getMap(i, j), getMap(i, j-1));
					}
				}
			}
		}
	}
}

void maze::printRecurPath(graph &g) {
	int currentRow, currentCol, previousRow, previousCol;
	

	for (int i = 0; i < g.RecPathSize(); i++) {
		currentRow = mapToRow(g.RecPath(i));
		currentCol = mapToCol(g.RecPath(i));
		if (i == 0) {
			previousRow = 0;
			previousCol = 0;
		}
		else {
			previousRow = mapToRow(g.RecPath(i - 1));
			previousCol = mapToCol(g.RecPath(i - 1));
		}
		if (currentRow == (previousRow + 1)) {
			cout << endl << "Go down!" << endl;
		}
		else if (currentRow == (previousRow - 1)) {
			cout << endl << "Go up!" << endl;
		}
		else if (currentCol == (previousCol + 1)) {
			cout << endl << "Go right!" << endl;
		}
		else if (currentCol == (previousCol - 1)) {
			cout << endl << "Go left!" << endl;
		}
		else {
			cout << endl << endl;
		}
		print(currentRow, currentCol, rows - 1, cols - 1);
	}
	cout << "Found Recursive Path!!!" << endl;
}

void maze::printShortestPath1(graph &g) {
	stack<int> path;
	int temp, k, currentRow, currentCol, previousRow, previousCol;

	k = g.numNodes() - 1;
	path.push(k);
	while (k != 0) {
		temp = g.Predecessor(k);
		path.push(temp);
		k = g.Predecessor(k);
	}

	while (!path.empty()) {
		if (path.top() == 0) {
			previousRow = 0;
			previousCol = 0;
		}
		else {
			previousRow = currentRow;
			previousCol = currentCol;
		}
		currentRow = mapToRow(path.top());
		currentCol = mapToCol(path.top());
		path.pop();

		if (currentRow == (previousRow + 1)) {
			cout << endl << "Go down!" << endl;
		}
		else if (currentRow == (previousRow - 1)) {
			cout << endl << "Go up!" << endl;
		}
		else if (currentCol == (previousCol + 1)) {
			cout << endl << "Go right!" << endl;
		}
		else if (currentCol == (previousCol - 1)) {
			cout << endl << "Go left!" << endl;
		}
		else {
			cout << endl << endl;
		}
		print(currentRow, currentCol, rows - 1, cols - 1);
	}
	cout << "Found Path Through BFS!!!" << endl;

}

void maze::printShortestPath2(graph &g) {
	stack<int> path;
	int temp, k, currentRow, currentCol, previousRow, previousCol;

	k = g.numNodes() - 1;

	path.push(k);
	while (k != 0) {
		temp = g.Predecessor2(k);
		path.push(temp);
		k = temp;
	}

	while (!path.empty()) {
		if (path.top() == 0) {
			previousRow = 0;
			previousCol = 0;
		}
		else {
			previousRow = currentRow;
			previousCol = currentCol;
		}
		currentRow = mapToRow(path.top());
		currentCol = mapToCol(path.top());
		path.pop();

		if (currentRow == (previousRow + 1)) {
			cout << endl << "Go down!" << endl;
		}
		else if (currentRow == (previousRow - 1)) {
			cout << endl << "Go up!" << endl;
		}
		else if (currentCol == (previousCol + 1)) {
			cout << endl << "Go right!" << endl;
		}
		else if (currentCol == (previousCol - 1)) {
			cout << endl << "Go left!" << endl;
		}
		else {
			cout << endl << endl;
		}
		print(currentRow, currentCol, rows - 1, cols - 1);
	}
	cout << "Found Path Through Dijkstra!!!" << endl;

}




int main(int argc, char* argv[])
{
	ifstream fin;

	// Read the maze from the input file.
	string fileName = argv[1];
	cout << argv[1] << endl;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		cin.get();
		exit(1);
	}

	try
	{

		graph g;
		while (fin && fin.peek() != 'Z')
		{
			maze m(fin);
			m.mapMazeToGraph(g);
			if (g.findShortestPath1()) {
				m.printShortestPath1(g);
			}
			else
				cout << "Path Not Found through BFS." << endl << endl;

			if (g.findShortestPath2()) {
				m.printShortestPath2(g);
			}
			else
				cout << "Path Not Found Through Dijkstra" << endl << endl;


			cin.get();

		}


	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; 
		cin.get();
		exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; 
		cin.get();
		exit(1);
	}
}
