#pragma once
#include <regex>
#include <string>
#include <iostream>
#include "QueryTree.h"
#include "PKB.h"

using namespace std;

class ParserTypeWithSyn
{
public:
	ParserTypeWithSyn(vector<string> selectSynonym, vector<vector<string>> suchThatSynonym, vector<vector<string>> withSynonym, vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym);
	~ParserTypeWithSyn();

	vector<vector<string>> selectSynAndType;
	vector<vector<string>> suchThatSynAndType;
	vector<vector<string>> patternSynAndType;
	vector<vector<string>> withSynAndType;
	vector<vector<string>> getSelectSynAndType();
	vector<vector<string>> getSuchThatSynAndType();
	vector<vector<string>> getPatternSynAndType();
	vector<vector<string>> getWithSynAndType();

	void parseSelectTypeWithSyn(vector<string> selectSynonym, vector<string> type, vector<string> synonym);
	void parseSuchThatTypeWithSyn(vector<vector<string>> suchThatSynonym, vector<string> type, vector<string> synonym);
	void parseWithTypeWithSyn(vector<vector<string>> withSynonym, vector<string> type, vector<string> synonym);
	void parsePatternTypeWithSyn(vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym);
	void checkCommonSynonym();
	vector<string> arrangeSyn(vector<string> syn);
};

