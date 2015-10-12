#pragma once
#include "QueryTree.h"
#include "PKB.h"
#include <list>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include "Synonym.h"
#include "Enum.h"

class QueryEvaluator
{
public:
	QueryEvaluator();
	QueryEvaluator(PKB &pkb);
	~QueryEvaluator();
	list<string> evaluateQuery(QueryTree tree);
	list<string> permutateResult(vector<vector<string>> intermediateResult);
	vector<Synonym> getResults();
	string convertToShuntingYard(string statement);

private:
	QueryTree tree;
	vector<Synonym> results;
	bool evaluateWith(Clauses clause);
	bool evaluateValidStmtRefs(Enum::TYPE type, int index);
	bool evaluateNonGivenAttr(Clauses clause);
	bool hasSameAttrNames(Clauses clause);
	bool hasSameAttrValues(Clauses clause);
	vector<string> getAllAttrNames(Enum::TYPE type);
	vector<int> getAllAttrValues(Enum::TYPE type);
	bool getCommonAttrNames(vector<string> leftResults, vector<string> rightResults, Clauses clause);
	bool getCommonAttrValues(vector<int> leftResults, vector<int> rightResults, Clauses clause);
	vector<int> convertNamesToIndexes(vector<string> stringResults, Enum::TYPE type);
	bool evaluateSuchThat(Clauses clause);
	bool evaluatePattern(Clauses clause);
	bool evaluateWhile(Clauses clause);
	bool evaluateIf(Clauses clause);
	bool evaluateAssign(Clauses clause);
	void storeResultsForSyn(Clauses clause, vector<pair<int, int>> results);
	void storeResults(vector<int> intermediateResult, string syn, Enum::TYPE type);
	bool isOperator(char o);
	int isPriority(const char & c);
	vector<string> evaluateSelect(Clauses select);
	vector<string> permutateResultPair(vector<string> firstSet, vector<string> secondSet);
	list<string> permutateResultSubset(vector<vector<string>> intermediateResult);
	list<string> convertVectorToList(vector<string> mergedResults);
	string convertToString(int index, Enum::TYPE type);
	PKB *pkb;
};