#pragma once
#include <stddef.h> //defines null
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Procedure.h"
#include "Stmt.h"
#include "Variable.h"

using namespace std;

#include "PKB.h"
#include "DesignExtractor.h"

const int OFFSET = 1;
const int NOT_FOUND = -1;
const int UNDEFINED = -1;

PKB* PKB::m_Instance = NULL;

PKB* PKB::getInstanceOf()
{
	if (m_Instance)
		m_Instance = new PKB;
	return m_Instance;
}

//-----------------------------------------------------------------------------
//ProcTable Setters:

//G: check for existence, return index if there is the procName. Parser stops if not -1.
int PKB::setProcNameInProcTable(string procName)
{
	int index = getProcIndex(procName);
	if (index == -1) {
		int size = procTable.size();
		procTable[size].setProcName(procName);
		index = getProcIndex(procName);
	}

	return index;
	}

void PKB::setStartNum(int index, int startNum)
{
	procTable[index].setStartNo(startNum);
}

void PKB::setEndNum(int index, int endNum)
{
	procTable[index].setEndNo(endNum);
}

void PKB::setProcModified(int index, vector<int> modifiedVar)
{
	procTable[index].setModifiedVar(modifiedVar);

}

void PKB::setProcUses(int index, vector<int> usesVar)
{
	procTable[index].setUsedVar(usesVar);
}

//G: Yet to implement in table.
void PKB::setProcCalls(int index, string calls)
{
	int procIndex = getProcIndex(calls);
	procTable[index].setCalls(procIndex);
}

void PKB::setProcCalledBy(int index, string called)
{
	int procIndex = getProcIndex(called);
	procTable[index].setCalledBy(procIndex);
}
//----------------------------------------------------------------------------------------------
//Vartable Setters:

//G: check for existence, return index if exists else, set varname and return new index
int PKB::setVarName(string varName)
{
	int index = getVarIndex(varName);
	if (index = -1) {
		int size = varTable.size();
		varTable[size].setVarName(varName);
		index = getVarIndex(varName);
	}
	return index;
}

//G: get proc index from procTable and set in varTable
void PKB::setProcNames(int index, string procName)
{
	int procIndex = getProcIndex(procName);
	varTable[index].setProcNames(procIndex);

}

void PKB::setUsedBy(int index, int stmtNum)
{
	varTable[index].setUsedBy(stmtNum);
}

void PKB::setModifiedBy(int index, int stmtNum)
{
	varTable[index].setModifiedBy(stmtNum);
}

//----------------------------------------------------------------------------------------------------------------

//PKB::PKB()
//{
//}

PKB::~PKB()
{
}
//G: index not necessary. 
int PKB::setType(int type)
{
	int index = stmtTable.size();
	stmtTable[index].setStmtType(type);

	return index;
}

//G: parent set from setChildren method.
void PKB::setParent(int index, int parentStmt)
{
	stmtTable[index].setParent(parentStmt);
}

void PKB::setParentT(int index, vector<int> parentStmts)
{
	stmtTable.setParentT(index, parentStmts);
}

//G: children and parent together as pair
void PKB::setChildren(vector<pair<int, int>> parentChildStmts)
{
	while (!parentChildStmts.empty()) {
		pair<int, int> paired = parentChildStmts.back();
		int index = paired.first;
		int child = paired.second;
		parentChildStmts.pop_back();
		stmtTable[index].setChildren(child);
		setParent(child,index);
	}
}

void PKB::setChildrenT(int index, vector<int> childrenT)
{
	stmtTable.setChildrenT(stmtNum, childrenT);
}

//G: set Follows and FollowedBy in same method
void PKB::setFollows(int index, vector<pair<int,int>> follows)
{
	while (!follows.empty()) {
		pair<int, int> paired = follows.back();
		int firstStmt = paired.first;
		int secondStmt = paired.second;
		follows.pop_back();
		stmtTable[secondStmt].setFollows(firstStmt);
		setFollowedBy(firstStmt,secondStmt);
	}
}

void PKB::setFollowedBy(int index, int followedBy)
{
	stmtTable[index].setFollowedBy(followedBy);
}


void PKB::setFollowsT(int index, vector<int> followsTStmts)
{
	stmtTable.setFollowsT(index, followsTStmts);
}

void PKB::setFollowedByT(int index, vector<int> followsByStmts)
{
	stmtTable.setFollowedByT(index, followsByStmts);
}

