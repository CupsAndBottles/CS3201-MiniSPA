#pragma once
#include <string>
#include <vector>
#include "Clauses.h"
using namespace std;

class QueryTree
{
public:
	QueryTree();
	~QueryTree();
	QueryTree & return_created_Object();
	void startPlotting(vector<vector<string>> resultData, vector<vector<string>> suchThatData, vector<vector<string>> patternData, vector<vector<string>> withData);

	vector<Clauses> getSuchThatTree();
	vector<Clauses> getPatternTree();
	vector<Clauses> getResultTree();
	vector<Clauses> getWithTree();
	bool getIsValid();

private:
	vector<Clauses> suchThatTree;
	vector<Clauses> patternTree;
	vector <Clauses> resultTree;
	vector <Clauses> withTree;
	bool isValid;

	void addResultTree(vector<string> syn, vector<string> type);
	void addWithTree(vector<string> stringVal, vector<string> type, vector<string> intVal, vector<string> isStmt);
	void addSuchThatTree(vector<string> stringVal, vector<string> type, vector<string> intVal);
	void addPatternTree(vector<string> stringVal, vector<string> type, vector<string> intVal, vector<string> expression);
	bool isItExpression(string expression);
};



