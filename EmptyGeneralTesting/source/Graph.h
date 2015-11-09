#pragma once
#include <vector>
#include <list>

using namespace std;

class Graph {
	int V;
	list<int> *adj;
	void DFSRec(int, bool[]);
	void DFSPath(int, bool[]);
	vector<int> path;
	vector<vector<int>> allSimplePaths;
	void storeAllPathsRec(int, int, bool[], int[], int &);
public:
	Graph(int);
	~Graph();
	void addEdge(int, int);
	vector<vector<int>> DFS(int);
	vector<vector<int>> col;
	vector<int> DFSOriginal(int);
	vector<vector<int>> storeAllPaths(int, int);
};