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
	}
}

void PKB::setStartNum(int index, int num)
{
	index = setProcNameInProcTable(procedure);
	startIndex = index;
	num = procTable.getStartStmtNo(procedure);
	startNum = num;
}

void PKB::setEndNum(int index, int endNum)
{
	if (index != 0) {
		index = setProcNameInProcTable(procedure);
	}
	if (endNum != 0) {
		endNum = procTable.getEndStmtNo(procedure);
	}

}

void PKB::setProcModified(int index, string modifiedVar)
{

}

void PKB::setProcUses(int, string)
{
}


void PKB::setProcCalls(int, string)
{
}

void PKB::setVarName(string)
{
}


//PKB::PKB()
//{
//}

PKB::~PKB()
{
}
void PKB::setType(int, int)
{
}

void PKB::setParent(int, int)
{
}

void PKB::setParentT(int, vector<int>)
{
	stmtTable.setParentT(stmtNum, parentT);
}

void PKB::setChildren(vector<pair<int, int>>)
{
}

void PKB::setChildrenT(int, vector<int>)
{
	stmtTable.setChildrenT(stmtNum, childrenT);
}

void PKB::setFollows(vector<pair<int, int>>)
{
}

void PKB::setFollowsT(int, vector<int>)
{
}

void PKB::setFollowedByT(int, vector<int>)
{
}

void PKB::setModifies(int, vector<int>)
{
}

void PKB::setUses(int, vector<int>)
{
}

void PKB::setRightExpr(int, string)
{
}

string PKB::getRightExpr(int)
{
	return string();
}

int PKB::getNumStmt()
{
	return 0;
}

int PKB::setProcToAST(PROC p, TNode* r) {
	return NULL;
}

TNode* PKB::getRootAST(PROC p) {
	return NULL;
}

std::vector<pair<int, int>> PKB::getModifies(TYPE, int, TYPE, int)
{
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getCalls(TYPE, int, TYPE, int)
{
	return std::vector<pair<int, int>>();
}
std::vector<pair<int, int>> PKB::getUses(TYPE, int, TYPE, int)
{
	return std::vector<pair<int, int>>();
}
std::vector<pair<int, int>> PKB::getParent(TYPE, int, TYPE, int)
{
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getFollows(TYPE, int, TYPE, int)
{
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getParentT(TYPE, int, TYPE, int)
{
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getFollowsT(TYPE, int, TYPE, int)
{
	return std::vector<pair<int, int>>();
}



void PKB::getProcNameInVarTable(int, string)
{
}

void PKB::getUsedByStmtNum(int, int)
{
}

void PKB::getModifiedByStmtNum(int, int)
{
}

void PKB::extractParent(int)
{

}

void PKB::extractChildren(int)
{
}

void PKB::extractFollowsT(int)
{
}

void PKB::extractFollowedByT(int)
{
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


int PKB::getProcIndex(string)
{
	return 0;
}

string PKB::getProcName(int procIndex)
{
	procIndex = proc.getIndexNum(procNum);
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

int PKB::getWholeStmt(int, int)
{
	return 0;
}

void PKB::getIfStmt(int, int)
{
}
