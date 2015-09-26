#pragma once
#include <stddef.h> //defines null
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

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
	if (index != 0) {
		index = setProcNameInProcTable(procedure);
		startNum = procTable.getStartStmtNo(procedure);
	}


}

void PKB::setEndNum(int index, int endNum)
{

	if (index != 0) {
		index = setProcNameInProcTable(procedure);
		endNum = procTable.getEndStmtNo(procedure);
	}



}

void PKB::setProcModified(int index, string modifiedVar)
{
	if (varTable.getNoOfVar() != 0) {
		index = getProcIndex(varName);
		modifiedVar = getVarName(index);
	}
	procTable.getVarModified(modifiedVar);

}

void PKB::setProcUses(int index, string usesVar)
{
	if (varTable.getNoOfVar() != 0) {
		index = getProcIndex(procName);
		usesVar = getVarName(index);
	}
	procTable.getVarUsed(usesVar);
}


void PKB::setProcCalls(int, string)
{
	//not implemented yet
}

void PKB::setVarName(string varName)
{
	varName = getVarName(index);
}


//PKB::PKB()
//{
//}

PKB::~PKB()
{
}
void PKB::setType(int index, int type)
{
	stmts.getType();
}

void PKB::setParent(int index, int parent)
{
	stmtTable.getParent(index);
}

void PKB::setParentT(int index, vector<int> parentT)
{
	stmtTable.setParentT(stmtNum, parentT);
}

void PKB::setChildren(vector<pair<int, int>> parentChild)
{
	stmtTable.getChildren(stmtNum);
}

void PKB::setChildrenT(int index, vector<int>childrenT)
{
	stmtTable.setChildrenT(stmtNum, childrenT);
}

void PKB::setFollows(vector<pair<int, int>>)
{
	stmtTable.getFollows(index);
}

void PKB::setFollowsT(int index, vector<int>follows)
{
	stmtTable.setFollowsT(stmtNum, follows);
}

void PKB::setFollowedByT(int index, vector<int> followedByT)
{
	stmtTable.getFollowedByT(index);
}

void PKB::setModifies(int index, vector<int> modify)
{
	stmtTable.getModifies(index);
}

void PKB::setUses(int index, vector<int> uses)
{
	stmtTable.getUses(index);
}

void PKB::setRightExpr(int index, string expr)
{
	setRightExpr(index, (getRightExpr(index)));
}

string PKB::getRightExpr(int index)
{
	return (stmtTable.getRightExpression(index));

}

int PKB::getNumStmt()
{
	stmtTable.getNoOfStmts();
	return 0;
}

int PKB::setProcToAST(PROC p, TNode* r) {
	return NULL;
}

TNode* PKB::getRootAST(PROC p) {
	return NULL;
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



void PKB::getProcNameInVarTable(int index, string procName)
{
	if (varTable.getNoOfVar() != 0) {
		if (varTable.isVariableName(varName) == true) {
			index = getVarIndex(varName);
			procName = getProcName(index);
		}
	}
}

void PKB::getUsedByStmtNum(int index, int stmtNum)
{
	if (procTable.getNoOfProc() != 0) {
		if (getVarIndex(varName) != 0) {
			stmtNum = procTable.getStartStmtNo(procName);
		}
	}
}

void PKB::getModifiedByStmtNum(int index, int stmtNum)
{
	if (procTable.getNoOfProc() != 0) {
		if (getVarIndex(varName) != 0) {
			stmtNum = procTable.getStartStmtNo(procName);
		}
	}
}

void PKB::extractParentT(int stmtNum)
{
	design.extractParentT(stmtNum);
}

void PKB::extractChildrenT(int stmtNum)
{
	design.extractChildrenT(stmtNum);
}

void PKB::extractFollowsT(int stmtNum)
{
	design.extractFollowsT(stmtNum);
}

void PKB::extractFollowedByT(int stmtNum)
{
	design.extractFollowedByT(stmtNum);
}

int PKB::getParent(int stmtNum)
{
	return stmtTable.getParent(stmtNum);
}

int PKB::getChildren(int stmtNum)
{
	return 0;
}

int PKB::getFollows(int)
{
	return stmtTable.getFollows(stmtNum);
}

int PKB::getFollowedBy(int)
{
	return stmtTable.getFollowedBy(stmtNum);
}


int PKB::getProcIndex(string procName)
{
	procName = proc.getName();
	procIndex = proc.getIndexNum(procName);
	return procIndex;
}

string PKB::getProcName(int procIndex)
{
	procIndex = getProcIndex(procName);
	index = procIndex;
	return procTable.getProcName(index);
}

int PKB::getVarIndex(string varName)
{
	varName = getVarName(index);
	varIndex = getVarIndex(varName);
	return varIndex;

}

string PKB::getVarName(int index)
{
	index = var.getindex();
	return varTable.getVarName(index);

}

int PKB::getWholeStmt(int startNum, int controlVar)
{
	startNum = procTable.getStartStmtNo(procedure);
	return startNum;
}

void PKB::getIfStmt(int startNum, int controlVar)
{
	startNum = procTable.getStartStmtNo(procedure);
}
