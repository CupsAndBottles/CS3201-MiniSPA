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
	vector<Synonym> getResults();
	string convertToShuntingYard(string statement);
	vector<vector<int>> rearrangeSynonym(vector<vector<int>> syn);
	vector<Synonym> mergeWithinGroup(vector<vector<int>> group);
	Synonym mergeSyn(Synonym syn1, Synonym syn2);
	vector<pair<int, int>> checkCommonSyn(vector<Enum::TYPE> type1, vector<Enum::TYPE> type2, vector<string> synName1, vector<string> synName2);
	vector<vector<int>> groupSynonym(vector<Synonym> result);
	vector<vector<int>> mergeSyn(vector<vector<int>> syn, int i, int j);
	bool hasCommonSyn(vector<int> syn1, vector<int> syn2);
	bool hasCommonSyn(Synonym synFirst, Synonym synSecond);

private:
	QueryTree tree;
	vector<Synonym> results;
	vector<Clauses> nonCommonSyn;
	bool evaluateSuchThat(Clauses clause);
	bool isGivenParam(Clauses clause, int paramPos);
	int checkValidityOfEntities(Clauses clause, int paramPos);
	int checkValidityOfStringEntities(Enum::TYPE type, string entityName);
	int checkValidityOfIntEntities(Enum::TYPE type, int entityInt);
	bool evaluatePattern(Clauses clause);
	bool evaluateWhile(Clauses clause);
	bool evaluateIf(Clauses clause);
	bool evaluateAssign(Clauses clause);
	bool isOperator(char o);
	int isPriority(const char & c);
	bool evaluateWith(Clauses clause);
	bool evaluateValidityOfIntLeftRef(Clauses clause);
	bool hasSameAttrNames(Clauses clause);
	bool hasSameAttrValues(Clauses clause);
	vector<string> getAllAttrNames(Enum::TYPE type);
	vector<int> getAllAttrValues(Enum::TYPE type);
	bool getCommonAttrNames(vector<string> leftResults, vector<string> rightResults, Clauses clause);
	bool getCommonAttrValues(vector<int> leftResults, vector<int> rightResults, Clauses clause);
	vector<int> convertNamesToIndexes(vector<string> stringResults, Enum::TYPE type);
	void storeResultsForSyn(Clauses clause, vector<pair<int, int>> results);
	void storeResults(vector<Enum::TYPE> type, vector<string> synString, vector<vector<int>> resultToStore);
	string convertToString(int index, Enum::TYPE type);
	list<string> evaluateSelect(vector<Synonym> groupedSyns, vector<Clauses> select);
	vector<pair<string, vector<int>>> getValuesOfNonCommonSyn(vector<Clauses> nonCommon);
	vector<int> getStringedAttrIndexes(Enum::TYPE type);
	vector<pair<Enum::TYPE, vector<int>>> QueryEvaluator::rearrangeSynOrder(vector<pair<string, vector<int>>> mergedSelectedSyns, vector<Clauses> select);
	list<string> QueryEvaluator::convertResultsToString(vector<pair<Enum::TYPE, vector<int>>> arrangedSyns);
	vector<pair<string, vector<int>>> QueryEvaluator::getValuesOfSelectedSyns(vector<Synonym> groupedSyns, vector<Clauses> select);
	vector<pair<string, vector<int>>> QueryEvaluator::mergeSelectedSyns(vector<pair<string, vector<int>>> mergedValues, pair<string, vector<int>> toBeMerged);
	void printResults();
	PKB *pkb;
};