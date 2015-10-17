#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "DesignExtractor.h"
#include "Procedure.h"
#include "Stmt.h"
#include "Variable.h"
#include "Constant.h"
#include "Enum.h"

using namespace std;
typedef short PROC;

class TNode;

class PKB {
private:
	vector<Stmt> stmtTable;
	vector<Variable> varTable;
	vector<Procedure> procTable;
	vector<Constant> constantTable;
	
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

	PKB();
	~PKB();
	Procedure proc;
	//DesignExtractor design;

	//PARSER->PKB
	void setByDesignExtractor();
	//From ProcTable
	int setProcNameInProcTable(string procName); //tested // working
	void setStartNum(int index, int startNum);  //working
	void setEndNum(int index, int endNum); //working
	void setProcModified(int index, vector<string> modifiedVar);//working
	void setProcUses(int index, vector<string> usesVar);//working
	string setProcCalls(vector<pair<int,string>> procCalls); 
	void setProcCalledBy(int index, int called);
	void setStmtNumProcCalled(vector<pair<int, string>> stmtNoAndCalls);

	
	void setProcCallsT(int index, vector<int> callsT);
	void setProcCalledByT(int index, vector<int> calledT);
	void setProcModifies(int index, vector<int> modifiesVar);
	void setProcUses(int index, vector<int> usesVar);

	//From StmtTable
	void setType(Enum::TYPE type);	//tested //done
	void setParentT(int index, vector<int> parentStmts); 
	void setChildren(vector<pair<int,int>> parentChildStmts); //working
	void setChildren(int index, int child); //working
	void setChildrenT(int index, vector<int> childrenStmts);
	void setFollows(vector<pair<int, int>> follows); // tested //working
	void setFollowedBy(int index, int followedBy); //working
	void setFollowsT(int index, vector<int> followsTStmts);
	void setFollowedByT(int index, vector<int> followsByStmts); 
	void setModifies(int index,string modifiedVar);//working
	void setControlVar(int index, int varIndex);
	void setUsedVar(int index, string usedVar);//working
	void setRightExpr(int index, string rightExpression);  //tested //working
	void setParent(int index, int parentStmt);  //working
	//void setCallsT(int index, vector<int> callsTStmts);

	void setNext(int index, int next);
	void setPrev(int index, int prev);

	void setCallsStmtModifiesUses();
	void setNextT(int index, vector<int> nextT);
	void setPrevT(int index, vector<int> prevT);

	//constant table
	int setConstant(int constantValue);
	void setStmtUsed(int index,int stmtNum);

	string getRightExpr(int index); //tested
	int getNoOfStmt(); //tested

	//PKB->DESIGNEXT
	void extractParentT(int index);
	void extractChildrenT(int index);
	void extractFollowsT(int index);
	void extractFollowedByT(int index);
	void extractCallsT(int index);
	void extractCalledByT(int index);
	void extractProcExtraModifiesUses();
	void extractNextT(int index);
	void extractPrevT(int index);

	//DESIGNEXT->PKB
	int getParent(int index);
	vector<int> getChildren(int index);
	int getFollows(int index);	
	int getFollowedBy(int index);
	vector<int> getFollowsT(int stmtNum);
	vector<int> getParentT(int stmtNum);
	vector<int> getFollowedByT(int stmtNum);
	vector<int>getChildrenT(int stmtNum);

	//PQL-PKB
	vector<pair<int, int>> getModifies(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int varIndex);
	vector<pair<int, int>> getCalls(int stmtNum, int stmtNum2);
	vector<pair<int, int>> getUses(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);
	vector<pair<int, int>> getParent(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);
	vector<pair<int, int>> getFollows(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2); // tested
	vector<pair<int, int>> getParentT(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);
	vector<pair<int, int>> getFollowsT(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int stmtNum2);

	//From ProcTable
	int getProcIndex(string procName);
	string getProcName(int procIndex);
	vector<int> getProcModified(int procIndex);
	vector<int> getProcUsed(int procIndex);
	vector<int> getProcCalls(int procIndex);
	vector<int> getProcCalledBy(int procIndex);
	vector<int> getCallsT(int procIndex);
	vector<int> getCalledByT(int procIndex);
	vector<int> getStmtNumProcCalled(int procIndex);

	//From VarTable
	int getVarIndex(string varName); //tested
	string getVarName(int index); //tested
	vector<int> getProcNameInVarTable(int index); // tested
	vector<int> getUsedByStmtNum(int index);
	vector<int> getModifiedByStmtNum(int index);
	 
	//From VarTable 
	int setVarName(string varName); //working - alphabets working, word is buggy. no uppercase, lowercase diff
	void setProcNames(int index,string procName); //working - need to do for multiple procs
	void setUsedBy(string varName,int stmtNum);  //working
	void setModifiedBy(string varName, int stmtNum); //working
	 
	int getType(int index);

	int getNoOfProc();
	int getNoOfVar();


	//From ConstantTable
	int getConstantIndex(int constant);


	int getControlVar(int stmtIndex);


};