//G: change variable passed as string to int and set stmttable.
void PKB::setModifies(int index, vector<string> modifiedVar)
{
	vector<int> modifiedVarIndex;
	while (!modifiedVar.empty()) {
		int i = getVarIndex(modifiedVar.back());
		modifiedVar.pop_back();
		modifiedVarIndex.push_back(i);
	}

	stmtTable[index].setModifiedVar(modifiedVarIndex);
}

/* G: Constants here or in another table?
void PKB::setConstant(int index, vector<int> usedConstant)
{
	stmtTable.setUsedConstant(index, usedConstant);
}
*/

//G: change variable passed as string to int and set stmttable.
void PKB::setUsedVar(int index, vector<string> usedVar)
{
	vector<int> usedVarIndex;
	while (!usedVar.empty()) {
		int i = getVarIndex(usedVar.back());
		usedVar.pop_back();
		usedVarIndex.push_back(i);
	}

	stmtTable[index].setUsedVar(usedVarIndex);
}

//ZH
void PKB::setRightExpr(int index, string expr)
{
	stmtTable[index].setRightExpr(expr);
}

//ZH
string PKB::getRightExpr(int index){
	return stmtTable[index].getRightExpression();

}

//ZH
int PKB::getNoOfStmt(){
	return this->stmtTable.size() - OFFSET;
}

//WL
//return <stmt no, varIndex>. 
std::vector<pair<int, int>> PKB::getModifies(TYPE type1, int stmtNum, TYPE type2, int varIndex)
{
	vector<int> stmtNos;
	vector<int> varNos;
	vector<pair<int, int>> results;
	if (stmtNum != -1 && varIndex != -1) {
		varNos = stmtTable.at(stmtNum).getModifies();
		for (int i = 0; i < varNos.size(); i++) {
			if (varNos.at(i) == varIndex) {
				results.push_back(std::make_pair(stmtNum, varIndex));
				break;
			}
		}
	}
	else if (stmtNum != -1) {
		varNos = stmtTable.at(stmtNum).getModifies();
		for (int i = 0; i < varNos.size(); i++) {
			results.push_back(std::make_pair(stmtNum, varNos.at(i)));
		}
	}
	else if (varIndex != -1) {
        stmtNos = varTable.at(varIndex).getModifiedBy();
		for (int i = 0; i < stmtNos.size(); i++) {
			if (type1 == stmtTable.at(stmtNos.at(i)).getType()) {
				results.push_back(std::make_pair(stmtNos.at(i), varIndex));
			}
		}
	}
	else {
		for (int i = 1; i < stmtTable.size(); i++) {
			if (type1 == stmtTable.at(i).getType()) {
				varNos = stmtTable.at(i).getModifies();
				for (int k = 0; k < varNos.size(); k++) {
	               results.push_back(std::make_pair(i, varNos.at(k)));
				}		
			}
		}
	}
	return results;
}

//ZH
vector<pair<int, int>> PKB::getCalls(TYPE type1, int procIndexFirst, TYPE type2, int procIndexSecond) {
	vector<pair<int, int>> result;
	vector<int> call;

	if (procIndexFirst == UNDEFINED) {
		if (procIndexSecond == UNDEFINED) {
			// Both undefined
			for (int i = 0; i < procTable.size(); i++) {
				call = procTable[i].getCalls();
				for (int j = 0; j < call.size(); j++) {
					result.push_back(make_pair(i, call[j]));
				}
			}
		}
		else {
			// Only Second defined
			call = procTable[procIndexSecond].getCalledBy();
			for (int j = 0; j < call.size(); j++) {
				result.push_back(make_pair(call[j], procIndexSecond));
			}
		}
	}
	else {
		if (procIndexSecond == UNDEFINED) {
			// Only first defined
			call = procTable[procIndexFirst].getCalls();
			for (int i = 0; i < call.size(); i++) {
				result.push_back(make_pair(procIndexFirst, call[i]));
			}
		}
		else {
			// both defined
			call = procTable[procIndexFirst].getCalls();
			for (int i = 0; i < call.size(); i++) {
				if (call[i] == procIndexSecond) {
					result.push_back(make_pair(procIndexFirst, procIndexSecond));
				}
			}
		}
	}
	
	return result;
}

