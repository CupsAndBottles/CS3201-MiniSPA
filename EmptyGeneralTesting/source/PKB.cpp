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

const int OFFSET = 1;
const int NOT_FOUND = -1;

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
int PKB::setProcNameInProcTable(string procedure)
{
	int index = procTable.getProcIndexNo(procedure);
	if (index == -1) {
		procTable.setProcName(procedure);
		index = procTable.getProcIndexNo(procedure);
	}

	return index;
	}

void PKB::setStartNum(int index, int startNum)
{
	procTable.setStartStmtNo(index, startNum);
}

void PKB::setEndNum(int index, int endNum)
{
	procTable.setEndStmtNo(index, endNum);
}

void PKB::setProcModified(int index, vector<int> modifiedVar)
{
	procTable.setModify(index, modifiedVar);

}

void PKB::setProcUses(int index, vector<int> usesVar)
{
	procTable.setUses(index, usesVar);
}

//G: Yet to implement in table.
void PKB::setProcCalls(int index, string callProc)
{
	//not implemented yet
}

//----------------------------------------------------------------------------------------------
//Vartable Setters:

//G: check for existence, return index if exists else, set varname and return new index
int PKB::setVarName(string varName)
{
	int index = varTable.getIndex(varName);
	if (index = -1) {
		varTable.setVarName(varName);
		index = varTable.getIndex(varName);
	}
	return index;
}

//G: get proc index from procTable and set in varTable
void PKB::setProcNames(int index, string procName)
{
	int procIndex = procTable.getProcIndexNo(procName);
	varTable.setProcNames(index,procIndex);

}

void PKB::setUsedBy(int index, int stmtNum)
{
	varTable.setUsedBy(index,stmtNum);
}

void PKB::setModifiedBy(int index, int stmtNum)
{
	varTable.setModifiedBy(index,stmtNum);
}

//----------------------------------------------------------------------------------------------------------------

//PKB::PKB()
//{
//}

PKB::~PKB()
{
}
//G: index not necessary. 
void PKB::setType(int type)
{
	stmtTable.setStmtType(type);
}

//G: parent set from setChildren method.
void PKB::setParent(int index, int parentStmt)
{
	stmtTable.setParent(index, parentStmt);
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
		stmtTable.setChildren(index, child);
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
		stmtTable.setFollows(secondStmt,firstStmt);
		setFollowedBy(firstStmt,secondStmt);
	}
}

void PKB::setFollowedBy(int index, int followedBy)
{
	stmtTable.setFollowedBy(index,followedBy);
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
		int i = varTable.getIndex(modifiedVar.back());
		modifiedVar.pop_back();
		modifiedVarIndex.push_back(i);
	}

	stmtTable.setModified(index, modifiedVarIndex);
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
		int i = varTable.getIndex(usedVar.back());
		usedVar.pop_back();
		usedVarIndex.push_back(i);
	}

	stmtTable.setUsedVar(index, usedVarIndex);
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
	if (stmtNum != -1) {
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
		for (int i = 0; i < stmtTable.size(); i++) {
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

std::vector<pair<int, int>> PKB::getCalls(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	return std::vector<pair<int, int>>();
}

//WL
std::vector<pair<int, int>> PKB::getUses(TYPE type1, int stmtNum, TYPE type2, int varIndex)
{
	vector<int> stmtNos;
	vector<int> varNos;
	vector<pair<int, int>> results;
	if (stmtNum != -1) {
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
		for (int i = 0; i < stmtTable.size(); i++) {
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
