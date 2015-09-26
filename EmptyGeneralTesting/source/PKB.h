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
#include "VarTable.h"

enum TYPE { ASSIGN, STATEMENT, PROCEDURE, UNDERSCORE, WHILE, IF, VARIABLE, CONSTANT, CALLS };
using namespace std;
typedef short PROC;

class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
private:
	PKB() {};
	PKB(const PKB&) {};
	PKB& operator = (PKB const&) {};
	static PKB* m_Instance;
public:
	//	string procNum;
	string procName;
	string varName;
	int procIndex;
	int index;
	int startNum;
	int varIndex;
	int stmtNum;
	int modify;
	int child;
	string procedure;
	vector<int> varModifiedList;


	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST(PROC p);
	~PKB();
	static PKB* getInstanceOf();
	VarTable varTable;
	ProcTable procTable;
	StmtTable stmtTable;
	Procedure proc;
	Stmt stmts;
	Variable var;
	DesignExtractor design;

	//PARSER->PKB
	//From ProcTable
	int setProcNameInProcTable(string);
	void setStartNum(int, int);
	void setEndNum(int, int);
	void setProcModified(int, string);
	void setProcUses(int, string);
	void setProcCalls(int, string);
	//From VarTable
	void getProcNameInVarTable(int, string);
	void getUsedByStmtNum(int, int);
	void getModifiedByStmtNum(int, int);
	void setVarName(string);
	//From StmtTable
	void setType(int, int);
	void setParent(int, int);
	void setParentT(int, vector<int>);
	void setChildren(vector<pair<int, int>>);
	void setChildrenT(int, vector<int>);
	void setFollows(vector<pair<int, int>>);
	void setFollowsT(int, vector<int>);
	void setFollowedByT(int, vector<int>);
	void setModifies(int, vector<int>);
	void setUses(int, vector<int>);
	void setRightExpr(int, string);
	string getRightExpr(int);
	int getNumStmt();

	//PKB->DESIGNEXT
	void extractParentT(int);
	void extractChildrenT(int);
	void extractFollowsT(int);
	void extractFollowedByT(int);
	//DESIGNEXT->PKB
	int getParent(int);
	int getChildren(int);
	int getFollows(int);
	int getFollowedBy(int);

	//PQL-PKB
	std::vector<pair<int, int>> getModifies(TYPE, int, TYPE, int);
	std::vector<pair<int, int>> getCalls(TYPE, int, TYPE, int);
	std::vector<pair<int, int>> getUses(TYPE, int, TYPE, int);
	std::vector<pair<int, int>> getParent(TYPE, int, TYPE, int);
	std::vector<pair<int, int>> getFollows(TYPE, int, TYPE, int);
	std::vector<pair<int, int>> getParentT(TYPE, int, TYPE, int);
	std::vector<pair<int, int>> getFollowsT(TYPE, int, TYPE, int);

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
