#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "DesignExtractor.h"
#include "Procedure.h"
#include "Stmt.h"
#include "Variable.h"
#include "Enum.h"

using namespace std;
typedef short PROC;

class TNode;

class PKB {
private:
	PKB() {};
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

	
	~PKB();
	static PKB* getInstanceOf();
	Procedure proc;
	DesignExtractor design;

	//PARSER->PKB
	//From ProcTable
	int setProcNameInProcTable(string procName); //tested
	void setStartNum(int index, int startNum); 
	void setEndNum(int index, int endNum); 
	void setProcModified(int index, vector<string> modifiedVar);
	void setProcUses(int index, vector<string> usesVar);
	void setProcCalls(vector<pair<int,string>> procCalls);
	void setProcCalledBy(int index, int called);
	
	//From StmtTable
	void setType(int type);
	void setParent(int index, int parentStmt); 
	void setParentT(int index, vector<int> parentStmts);
	void setChildren(vector<pair<int,int>> parentChildStmts); 
	void setChildrenT(int index, vector<int> childrenStmts);
	void setFollows(vector<pair<int, int>> follows);
	void setFollowedBy(int index, int followedBy);
	void setFollowsT(int index, vector<int> followsTStmts);
	void setFollowedByT(int index, vector<int> followsByStmts);
	void setModifies(int index,string modifiedVar);
	//void setConstant(int index, vector<int> usesStmts);
	void setUsedVar(int index, string usedVar);
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
	vector<pair<int, int>> getModifies(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int varIndex);
	vector<pair<int, int>> getCalls(int stmtNum, int stmtNum2);
	vector<pair<int, int>> getUses(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);
	vector<pair<int, int>> getParent(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);
	vector<pair<int, int>> getFollows(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);
	vector<pair<int, int>> getParentT(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);
	vector<pair<int, int>> getFollowsT(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);

	//From ProcTable
	int getProcIndex(string procName);
	string getProcName(int procIndex);

	//From VarTable
	int getVarIndex(string varName);
	string getVarName(int index);
	vector<int> getProcNameInVarTable(int index);
	vector<int> getUsedByStmtNum(int index);
	vector<int> getModifiedByStmtNum(int index);
	 
	//From VarTable 
	int setVarName(string varName);//done
	void setProcNames(int index,string procName); //done
	void setUsedBy(string varName,int stmtNum); //done
	void setModifiedBy(string varName, int stmtNum);//done
	

	//While table
	int getWholeStmt(int, int);

	//If table
	void getIfStmt(int, int);

};
