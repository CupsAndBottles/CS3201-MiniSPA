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
	string procedure;
	vector<int> varModifiedList;
	vector<int>parentT;
	vector<int> childrenT;

	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST(PROC p);
	~PKB();
	static PKB* getInstanceOf();
	VarTable varTable;
	ProcTable procTable;
	StmtTable stmtTable;
	Procedure proc;
	DesignExtractor design;
	//Stmt stmts;
	//Variable var;

	//PARSER->PKB
	//From ProcTable
	int setProcNameInProcTable(string procName);
	void setStartNum(int index, int startNum);
	void setEndNum(int index, int endNum);
	void setProcModified(int index, vector<int> modifiedVar);
	void setProcUses(int index, vector<int> usesVar);
	void setProcCalls(int index, string callProc);

	//From StmtTable
	void setType(int index, int type);
	void setParent(int index, int parentStmt);
	void setParentT(int index, vector<int> parentStmts);
	void setChildren(int index, vector<int> parentChildStmts);
	void setChildrenT(int index, vector<int> childrenStmts);
	void setFollows(int index, int follows);
	void setFollowsT(int index, vector<int> followsTStmts);
	void setFollowedByT(int index, vector<int> followsByStmts);
	void setModifies(int index, vector<int> modifiesStmts);
	void setConstant(int index, vector<int> usesStmts);
	void setUsedVar(int index, vector<int> usesStmts);
	void setRightExpr(int index, string rightExpression);
	string getRightExpr(int index);
	int getNoOfStmt();

	//PKB->DESIGNEXT
	vector<int> extractParentT(int index);
	vector<int> extractChildrenT(int index);
	vector<int> extractFollowsT(int index);
	vector<int> extractFollowedByT(int index);

	//DESIGNEXT->PKB
	int getParent(int index);
	vector<int> getChildren(int index);
	int getFollows(int index);
	int getFollowedBy(int index);

	//PQL-PKB
	std::vector<pair<int, int>> getModifies(TYPE type1, int stmtNum, TYPE type2, int varIndex);
	std::vector<pair<int, int>> getCalls(TYPE type1, int stmtNum, TYPE type2, int stmtNum2);
	std::vector<pair<int, int>> getUses(TYPE type1, int stmtNum, TYPE type2, int stmtNum2);
	std::vector<pair<int, int>> getParent(TYPE type1, int stmtNum, TYPE type2, int stmtNum2);
	std::vector<pair<int, int>> getFollows(TYPE type1, int stmtNum, TYPE type2, int stmtNum2);
	std::vector<pair<int, int>> getParentT(TYPE type1, int stmtNum, TYPE type2, int stmtNum2);
	std::vector<pair<int, int>> getFollowsT(TYPE type1, int stmtNum, TYPE type2, int stmtNum2);

	//From ProcTable
	int getProcIndex(string procName);
	string getProcName(int procIndex);

	//From VarTable
	int getVarIndex(string varName);
	string getVarName(int index);
	string getProcNameInVarTable(int index);
	int getUsedByStmtNum(int index);
	int getModifiedByStmtNum(int index);
	 
	//From VarTable --> SOMEONE PLS ADD IN SET METHOD
	void setVarName(string varName);

	//While table
	int getWholeStmt(int, int);

	//If table
	void getIfStmt(int, int);

};
