#pragma once
#include <stddef.h> //defines null
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"

PKB* PKB::m_Instance = NULL;

PKB* PKB::getInstanceOf()
{
	if (m_Instance)
		m_Instance = new PKB;
	return m_Instance;
}
int PKB::setProcNameInProcTable(string)
{
	return 0;
}

void PKB::setStartNum(int, int)
{
}

void PKB::setEndNum(int, int)
{
}

void PKB::setProcModified(int, string)
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
}

void PKB::setChildren(vector<pair<int, int>>)
{
}

void PKB::setChildrenT(int, vector<int>)
{
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


int PKB::getProcIndex(string)
{
	return 0;
}

string PKB::getProcName(int)
{
	return string();
}

int PKB::getVarIndex(string varName)
{

	return 0;
}

string PKB::getVarName(int index)
{

	return 0;

}

int PKB::getWholeStmt(int, int)
{
	return 0;
}

void PKB::getIfStmt(int, int)
{
}
