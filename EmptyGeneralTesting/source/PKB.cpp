#pragma once
#include <stddef.h> //defines null
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Variable.h"

using namespace std;

#include "PKB.h"
#include "DesignExtractor.h"
PKB* PKB::m_Instance = NULL;

PKB* PKB::getInstanceOf()
{
	if (m_Instance)
		m_Instance = new PKB;
	return m_Instance;
}
int PKB::setProcNameInProcTable(string procedure)
{
	procedure = getProcName(index);
	if (procedure.size() == NULL) {
		return -1;
	}
	else {
		index = setProcNameInProcTable(procedure);
		return index;
	}
}

void PKB::setStartNum(int index, int startNum)
{
	procTable.setStartStmtNo(index, startNum);
}

void PKB::setEndNum(int index, int endNum)
{
	procTable.setStartStmtNo(index, endNum);
}

void PKB::setProcModified(int index, vector<int> modifiedVar)
{
	procTable.setModify(index, modifiedVar);

}

void PKB::setProcUses(int index, vector<int> usesVar)
{
	procTable.setUses(index, usesVar);
}


void PKB::setProcCalls(int index, string callProc)
{
	//not implemented yet
}

void PKB::setVarName(string varName)
{
	varTable.setVarName(varName);
}


//PKB::PKB()
//{
//}

PKB::~PKB()
{
}
void PKB::setType(int index, int type)
{
	stmtTable.setStmtType(index, type);
}

void PKB::setParent(int index, int parentStmt)
{
	stmtTable.setParent(index, parentStmt);
}

void PKB::setParentT(int index, vector<int> parentStmts)
{
	stmtTable.setParentT(index, parentStmts);
}

void PKB::setChildren(int index, vector<int> children)
{
	stmtTable.setChildren(index, children);
}

void PKB::setChildrenT(int index, vector<int> childrenT)
{
	stmtTable.setChildrenT(stmtNum, childrenT);
}

void PKB::setFollows(int index, int follows)
{
	stmtTable.setFollows(index, follows);
}

void PKB::setFollowsT(int index, vector<int> followsTStmts)
{
	stmtTable.setFollowsT(index, followsTStmts);
}

void PKB::setFollowedByT(int index, vector<int> followsByStmts)
{
	stmtTable.setFollowedByT(index, followsByStmts);
}

void PKB::setModifies(int index, vector<int> modifiesStmts)
{
	stmtTable.setModified(index, modifiesStmts);
}

void PKB::setConstant(int index, vector<int> usedConstant)
{
	stmtTable.setUsedConstant(index, usedConstant);
}

void PKB::setUsedVar(int index, vector<int> usedVar)
{
	stmtTable.setUsedVar(index, usedVar);
}

void PKB::setRightExpr(int index, string expr)
{
	stmtTable.setRightExpr(index, expr);
}

string PKB::getRightExpr(int index)
{
	return stmtTable.getRightExpression(index);

}

int PKB::getNoOfStmt()
{
	return stmtTable.getNoOfStmts();;
}

std::vector<pair<int, int>> PKB::getModifies(TYPE type1, int stmtNum, TYPE type2, int varIndex)
{
	if (type1 == ASSIGN) {
		return std::vector<pair<int, int>>();
	}
	else if (type1 == STATEMENT) {

	}
	else if (type1 == PROCEDURE) {

	}

	if (type2 == ASSIGN) {

	}
	else if (type2 == STATEMENT) {

	}
	else if (type2 == PROCEDURE) {

	}
	else if (type2 == CONSTANT) {

	}
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getCalls(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}
std::vector<pair<int, int>> PKB::getUses(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}
std::vector<pair<int, int>> PKB::getParent(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getFollows(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getParentT(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getFollowsT(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}



string PKB::getProcNameInVarTable(int index)
{
	
}

int PKB::getUsedByStmtNum(int index)
{
	//return varTable.setUsedBy(index);
}

int PKB::getModifiedByStmtNum(int index)
{
	//return varTable.setModifiedBy(index);
}

vector<int> PKB::extractParentT(int stmtNum)
{
	return design.extractParentT(stmtNum);
}

vector<int> PKB::extractChildrenT(int stmtNum)
{
	return design.extractChildrenT(stmtNum);
}

vector<int> PKB::extractFollowsT(int stmtNum)
{
	return design.extractFollowsT(stmtNum) ;
}

vector<int> PKB::extractFollowedByT(int stmtNum)
{
	design.extractFollowedByT(stmtNum);
}

int PKB::getParent(int stmtNum)
{
	return stmtTable.getParent(stmtNum);
}

std::vector<int> PKB::getChildren(int stmtNum)
{
	return stmtTable.getChildren(stmtNum);
}

int PKB::getFollows(int stmtNum)
{
	return stmtTable.getFollows(stmtNum);
}

int PKB::getFollowedBy(int stmtNum)
{
	return stmtTable.getFollowedBy(stmtNum);
}


int PKB::getProcIndex(string procName)
{
	int procIndex = procTable.getProcIndexNo(procName);
	return procIndex;
}

string PKB::getProcName(int procIndex)
{
	string procName = procTable.getProcName(procIndex);
	return procName;
}

int PKB::getVarIndex(string varName)
{
	int varIndex = varTable.getIndex(varName);
	return varIndex;

}

string PKB::getVarName(int index)
{
	string varName = varTable.getVarName(index);
	return varName;
}

//someone pls edit this
int PKB::getWholeStmt(int startNum, int controlVar)
{
//	startNum = procTable.getStartStmtNo(procedure);
	//return startNum;
}

void PKB::getIfStmt(int startNum, int controlVar)
{
	//startNum = procTable.getStartStmtNo(procedure);
}
