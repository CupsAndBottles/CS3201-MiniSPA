#pragma once
#include "iostream"
#include <fstream>
#include <string>
#include <list>
#include <stack>
#include <map>
#include "PKB.h"

using namespace std;

class Parser

{
	char nextChar;
	int charType;
public:
	Parser();
	~Parser();
	PKB* getPkb();
	string openFile(string fileName);
	string linesSplitted(list<pair<int, string>> linesToTest);
	list<pair<int, string>>splitLines(string lines);
	void Procedure();
	void setRelationsInTable();
	void addToParent(int child);
	string toLowerCase(string s);
	void processProcedure(int index, string statment);
	void processCalls(int index, string stmt);
	void processWhile(int index, string statment);
	void handleModifyAndUses(int i, string stmt);
	string getParentChild();
	string getExpression();
	string getFollow();
	void setExprInStmtTable(int index, list<char> exprOutput);
	void Error();
	int isPriority(const char & c);
	void pushCloseBracket(int stmtNum);
	bool isOperator(char o);
	void processExpressions(int index, string statement);
	void pushOpenBracket();
	bool isVariable(char c);
	//	void insertIntoVarTable(int stmtNum, char var);
	bool isConstant(char c);
	bool constantValue;

	string userInput;
	void setProcEndNum(int stmtNum);

	int stmtCount;
	int countOfVecs = 0;

private:
	string fileName;
	string proc_name;
	string currLine;
	string allLines;
	int stmtNum;
	int procIndex;
	string procName;
	list<pair<int, string >> lines;
	int cBrack;
	void handleFollows(int index, string stmt);
	PKB *pkb = new PKB();
};