#pragma once
#include <vector>
#include <list>

using namespace std;

class Graph {
	int V;
	list<int> *adj;
	void DFSRec(int, bool[]);
public:
	Graph(int);
	void addEdge(int, int);
	vector<vector<int>> DFS(int);
	vector<vector<int>> col;
};