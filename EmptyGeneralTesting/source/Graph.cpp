#include "Graph.h";
#include <iostream>;

Graph::Graph(int V)
{
	this->V = V;
	adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w); // Add w to v’s list.
}

void Graph::DFSRec(int v, bool updated[])
{
	//nodesVisited.push_back(v);
	// Recur for all the vertices adjacent to this vertex
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i) {
		if (!updated[*i]) {
			DFSRec(*i, updated);
		}
		//updated then add missing variables into current list
		vector<int> list = col.at(*i);
		vector<int> existingList = col.at(v);
		for (int j = 0; j < list.size(); j++) {
			int var = list.at(j);
			if (find(existingList.begin(), existingList.end(), var) == existingList.end()) {
				existingList.push_back(var);
			}
		}
		col.at(v) = existingList;
	}
	updated[v] = true;
	return;
}

void Graph::DFSPath(int v, bool visited[]) {
	visited[v] = true;
	path.push_back(v);

	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i) {
		if (!visited[*i]) {
			DFSPath(*i, visited);
		}
	}
}

// DFS traversal of the vertices reachable from v. It uses recursive DFSUtil()
vector<vector<int>> Graph::DFS(int v)
{
	// Mark all the vertices as not updated
	bool *updated = new bool[V];
	for (int i = 0; i < V; i++) {
		if (adj[i].size() == 0) {
			updated[i] == true;
		}
		else {
			updated[i] = false;
		}
	}

	DFSRec(v, updated);
	return col;
}

vector<int> Graph::DFSOriginal(int v) {
	bool *visited = new bool[V];
	for (int i = 0; i < V; i++) {
		visited[i] = false;
	}

	DFSPath(v, visited);

	if (path.size() == 0) {
		cout << "EMPTY\n";
	}
	else {
		for (int i = 0; i < path.size(); i++) {
			//cout << path.at(i) << " ";
		}
	}
	return path;
}