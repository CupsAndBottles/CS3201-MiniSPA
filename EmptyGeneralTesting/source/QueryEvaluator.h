#pragma once
#include <list>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include "Enum.h"
#include "QueryTree.h"
#include "PKB.h"
#include "Synonym.h"

class QueryEvaluator
{
public:
	QueryEvaluator();
	QueryEvaluator(PKB &pkb);
	~QueryEvaluator();

	vector<pair<int, int>> checkCommonSyn(vector<Enum::TYPE> type1, vector<Enum::TYPE> type2, vector<string> synName1, vector<string> synName2);
	list<string> evaluateQuery(QueryTree tree);
	vector<Synonym> getResults();
	vector<vector<int>> groupSynonym(vector<Synonym> result);
	vector<vector<int>> mergeSyn(vector<vector<int>> syn, int i, int j);
	Synonym mergeSyn(Synonym syn1, Synonym syn2);
	vector<Synonym> mergeWithinGroup(vector<vector<int>> group);
	vector<vector<int>> rearrangeSynonym(vector<vector<int>> syn);

private:
	QueryTree tree;
	vector<Synonym> results;
	vector<Clauses> nonCommonSyn;
	PKB *pkb;

	int checkValidityOfEntities(Clauses clause, int paramPos);
	int checkValidityOfStringEntities(Enum::TYPE type, string entityName);
	int checkValidityOfIntEntities(Enum::TYPE type, int entityInt);
	vector<int> convertNamesToIndexes(vector<string> stringResults, Enum::TYPE type);
	list<string> convertResultsToString(vector<pair<Enum::TYPE, vector<int>>> arrangedSyns);
	string convertToShuntingYard(string statement);
	string convertToString(int index, Enum::TYPE type);
	bool evaluateAssign(Clauses clause);
	bool evaluateIf(Clauses clause);
	bool evaluatePattern(Clauses clause);
	list<string> evaluateSelect(vector<Synonym> groupedSyns, vector<Clauses> select);
	bool evaluateSuchThat(Clauses clause);
	bool evaluateWith(Clauses clause);
	bool evaluateWhile(Clauses clause);
	bool evaluateValidityOfIntLeftRef(Clauses clause);
	vector<pair<Enum::TYPE, vector<int>>> extractTypeAndIndexes(Synonym merged);
	vector<string> getAllAttrNames(Enum::TYPE type);
	vector<int> getAllAttrValues(Enum::TYPE type);
	vector<int> getStringedAttrIndexes(Enum::TYPE type);
	vector<int> getValuesOfNonCommonSyn(Clauses nonCommon);
	vector<Synonym> getValuesOfSelectedSyns(vector<Synonym> groupedSyns, vector<Clauses> select);
	bool getCommonAttrNames(vector<string> leftResults, vector<string> rightResults, Clauses clause);
	bool getCommonAttrValues(Clauses clause);
	bool hasCommonSyn(vector<int> syn1, vector<int> syn2);
	bool hasCommonSyn(Synonym synFirst, Synonym synSecond);
	bool hasSameAttrNames(Clauses clause);
	bool isGivenParam(Clauses clause, int paramPos);
	bool isOperator(char o);
	int isPriority(const char & c);
	vector<pair<Enum::TYPE, vector<int>>> mergeBetweenGroups(vector<Synonym> mergedSyns);
	Synonym mergeTwoGroups(Synonym firstSet, Synonym secondSet);
	Synonym rearrangeSynOrder(Synonym mergedSelectedSyns, vector<Clauses> select);
	void storeResultsForSyn(Clauses clause, vector<pair<int, int>> results);
	void storeResults(vector<Enum::TYPE> type, vector<string> synString, vector<vector<int>> resultToStore);
	void printResults();

};