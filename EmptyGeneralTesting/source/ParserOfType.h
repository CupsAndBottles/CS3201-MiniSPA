#pragma once
#pragma once
#include<vector>
#include<string>
#include<regex>
#include "PKB.h"

using namespace std;
class ParserOfType
{
public:
	ParserOfType();
	~ParserOfType();

	vector<vector<string>> setType(int clauseType, string synonym, vector<string> type, vector<string> synonymType);
	vector<vector<string>> setClauseType(int clauseType, string synonym, vector<string> type, vector<string> synonymType);
	bool isSynDigit(string syn);
	int isBeingDeclared(string syn, vector<string> synType);
	string checkSubExpression(string arg2);
	string removeUnwanted(string syn);
	bool isVariable(string syn);
	bool isProcedure(string syn);
	bool isExpression(string syn); 
	PKB *pkb = PKB::getInstanceOf();
};