//WL
std::vector<pair<int, int>> PKB::getUses(TYPE type1, int stmtNum, TYPE type2, int varIndex)
{
	vector<int> stmtNos;
	vector<int> varNos;
	vector<pair<int, int>> results;
	if (stmtNum != -1 && varIndex != -1) {
		varNos = stmtTable.at(stmtNum).getUses();
		for (int i = 0; i < varNos.size(); i++) {
			if (varNos.at(i) == varIndex) {
				results.push_back(std::make_pair(stmtNum, varIndex));
				break;
			}
		}
	}	
	else if(stmtNum != -1) {
		varNos = stmtTable.at(stmtNum).getUses();
		for (int i = 0; i < varNos.size(); i++) {
			results.push_back(std::make_pair(stmtNum, varNos.at(i)));
		}
	}
	else if (varIndex != -1) {
		stmtNos = varTable.at(varIndex).getUsedBy();
		for (int i = 0; i < stmtNos.size(); i++) {
			if (type1 == stmtTable.at(stmtNos.at(i)).getType()) {
				results.push_back(std::make_pair(stmtNos.at(i), varIndex));
			}
		}
	}
	else {
		for (int i = 1; i < stmtTable.size(); i++) {
			if (type1 == stmtTable.at(i).getType()) {
				varNos = stmtTable.at(i).getUses();
				for (int k = 0; k < varNos.size(); k++) {
					results.push_back(std::make_pair(i, varNos.at(k)));
				}
			}
		}
	}
	return results;
	
}

std::vector<pair<int, int>> PKB::getParent(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	if (type1 == STATEMENT) {
		if (type2 == STATEMENT) {
			return std::vector<pair<int, int>>();
		}
		else if (type2 == ASSIGN) {
			return std::vector<pair<int, int>>();
		}
		else {
			return std::vector<pair<int, int>>(); //if not statement/assign, will be call
		}
	}
	else if (type1 == WHILE) {
		if (type2 == STATEMENT) {
			return std::vector<pair<int, int>>();
		}
		else if (type2 == ASSIGN) {
			return std::vector<pair<int, int>>();
		}
		else {
			return std::vector<pair<int, int>>(); //if not stmt/assign, will be call
		}
	}
	else {
		if (type1 == IF) {
			if (type2 == STATEMENT) {
				return std::vector<pair<int, int>>();
			}
			else if (type2 == ASSIGN) {
				return std::vector<pair<int, int>>();
			}
		}
	}
}

std::vector<pair<int, int>> PKB::getFollows(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	if (type1 == STATEMENT) {
		if (type2 == ASSIGN) {
			return std::vector<pair<int, int>>();
		}
	}
	else if (type1 == ASSIGN) {
		if (type2 == ASSIGN) {
			return std::vector<pair<int, int>>();
		}
		else {
			return std::vector<pair<int, int>>(); //if not assign,it is call
		}
	}
	else if (type1 == WHILE) {
		if (type2 == ASSIGN) {
			return std::vector<pair<int, int>>();
		}
		else {
			return std::vector<pair<int, int>>(); //if not assign, it is if
		}
	}
	else {
		if (type1 == IF) {
			if (type2 == ASSIGN) {
				return std::vector<pair<int, int>>();
			}
		}
	}

}

std::vector<pair<int, int>> PKB::getParentT(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}

std::vector<pair<int, int>> PKB::getFollowsT(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}


//WL
vector<int> PKB::getProcNameInVarTable(int index)
{
	return varTable.at(index).getProcNames();
}

//WL
vector<int> PKB::getUsedByStmtNum(int index)
{
	return varTable.at(index).getUsedBy();
}

//WL
vector<int> PKB::getModifiedByStmtNum(int index)
{
	return varTable.at(index).getModifiedBy();
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
	return design.extractFollowedByT(stmtNum);
}

// These methods might not be required
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

//ZH
int PKB::getProcIndex(string procName){

	for (int i = 0; i < procTable.size(); i++) {
		if (this->procTable[i].getName() == procName) {
			return i;
		}
	}
	return NOT_FOUND;
}

//ZH
string PKB::getProcName(int procIndex)
{
	return procTable[procIndex].getName();
}


//ZH
int PKB::getVarIndex(string varName)
{
	bool isFound = false;

	for (int i = 0; i < varTable.size(); i++) {
		if (this->varTable[i].getVarName() == varName) {
			return i;
		}
	}

	return NOT_FOUND;
}

//ZH
string PKB::getVarName(int index)
{
	return varTable[index].getVarName();
}
