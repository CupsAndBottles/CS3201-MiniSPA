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

//V: set parents
void PKB::setParentT(int index, vector<int> parents)
{
	stmtTable[index].setParentT(parents);
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

//V 
void PKB::setChildrenT(int index, vector<int> childrenT)
{
	stmtTable.at[index].setChildrenT(childrenT);
	
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

//V
void PKB::setFollowsT(int index, vector<int> followsT)
{

	stmtTable.at[index].setFollowsT(followsT);
}

//V
void PKB::setFollowedByT(int index, vector<int> followedByT)
{

	stmtTable.at[index].setFollowedByT(followedByT);
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
	vector<int> procNos;
	vector<pair<int, int>> results;
	if (type1 == 2 && stmtNum != -1) {
		varNos = procTable.at(stmtNum).getModified();
		if (varIndex != -1) {
			for (int i = 0; i < varNos.size(); i++) {
				if (varNos.at(i) == varIndex) {
					results.push_back(std::make_pair(stmtNum, varIndex));
					break;
				}
			}
		}
		else {
			for (int i = 0; i < varNos.size(); i++) {
				results.push_back(std::make_pair(stmtNum, varNos.at(i)));
			}
		}
	}
	else if (type1 == 2 && stmtNum == -1 && varIndex == -1) {
		for (int i = 1; i < procTable.size(); i++) {
			varNos = procTable.at(i).getModified();
			for (int k = 0; k < varNos.size(); k++) {
				results.push_back(std::make_pair(i, varNos.at(k)));
			}
		}
	}
	else if (stmtNum != -1 && varIndex != -1) {
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
			if (type1 == stmtTable.at(stmtNos.at(i)).getType() || type1 == 1) {
				results.push_back(std::make_pair(stmtNos.at(i), varIndex));
			}
		}
	}
	else {
		for (int i = 1; i < stmtTable.size(); i++) {
			if (type1 == stmtTable.at(i).getType() || type1 == 1 || type2 == 3) {
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

	if ((type1 == UNDERSCORE) || (procIndexFirst == UNDEFINED)) {
		if ((type2 == UNDERSCORE) || (procIndexSecond == UNDEFINED)) {
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
		if ((type2 == UNDERSCORE) || (procIndexSecond == UNDEFINED)) {
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
	vector<int> procNos;
	vector<pair<int, int>> results;
	if (type1 == 2 && stmtNum != -1) {
		varNos = procTable.at(stmtNum).getUsed();
		if (varIndex != -1) {
			for (int i = 0; i < varNos.size(); i++) {
				if (varNos.at(i) == varIndex) {
					results.push_back(std::make_pair(stmtNum, varIndex));
					break;
				}
			}
		}
		else {
			for (int i = 0; i < varNos.size(); i++) {
				results.push_back(std::make_pair(stmtNum, varNos.at(i)));
			}
		}
	}
	else if (type1 == 2 && stmtNum == -1 && varIndex == -1) {
		for (int i = 1; i < procTable.size(); i++) {
				varNos = procTable.at(i).getUsed();
				for (int k = 0; k < varNos.size(); k++) {
					results.push_back(std::make_pair(i, varNos.at(k)));
				}
			}
	}
	else if (stmtNum != -1 && varIndex != -1) {
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
			if (type1 == stmtTable.at(stmtNos.at(i)).getType() || type1 == 1) {
				results.push_back(std::make_pair(stmtNos.at(i), varIndex));
			}
		}
	}
	else {
		for (int i = 1; i < stmtTable.size(); i++) {
			if (type1 == stmtTable.at(i).getType() || type1 == 1 || type2 == 3) {
				varNos = stmtTable.at(i).getUses();
				for (int k = 0; k < varNos.size(); k++) {
					results.push_back(std::make_pair(i, varNos.at(k)));
				}
			}
		}
	}
	return results;
	
}

//XH
std::vector<pair<int, int>> PKB::getParent(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{
	vector<int> childrenStmtNos;
	vector<int> parentStmtNos;
	vector<pair<int, int>> results;

	if (stmtNum1 != -1) {
		childrenStmtNos = stmtTable.at(stmtNum1).getChildren();

		for (int i = 0; i < childrenStmtNos.size(); i++) {
			if (stmtNum2 != -1) { // Parent(2, 6)
				if (stmtNum2 == childrenStmtNos.at(i)) {
					results.push_back(std::make_pair(stmtNum1, stmtNum2));
					break;
				}
			}
			else { // Parent(2, s/w/a/_/c)
				if (type2 == stmtTable.at(childrenStmtNos.at(i)).getType()) {
					results.push_back(std::make_pair(stmtNum1, childrenStmtNos.at(i)));
				}
			}
		}
	}
	else if (stmtNum2 != -1) { // Parent(s/w/a/_/c, 4)
		parentStmtNos = stmtTable.at(stmtNum2).getParent();

		for (int i = 0; i < parentStmtNos.size(); i++) {
			if (type1 == stmtTable.at(parentStmtNos.at(i)).getType()) {
				results.push_back(std::make_pair(parentStmtNos.at(i), stmtNum2));
			}
		}
	}
	else { // Parent(s/w/a/_/c, s/w/a/_/c)
		for (int i = 1; i < stmtTable.size(); i++) {
			if (type1 == stmtTable.at(i).getType()) {
				childrenStmtNos = stmtTable.at(i).getChildren();

				for (int j = 0; j < childrenStmtNos.size(); j++) {
					if (type2 == stmtTable.at(i).getType()) {
						results.push_back(std::make_pair(i, childrenStmtNos.at(j)));
					}
				}
			}
		}
	}
}

//ZH
std::vector<pair<int, int>> PKB::getFollows(TYPE type1, int stmt1, TYPE type2, int stmt2){
	vector<pair<int, int>> follows;

	if (stmt1 == -1) {
		if (stmt2 == -1){
			if (type1 == UNDERSCORE || type1 == STATEMENT) {
				if (type2 == UNDERSCORE || type2 == STATEMENT) {
					// follows(_,_) or follows(s1,s2)
					for (int i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getFollows() > 0) {
							follows.push_back(make_pair(i, stmtTable[i].getFollows()));
						}
					}
				}
				else {
					// follows(_ ,call/if/assign/while) or follows(s1, call/if/assign/while)
					for (int i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type2) {
							if (stmtTable[i].getFollows() > 0) {
								follows.push_back(make_pair(i, stmtTable[i].getFollows()));
							}
						}
					}
				}
			}
			else {
				if (type2 == UNDERSCORE || type2 == STATEMENT) {
					// follows(call/if/assign/while, _) or follows(call/if/assign/while, s1)
					for (int i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type1) {
							if (stmtTable[i].getFollowedBy() > 0) {
								follows.push_back(make_pair(stmtTable[i].getFollowedBy(),i));
							}
						}
					}
				}
				else {
					// follows(call/if/assign/while, call/if/assign/while)
					for (int i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type1) {
							if (stmtTable[i].getFollows() > 0) {
								if (stmtTable[stmtTable[i].getFollows()].getType() == type2) {
									follows.push_back(make_pair(i, stmtTable[i].getFollows()));
								}
							}
						}
					}
				}
			}

		}
		else {
			// stmt1 is -1, stmt2 is not -1
			if (stmtTable[stmt2].getFollowedBy() > 0) {
				follows.push_back(make_pair(stmtTable[stmt2].getFollowedBy(), stmt2));
			}
		}
	}
	else {
		if (stmt2 == -1) {
		// stmt 1 is not -1, stmt 2 is -1
			if (stmtTable[stmt1].getFollows() > 0) {
				follows.push_back(make_pair(stmt1, stmtTable[stmt1].getFollows()));
			}
		}
		else {
			//Both parameters are defined
			if (stmtTable[stmt1].getFollows() == stmt2) {
				follows.push_back(make_pair(stmt1, stmt2));
			}
		}
	}

}

std::vector<pair<int, int>> PKB::getParentT(TYPE type1, int stmtNum1, TYPE type2, int stmtNum2)
{	
	vector<int>parentT;
	vector<int>childrenT;
	vector<pair<int, int>> results;

	if ((type1 == STATEMENT) && (type2 == STATEMENT)) {
		//both are undefined ie (s1,s2)
		if ((stmtNum1 == -1) && (stmtNum2 == -1)) {
			//check if parent exists inefficient
			for (int i = 1; i < stmtTable.size(); i++) {
				if (stmtTable.at(i).getParent() != -1) {
					results.push_back(std::make_pair(stmtTable.at(i).getParent(), i));
					break;
				}
			}
			return results;

		//(s1,num)
		}else if ((stmtNum1 == -1) && (stmtNum2 != -1)) {
			parentT = extractParentT(stmtNum2);
			for (int i = 0; i < parentT.size; i++) {
				results.push_back(std::make_pair(parentT.at(i), stmtNum2));
			}
			return results;

		} //(num,s1)
		else if ((stmtNum1 != -1) && (stmtNum2 == -1)) {
			childrenT = extractChildrenT(stmtNum1);
			for (int i = 0; i < childrenT.size; i++) {
				results.push_back(std::make_pair(stmtNum1,childrenT.at(i)));
			}
			return results;

		} //(num,num)
		else if ((stmtNum1 != -1) && (stmtNum2 != -1)) {
			parentT = extractParentT(stmtNum2);
			for (int i = 0; i < parentT.size; i++) {
				if (parentT.at(i) == stmtNum1) {
					results.push_back(std::make_pair(stmtNum1, stmtNum2));
				}
			}
			return results;
		}
	}
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

//V: design Extractor required methods
int PKB::getParent(int stmtNum)
{
	return stmtTable[stmtNum].getParent();
}

//V: for the design extractor
std::vector<int> PKB::getChildren(int stmtNum)
{

	return stmtTable[stmtNum].getChildren();
}

//V: for the design extractor
int PKB::getFollows(int stmtNum)
{
	return stmtTable[stmtNum].getFollows();
}

//V: for the design extractor
int PKB::getFollowedBy(int stmtNum)
{
	return stmtTable[stmtNum].getFollowedBy();
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
