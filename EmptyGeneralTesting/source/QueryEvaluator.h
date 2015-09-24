#pragma once
#include "QueryTree.h"
#include "PKB.h"
#include <list>
#include <string>
#include <vector>
#include <stack>
#include "Synonym.h"
#include "Enum.h"

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
	bool evaluateAssign(Clauses clause);
	void storeResults(vector<int>& intermediateResult, string syn, TYPE type);
	string convertToShuntingYard(string statement);
	bool isOperator(char o);
	int isPriority(const char & c);
	PKB *pkb = PKB::getInstanceOf();

};