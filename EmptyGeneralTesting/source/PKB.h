#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "DesignExtractor.h"
#include "ProcTable.h"
#include "Procedure.h"
#include "StmtTable.h"
#include "Stmt.h"
#include "Variable.h"

enum class TYPE { ASSIGN, STATEMENT, PROCEDURE, UNDERSCORE, WHILE, IF, VARIABLE, CONSTANT, CALLS };
using namespace std;
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
private:
	PKB() {};
	PKB(const PKB&) {};
	PKB& operator = (PKB const&) {};
	static PKB* m_Instance;
public:
	static VarTable* varTable;
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST(PROC p);
	~PKB();
	static PKB* getInstanceOf();
	//	VarTable varTable;
	//	ProcTable procTable;
	//	StmtTable stmtTable;
	//PARSER->PKB
	//From VarTable
	void getProcNameInVarTable(int, string);
	void getUsedByStmtNum(int, int);
	void getModifiedByStmtNum(int, int);
	//DESIGNEXT->PKB
	void extractParent(int);
	void extractChildren(int);
	void extractFollowsT(int);
	void extractFollowedByT(int);
	//PQL-PKB
	std::vector<pair<int, int>> getCalls(TYPE, int);
	std::vector<pair<int, int>> getUses(TYPE, int);
	std::vector<pair<int, int>> getParent(TYPE, int);
	std::vector<pair<int, int>> getFollows(TYPE, int);
	std::vector<pair<int, int>> getParentT(TYPE, int);
	std::vector<pair<int, int>> getFollowsT(TYPE, int);

	//From ProcTable
	int getProcIndex(string);
	string getProcName(int);

	//From VarTable
	int getVarIndex(string);
	string getVarName(int);

	//While table
	int getWholeStmt(int, int);

	//If table
	void getIfStmt(int, int);





};

