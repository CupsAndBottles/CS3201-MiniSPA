#pragma once
#include <vector>
#include <string>
#include <regex>
#include "QueryTree.h"
#include "PKB.h"

using namespace std;

class ParserForPQL
{
public:
	vector<int> numberOfPos;
	vector<vector<string> > selectSynAndType;
	vector<vector<string> > suchThatSynAndType;
	vector<vector<string> > withSynAndType;
	vector<vector<string> > patternSynAndType;

	vector<vector<string> > type;
	vector<vector<string> > synonym;
	vector<vector<int> > position;
	vector<string> keywordSynonym;
	vector<regex> regexForBoth = { std::regex("\\band pattern\\b|\\band with\\b|\\band such that\\b|\\bSelect\\b|\\bsuch that\\b|\\bwith\\b|\\bpattern\\b|\\band\\b"),
		std::regex("\\bassign\\b|\\bstmt\\b|\\bprog_line\\b|\\bconstant\\b|\\bwhile\\b|\\bif\\b|\\bprocedure\\b|\\bcall\\b|\\bvariable\\b") };

	ParserForPQL(string input);
	~ParserForPQL();

	QueryTree getQueryTree();

private:
	QueryTree queryTree;
	void parse(string input);
	void setQueryTree(QueryTree queryTree);

	void getPosition(string input, int typeNo);
	int addNumOfPos(int number, smatch match, int typeNo);
	void addInFinalSyn(int typeNo, string input);
	void getSyn(string input, int typeNo);

	void parseTypeWithSyn(vector<string> selectSynonym, vector<vector<string>> suchThatSynonym,
		vector<vector<string>> withSynonym,
		vector<vector<string>> patternSynonym);
	void validateType();
	void parseRespectively();
	void parseKeyword();
	void startValidate();
//	PKB *pkb;

};

