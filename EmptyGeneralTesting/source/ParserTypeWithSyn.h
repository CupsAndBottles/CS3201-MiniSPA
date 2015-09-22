#pragma once
#include <regex>
#include <string>
#include <iostream>
#include "QueryTree.h"

using namespace std;

class ParserTypeWithSyn
{
public:
	ParserTypeWithSyn(vector<string> selectSynonym, vector<vector<string>> suchThatSynonym,
		vector<vector<string>> withSynonym,
		vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym);
	~ParserTypeWithSyn();

	vector<vector<string>> selectSynAndType;
	vector<vector<string>> suchThatSynAndType;
	vector<vector<string>> patternSynAndType;
	vector<vector<string>> getSelectSynAndType();
	vector<vector<string>> getSuchThatSynAndType();
	vector<vector<string>> getPatternSynAndType();

	void parseSelectTypeWithSyn(vector<string> selectSynonym, vector<string> type, vector<string> synonym);
	void parseSuchThatTypeWithSyn(vector<vector<string>> suchThatSynonym, vector<string> type, vector<string> synonym);
	void parseWithTypeWithSyn(vector<vector<string>> withSynonym, vector<string> type, vector<string> synonym);
	void parsePatternTypeWithSyn(vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym);
	string checkExpression(string arg2);
	bool checkIfVariable(string syn);
	bool ifDigit(string syn);
	bool matchWithKeyword(string syn, vector<string> type, vector<string> synonym);
	void checkCommonSynonym();
};

