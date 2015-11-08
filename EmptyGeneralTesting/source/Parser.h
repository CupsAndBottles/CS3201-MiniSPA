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
	vector<int> ifIndex;
	vector<int> whileIndex;
	vector<int> ifIndexStmt;
	list<pair<int, string>> listOfStatements;
	vector<pair<int, int>> parentLink;
	vector<pair<int, int>> followLink;
	vector<pair<int, int>> indexAndType;
	vector<pair<int, string>> callsLink;
	list<pair<int, string>> stmtNoAndExpr;
	vector<pair<int, string>> stmtNoAndCalls;
	string currProcName;
	int currIndex = 0;
	int numOfProc = 0;
	int numOfElse = 0;
	int procNumInTble = 0;
	int currElse = 0;
	vector<pair<int, string>> varModifiedInProc;
	vector<pair<int, string>> varUsedInProc;
	vector<int> currFollows;
	stack<char> closeBracket;
	stack<char> openBracket;
	stack<pair<int, string>> ifStmtVec;
	list<pair<int, string>> containerElements;
	string prevStmt;
	string pStmt;
	int pStmtType = 0;
	int pStmtIndex = 0;

	Parser();
	~Parser();
	PKB* getPkb();
	string openFile(string fileName);
	string linesSplitted(list<pair<int, string>> linesToTest);
	list<pair<int, string>>splitLines(string lines);
	void Procedure();
	void addModifies();
	void addUses();
	void setRelationsInTable();
	void processNextPrev(int index, string stmt);
	void addToParent(int child);
	void processProcedure(int index, string statment);
	void processCalls(int index, string stmt);
	void processWhile(int index, string statment);
	void processIf(int index, string statement);
	void processElse(int index, string statement);
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
	void setProcEndNum(int procNum, int stmtNum);
	void pushOpenBracket();
	bool isVariable(char c);
	//	void insertIntoVarTable(int stmtNum, char var);
	bool constantValue;

	string userInput;

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
	bool isConstant(string s);
	void handleFollows(int index, string stmt);
	PKB *pkb = new PKB();
};