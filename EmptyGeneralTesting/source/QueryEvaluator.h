#pragma once
#include "QueryTree.h"
#include <list>
#include <string>
#include <vector>
#include "Synonym.h"

enum class TYPE { ASSIGN, STATEMENT, PROCEDURE, UNDERSCORE, WHILE, IF, VARIABLE, CONSTANT, CALLS };

class QueryEvaluator
{
public:
	QueryEvaluator();
	~QueryEvaluator();

	list<string> evaluateQuery(QueryTree tree);

	vector<Synonym> getResults();

private:
	QueryTree tree;
	vector<Synonym> results;
	bool evaluateSuchThat(Clauses clause);
	bool evaluatePattern(Clauses clause);


};