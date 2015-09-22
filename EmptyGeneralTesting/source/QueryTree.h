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
	void startPlotting(vector<vector<string>> synAndTypeForResult, vector<vector<string>> synAndTypeForSuchThat, vector<vector<string>> synAndTypeForPattern);

//	vector<Clauses> getSuchThatTree();
	//vector<Clauses> getPatternTree();
	//vector<Clauses> getResultTree();
	bool getIsValid();

private:
//	vector<Clauses> suchThatTree;
	//vector<Clauses> patternTree;
	vector<Clauses> resultTree;
	bool isValid;

	void addResultTree(vector<string> syn, vector<string> type);
	void addSuchThatTree(vector<string> stringVal, vector<string> type, vector<string> intVal);
	void addPatternTree(vector<string> stringVal, vector<string> type, vector<string> intVal, vector<string> expression);
	bool isItExpression(string expression);
};



