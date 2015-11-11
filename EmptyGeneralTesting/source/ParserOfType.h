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

	vector<vector<string>> setType(int clauseType, string synonym, vector<string> type, vector<string> synonymType, string indication);
	vector<vector<string>> setClauseType(int clauseType, string synonym, vector<string> type, vector<string> synonymType);
	vector<vector<string>> setDigitTypeAndSyn(string clauseType, string synonym, vector<string> type);
	vector<vector<string>> setDigitTypeAndSyn(string clauseType, string synonym, vector<string> type, vector<string> synType);
	vector<vector<string>> setVariableTypeAndSyn(string synonym);
	vector<vector<string>> setProcedureTypeAndSyn(string synonym, string clauseType);
	vector<vector<string>> setDeclaredSynAndType(string synonym, vector<string> type, vector<string> synonymType);
	bool isSynDigit(string syn);
	int isBeingDeclared(string syn, vector<string> synType);
	string checkSubExpression(string arg2);
	string removeUnwanted(string syn);
	string removeOpenComma(string syn);
	bool isExpression(string syn); 
	//PKB *pkb = PKB::getInstanceOf();
	PKB *pkb = new PKB();
};


