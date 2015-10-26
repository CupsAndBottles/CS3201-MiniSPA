#pragma once
#include <stddef.h>
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Procedure.h"
#include "Stmt.h"
#include "Variable.h"
#include "Enum.h"

using namespace std;

#include "PKB.h"
#include "DesignExtractor.h"

const int OFFSET = 1;
const int NOT_FOUND = -1;
const int UNDEFINED = -1;

//-----------------------------------------------------------------------------
//ProcTable Setters:

//ZH : tested
//G: check for existence, return index if there is the procName. Parser stops if not -1.
int PKB::setProcNameInProcTable(string procName)
{
	int index = getProcIndex(procName);
	if (index == -1) {
		procTable.push_back(Procedure());
		int size = procTable.size() - OFFSET;
		procTable[size].setProcName(procName);
		index = getProcIndex(procName);
	}
	//cout << "ProcName: "<<procName << "\n";
	//cout << "index: " << index<< "\n";
	return index;
}

void PKB::setStartNum(int index, int startNum)
{
	procTable[index].setStartNo(startNum);
	//cout << "index: "<<index<<"\n";
	//cout << "start: " << startNum << "\n";
}

void PKB::setEndNum(int index, int endNum)
{
	procTable[index].setEndNo(endNum);
	//cout << "index: " << index << "\n";
	//cout << "end: " << endNum << "\n";
}

void PKB::setProcModified(vector<pair<int,string>> modifiedVar)
{
	while (!modifiedVar.empty()) {
		int index = modifiedVar.back().first;
		int modifiedVarIndex = getVarIndex(modifiedVar.back().second);
		procTable[index].setModifiedVar(modifiedVarIndex);
		modifiedVar.pop_back();
	}

}

void PKB::setProcUses(vector<pair<int, string>> usedVar)
{
	while (!usedVar.empty()) {
		int index = usedVar.back().first;
		int usedVarIndex = getVarIndex(usedVar.back().second);
		procTable[index].setUsedVar(usedVarIndex);
		usedVar.pop_back();
	}

}

//index=procCalling, calls - procCalled
string PKB::setProcCalls(vector<pair<int, string>> procCalls)
{
	string procNot = "";
	while (!procCalls.empty()) {
		pair<int, string> paired = procCalls.back();
		int index = paired.first;
		string procCalled = paired.second;
		int procIndex = getProcIndex(procCalled);
		procCalls.pop_back();
		if (procIndex != -1) {
			procTable[index].setCalls(procIndex);
			setProcCalledBy(procIndex, index);
		}
		else {
			procNot = procCalled;
		}
	}
	return procNot;
}

void PKB::setProcCalledBy(int index, int called)
{
	procTable[index].setCalledBy(called);
}

void PKB::setStmtNumProcCalled(vector<pair<int, string>> stmtNoAndCalls)
{
	while (!stmtNoAndCalls.empty()) {
		pair<int, string> paired = stmtNoAndCalls.back();
		int index = getProcIndex(paired.second);
		int stmtNum = paired.first;
		stmtNoAndCalls.pop_back();
	//	cout << index << ":called \n";
	//	cout << stmtNum << ":stmtNum \n";
		procTable[index].setStmtNum(stmtNum);
	}
}

//V
void PKB::setProcCallsT(int index, vector<int> callsT) {
	procTable[index].setCallsT(callsT);
}

//V
void PKB::setProcCalledByT(int index, vector<int> calledByT) {
	procTable[index].setCalledByT(calledByT);
}

//V
void PKB::setProcModifies(int index, vector<int> modifies) {
	procTable[index].setUpdatedModifies(modifies);
}

//V
void PKB::setProcUses(int index, vector<int> uses) {
	procTable[index].setUpdatedUses(uses);
}
//----------------------------------------------------------------------------------------------
//Vartable Setters:

//ZH: tested
//G: check for existence, return index if exists else, set varname and return new index
int PKB::setVarName(string varName){
	
	int index = getVarIndex(varName);
	if (index == -1) {
		varTable.push_back(Variable());
		int size = varTable.size() - OFFSET;
		varTable[size].setVarName(varName);
		index = getVarIndex(varName);
	//	cout << "set varName: " << varName << "\n";
	//	cout << "set varIndex: " << index << "\n";
	}
	return index;
}

//G: get proc index from procTable and set in varTable
void PKB::setProcNames(int index, string procName)
{
	int procIndex = getProcIndex(procName);
	varTable[index].insertIntoProc(procIndex);
	//cout << "Procname in vartable: " << procName <<"\n";
	//cout << "Proc Index: " << procIndex<< "\n";

}

bool PKB::isConstant(string s) {
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it))
	{
		++it;
	}
	return !s.empty() && it == s.end();
}

void PKB::setUsedBy(string varName, int stmtNum)
{
	int varIndex = getVarIndex(varName);
	if (varIndex != -1) {
		varTable[varIndex].insertIntoUses(stmtNum);
	}
	else {
		if (!isConstant(varName)) {
			setVarName(varName);
			setUsedBy(varName, stmtNum);
		}
	}
}

void PKB::setModifiedBy(string varName, int stmtNum)
{
	int varIndex = getVarIndex(varName);
	if (varIndex != -1) {
	varTable[varIndex].insertIntoModify(stmtNum);
	}
	else {
		if (!isConstant(varName)) {
			setVarName(varName);
			setModifiedBy(varName, stmtNum);
		}
	}
	//cout << "varname: " << varName << "\n";
	//cout << "stmtNum: " << stmtNum << "\n";
}

//----------------------------------------------------------------------------------------------------------------

PKB::PKB()
{
}

PKB::~PKB()
{
}

//ZH: tested
//G: index not necessary. 
void PKB::setType(Enum::TYPE type) {

	if (stmtTable.size() == 0) {
		stmtTable.push_back(Stmt());
	}
	stmtTable.push_back(Stmt());
	int index = stmtTable.size() - OFFSET;
	stmtTable[index].setStmtType(type);
	//cout << "Index: "<<index << "\n";
	//cout << "Type: " << type << "\n";
}

//G: parent set from setChildren method.
void PKB::setParent(int index, int parentStmt)
{
	//cout << "Parent: " << parentStmt << "\n";
	//cout << "Child: " << index << "\n\n";
	stmtTable[index].setParent(parentStmt);
}

void PKB::setNext(int index, int next)
{
	if (index != next) {
		//cout << "Index: " << index << "\n";
		//cout << "Next: " << next << "\n\n";
		stmtTable[index].setNext(next);
	}
}

void PKB::setPrev(int index, int prev)
{
	if (index != prev) {
		//cout << "Index: " << index << "\n";
		//cout << "Prev: " << prev << "\n\n";
		stmtTable[index].setPrev(prev);
	}
}

int PKB::setConstant(int constantValue)
{
	int index = getConstantIndex(constantValue);
	if (index == -1) {
		constantTable.push_back(Constant());
		int size = constantTable.size() - OFFSET;
		constantTable[size].setConstantValue(constantValue);
		index = getConstantIndex(constantValue);
	}
	//cout << "constant val: " << constantValue << "\n";
	return index;
}

void PKB::setStmtUsed(int index, int stmtNum)
{
	constantTable[index].insertIntoStmtUsed(stmtNum);
	//cout << "index of constant: " << index << "\n";
	//cout << "stmt num: " << stmtNum << "\n";
}

//V
void PKB::setCallsStmtModifiesUses() {
	int stmtNum;
	vector<int> stmtNumbers;

	for (int i = 0; i < procTable.size(); i++) {
		stmtNumbers = getStmtNumProcCalled(i);
		if (stmtNumbers.size() == 0) {
			continue;
		}
		else {
			for (int j = 0; j < stmtNumbers.size(); j++) {
				int stmtNum = stmtNumbers.at(j);
				stmtTable.at(stmtNum).setCallsStmtModifiesVar(getProcModified(i));
				stmtTable.at(stmtNum).setCallsStmtUsesVar(getProcUsed(i));
			}
		}
	}
}

//V: set parents
void PKB::setParentT(int index, vector<int> parents)
{
	stmtTable[index].setParentT(parents);
}

//V
vector<int> PKB::getParentT(int stmtNum) {
	return stmtTable[stmtNum].getParentT();
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
		//cout << "Parent: " << index << "\n";
		//cout << "Child: " << child << "\n\n";
	}
}

void PKB::setChildren(int index, int child) {
	stmtTable[index].setChildren(child);
}
//V 
void PKB::setChildrenT(int index, vector<int> childrenT)
{
	stmtTable[index].setChildrenT(childrenT);
	
}

//ZH: tested
//G: set Follows and FollowedBy in same method
void PKB::setFollows(vector<pair<int,int>> follows)
{
	while (!follows.empty()) {
		pair<int, int> paired = follows.back();
		int firstStmt = paired.first;
		int secondStmt = paired.second;
		follows.pop_back();
		//cout << "first: " << firstStmt << "\n";
		//cout << "second: " << secondStmt << "\n\n";
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

	stmtTable[index].setFollowsT(followsT);
}

//V
void PKB::setFollowedByT(int index, vector<int> followedByT)
{

	stmtTable[index].setFollowedByT(followedByT);
}

//G: change variable passed as string to int and set stmttable.
void PKB::setModifies(int index, string modifiedVar)
{
	int varIndex = getVarIndex(modifiedVar);
	stmtTable[index].setModifiedVar(varIndex);
	/*cout << "---------------------------------------\n";
	cout << "Index : " << varIndex << "\n";
	cout << "Modified var: " << modifiedVar << "\n";
	cout << "stmt table:  " << index << "\n";
	cout << "---------------------------------------\n";*/
}

void PKB::setControlVar(int index, int varIndex)
{
	stmtTable[index].setControlVar(varIndex);
	/*cout << "Index : " << varIndex << "\n";
	string varName = getVarName(varIndex);
	cout << "Varname: " << varName << "\n";*/
}

/* G: Constants here or in another table?
void PKB::setConstant(int index, vector<int> usedConstant)
{
	stmtTable.setUsedConstant(index, usedConstant);
}
*/

//G: change variable passed as string to int and set stmttable.
void PKB::setUsedVar(int index, string usedVar)
{
	int varIndex = getVarIndex(usedVar);
	stmtTable[index].setUsedVar(varIndex);
	/*cout << "---------------------------------------\n";
	cout << "Index : " << varIndex << "\n";
	cout << "Used var: " << usedVar << "\n";
	cout << "stmt table:  " << index << "\n";
	cout << "---------------------------------------\n";*/
}

//ZH - tested
void PKB::setRightExpr(int index, string expr)
{
//	cout << "Index: " << index << "\n";
//	cout << "Expr: " << expr << "\n";

	stmtTable[index].setRightExpr(expr);
}

// V: Parser - PKB, called at start by parser
void PKB::setByDesignExtractor() {
	for (int i = OFFSET; i < stmtTable.size(); i++) {
	extractFollowsT(i);
	extractFollowedByT(i);
	}
	 
	for (int j = 0; j < procTable.size(); j++) {
		extractCallsT(j);
		extractCalledByT(j);
	}
	extractProcExtraModifiesUses();
	setCallsStmtModifiesUses();
	setCallStmtsParentTModifiesUses();
}

//V
void PKB::setParentTChildrenT() {
	for (int i = OFFSET; i < stmtTable.size(); i++) {
		extractParentT(i);
		extractChildrenT(i);
	}
}

//V
void PKB::setCallStmtsParentTModifiesUses() {
	vector<int> callStmtNum;
	vector<int> parentTList;
	vector<int> modifies, uses;
	vector<int> existingModifiesList;
	vector<int> existingUsesList;

	for (int i = 0; i < procTable.size(); i++) {
		callStmtNum = getStmtNumProcCalled(i);
		for (int j = 0; j < callStmtNum.size(); j++) {
			parentTList = stmtTable[callStmtNum.at(j)].getParentT();
			modifies = stmtTable[callStmtNum.at(j)].getModifies();
			uses = stmtTable[callStmtNum.at(j)].getUses();
			for (int k = 0; k < parentTList.size(); k++) {
				existingModifiesList = stmtTable[parentTList.at(k)].getModifies();
				for (int m = 0; m < modifies.size(); m++) {
					int modifiesVarIndex = modifies.at(m);
					if (find(existingModifiesList.begin(), existingModifiesList.end(), modifiesVarIndex) == existingModifiesList.end()) {
						existingModifiesList.push_back(modifiesVarIndex);
					}
				}
				stmtTable.at(parentTList.at(k)).setModifies(existingModifiesList);
				existingUsesList = stmtTable[parentTList.at(k)].getUses();
				for (int n = 0; n < uses.size(); n++) {
					int usesVarIndex = uses.at(n);
					if (find(existingUsesList.begin(), existingUsesList.end(), usesVarIndex) == existingUsesList.end()) {
						existingUsesList.push_back(usesVarIndex);
					}
				}
				stmtTable.at(parentTList.at(k)).setUses(existingUsesList);
			}
		}
	}
}

//ZH - tested
string PKB::getRightExpr(int index){
	return stmtTable[index].getRightExpression();

}

//ZH - tested
int PKB::getNoOfStmt(){
	if (stmtTable.size() == 0) {
		return 0;
	}
	else {
		return stmtTable.size() - OFFSET;
	}
}

//WL
//return <stmt no, varIndex>. 
std::vector<pair<int, int>> PKB::getModifies(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int varIndex)
{
	vector<int> stmtNos;
	vector<int> varNos;
	vector<int> procNos;
	vector<pair<int, int>> results;
	if (type1 == 2 && stmtNum != -1) {
		varNos = procTable.at(stmtNum).getModified();
		if (varIndex != -1) {
			for (size_t i = 0; i < varNos.size(); i++) {
				if (varNos.at(i) == varIndex) {
					results.push_back(std::make_pair(stmtNum, varIndex));
					break;
				}
			}
		}
		else {
			for (size_t i = 0; i < varNos.size(); i++) {
				results.push_back(std::make_pair(stmtNum, varNos.at(i)));
			}
		}
	}
	else if (type1 == 2 && stmtNum == -1 && varIndex == -1) {
		for (size_t i = 0; i < procTable.size(); i++) {
			varNos = procTable.at(i).getModified();
			for (size_t k = 0; k < varNos.size(); k++) {
				results.push_back(std::make_pair(i, varNos.at(k)));
			}
		}
	}
	else if (stmtNum != -1 && varIndex != -1) {
		varNos = stmtTable.at(stmtNum).getModifies();
		for (size_t i = 0; i < varNos.size(); i++) {
			if (varNos.at(i) == varIndex) {
				results.push_back(std::make_pair(stmtNum, varIndex));
				break;
			}
		}
	}
	else if (stmtNum != -1) {
		varNos = stmtTable.at(stmtNum).getModifies();
		for (size_t i = 0; i < varNos.size(); i++) {
			results.push_back(std::make_pair(stmtNum, varNos.at(i)));
		}
	}
	else if (varIndex != -1) {
		stmtNos = varTable.at(varIndex).getModifiedBy();
		for (size_t i = 0; i < stmtNos.size(); i++) {
			if (type1 == stmtTable.at(stmtNos.at(i)).getType() || type1 == 1) {
				results.push_back(std::make_pair(stmtNos.at(i), varIndex));
			}
		}
	}
	else {
		for (size_t i = 1; i < stmtTable.size(); i++) {
			if (type1 == stmtTable.at(i).getType() || type1 == 1 || type2 == 3) {
				varNos = stmtTable.at(i).getModifies();
				for (size_t k = 0; k < varNos.size(); k++) {
					results.push_back(std::make_pair(i, varNos.at(k)));
				}
			}
		}
	}
	return results;
}

//ZH
vector<pair<int, int>> PKB::getCalls(int procIndexFirst, int procIndexSecond) {
	vector<pair<int, int>> result;
	vector<int> call;

	if (procIndexFirst == -1) {
		if (procIndexSecond == -1) {
			// Both undefined
			for (size_t i = 0; i < procTable.size(); i++) {
				call = procTable[i].getCalls();
				for (size_t j = 0; j < call.size(); j++) {
					result.push_back(make_pair(i, call[j]));
				}
			}
		}
		else {
			// Only Second defined
			call = procTable[procIndexSecond].getCalledBy();
			for (size_t j = 0; j < call.size(); j++) {
				result.push_back(make_pair(call[j], procIndexSecond));
			}
		}
	}
	else {
		if (procIndexSecond == -1) {
			// Only first defined
			call = procTable[procIndexFirst].getCalls();
			for (size_t i = 0; i < call.size(); i++) {
				result.push_back(make_pair(procIndexFirst, call[i]));
			}
		}
		else {
			// both defined
			call = procTable[procIndexFirst].getCalls();
			for (size_t i = 0; i < call.size(); i++) {
				if (call[i] == procIndexSecond) {
					result.push_back(make_pair(procIndexFirst, procIndexSecond));
				}
			}
		}
	}
	
	return result;
}

//V
vector<pair<int, int>> PKB::getCallsT(int procIndexFirst, int procIndexSecond) {
	vector<pair<int, int>> result;
	vector<int> callsT;

	if (procIndexFirst == -1) {
		if (procIndexSecond == -1) {
			// Both undefined
			for (size_t i = 0; i < procTable.size(); i++) {
				callsT = procTable[i].getCallsT();
				for (size_t j = 0; j < callsT.size(); j++) {
					result.push_back(make_pair(i, callsT[j]));
				}
			}
		}
		else {
			// Only Second defined
			callsT = procTable[procIndexSecond].getCalledByT();
			for (size_t j = 0; j < callsT.size(); j++) {
				result.push_back(make_pair(callsT[j], procIndexSecond));
			}
		}
	}
	else {
		if (procIndexSecond == -1) {
			// Only first defined
			callsT	= procTable[procIndexFirst].getCallsT();
			for (size_t i = 0; i < callsT.size(); i++) {
				result.push_back(make_pair(procIndexFirst, callsT[i]));
			}
		}
		else {
			// both defined
			callsT = procTable[procIndexFirst].getCallsT();
			for (size_t i = 0; i < callsT.size(); i++) {
				if (callsT[i] == procIndexSecond) {
					result.push_back(make_pair(procIndexFirst, procIndexSecond));
				}
			}
		}
	}

	return result;
}

//WL
std::vector<pair<int, int>> PKB::getUses(Enum::TYPE type1, int stmtNum, Enum::TYPE type2, int varIndex)
{
	vector<int> stmtNos;
	vector<int> varNos;
	vector<int> procNos;
	vector<pair<int, int>> results;
	if (type1 == 2 && stmtNum != -1) {
		varNos = procTable.at(stmtNum).getUsed();
		if (varIndex != -1) {
			for (size_t i = 0; i < varNos.size(); i++) {
				if (varNos.at(i) == varIndex) {
					results.push_back(std::make_pair(stmtNum, varIndex));
					break;
				}
			}
		}
		else {
			for (size_t i = 0; i < varNos.size(); i++) {
				results.push_back(std::make_pair(stmtNum, varNos.at(i)));
			}
		}
	}
	else if (type1 == 2 && stmtNum == -1 && varIndex == -1) {
		for (size_t i = 1; i < procTable.size(); i++) {
				varNos = procTable.at(i).getUsed();
				for (size_t k = 0; k < varNos.size(); k++) {
					results.push_back(std::make_pair(i, varNos.at(k)));
				}
			}
	}
	else if (stmtNum != -1 && varIndex != -1) {
		varNos = stmtTable.at(stmtNum).getUses();
		for (size_t i = 0; i < varNos.size(); i++) {
			if (varNos.at(i) == varIndex) {
				results.push_back(std::make_pair(stmtNum, varIndex));
				break;
			}
		}
	}	
	else if(stmtNum != -1) {
		varNos = stmtTable.at(stmtNum).getUses();
		for (size_t i = 0; i < varNos.size(); i++) {
			results.push_back(std::make_pair(stmtNum, varNos.at(i)));
		}
	}
	else if (varIndex != -1) {
		stmtNos = varTable.at(varIndex).getUsedBy();
		for (size_t i = 0; i < stmtNos.size(); i++) {
			if (type1 == stmtTable.at(stmtNos.at(i)).getType() || type1 == 1) {
				results.push_back(std::make_pair(stmtNos.at(i), varIndex));
			}
		}
	}
	else {
		for (size_t i = 1; i < stmtTable.size(); i++) {
			if (type1 == stmtTable.at(i).getType() || type1 == 1 || type2 == 3) {
				varNos = stmtTable.at(i).getUses();
				for (size_t k = 0; k < varNos.size(); k++) {
					results.push_back(std::make_pair(i, varNos.at(k)));
				}
			}
		}
	}
	return results;
	
}

//XH
vector<pair<int, int>> PKB::getParent(Enum::TYPE type1, int stmtNum1, Enum::TYPE type2, int stmtNum2)
{
	vector<int> childrenStmtNos;
	int parentStmtNo;
	vector<pair<int, int>> results;

	if (stmtNum1 != -1) {
		childrenStmtNos = stmtTable.at(stmtNum1).getChildren();

		for (size_t i = 0; i < childrenStmtNos.size(); i++) {
			if (stmtNum2 != -1) { // Parent(2, 6)
				if (stmtNum2 == childrenStmtNos.at(i)) {
					results.push_back(std::make_pair(stmtNum1, stmtNum2));
					break;
				}
			}
			else { // Parent(2, s/w/a/_/c)
				if (childrenStmtNos.at(i) > 0) {
					if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(childrenStmtNos.at(i)).getType()) {
						results.push_back(std::make_pair(stmtNum1, childrenStmtNos.at(i)));
					}
				}
			}
		}
	}
	else if (stmtNum2 != -1) { // Parent(s/w,_ , 4)
		parentStmtNo = stmtTable.at(stmtNum2).getParent();

		if (parentStmtNo > 0) {
			if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(parentStmtNo).getType()) {
				results.push_back(std::make_pair(parentStmtNo, stmtNum2));
			}
		}
	}
	else { // Parent(s/w/_, s/w/a/_/c)
		for (size_t i = 1; i < stmtTable.size(); i++) {
			if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(i).getType()) {
				childrenStmtNos = stmtTable.at(i).getChildren();

				for (size_t j = 0; j < childrenStmtNos.size(); j++) {
					if (childrenStmtNos.at(j) > 0) {
						if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(childrenStmtNos.at(j)).getType()) {
							results.push_back(std::make_pair(i, childrenStmtNos.at(j)));
						}
					}
				}
			}
		}
	}

	return results;
}

//ZH : tested
std::vector<pair<int, int>> PKB::getFollows(Enum::TYPE type1, int stmt1, Enum::TYPE type2, int stmt2){
	vector<pair<int, int>> follows;
	size_t follow;

	if (stmt1 == -1) {
		if (stmt2 == -1){
			if (type1 == Enum::TYPE::UNDERSCORE || type1 == Enum::TYPE::STATEMENT) {
				if (type2 == Enum::TYPE::UNDERSCORE || type2 == Enum::TYPE::STATEMENT) {
					// follows(_,_) or follows(s1,s2)
					for (size_t i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getFollowedBy() > 0) {
							follows.push_back(make_pair(i, stmtTable[i].getFollowedBy()));
						}
					}
				}
				else {
					// follows(_ ,call/if/assign/while) or follows(s1, call/if/assign/while)
					for (size_t i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type2) {
							if (stmtTable[i].getFollows() > 0) {
								follows.push_back(make_pair(stmtTable[i].getFollows(), i));
							}
						}
					}
				}
			}
			else {
				if (type2 == Enum::TYPE::UNDERSCORE || type2 == Enum::TYPE::STATEMENT) {
					// follows(call/if/assign/while, _) or follows(call/if/assign/while, s1)
					for (size_t i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type1) {
							if (stmtTable[i].getFollowedBy() > 0) {
								follows.push_back(make_pair(i, stmtTable[i].getFollowedBy()));
							}
						}
					}
				}
				else {
					// follows(call/if/assign/while, call/if/assign/while)
					for (size_t i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type1) {
							if (stmtTable[i].getFollowedBy() > 0) {
								if (stmtTable[stmtTable[i].getFollowedBy()].getType() == type2) {
									follows.push_back(make_pair(i, stmtTable[i].getFollowedBy()));
								}
							}
						}
					}
				}
			}

		}
		else {
			// stmt1 is -1, stmt2 is not -1
			if (stmt2 > stmtTable.size()) {
				return follows;
			}
			follow = stmtTable[stmt2].getFollows();
			if (follow > 0) {
				if (stmtTable[follow].getType() == type1 || type1 == Enum::TYPE::STATEMENT) {
					follows.push_back(make_pair(stmtTable[stmt2].getFollows(), stmt2));
				}
			}
		}
	}
	else {
		if (stmt2 == -1) {
		// stmt 1 is not -1, stmt 2 is -1
			if (stmt1 > stmtTable.size()) {
				return follows;
			}
			follow = stmtTable[stmt1].getFollowedBy();
			if (follow > 0) {
				if (stmtTable[follow].getType() == type2 || type2 == Enum::TYPE::STATEMENT) {
					follows.push_back(make_pair(stmt1, stmtTable[stmt1].getFollowedBy()));
				}
			}
		}
		else {
			//Both parameters are defined
			if (stmt1 > stmtTable.size() || stmt2 > stmtTable.size()) {
				return follows;
			}
			if (stmtTable[stmt1].getFollowedBy() == stmt2) {
				follows.push_back(make_pair(stmt1, stmt2));
			}
		}
	}
	
	return follows;
}


//V
std::vector<pair<int, int>> PKB::getParentT(Enum::TYPE type1, int stmtNum1, Enum::TYPE type2, int stmtNum2)
{	
	vector<int>parentT;
	vector<int>children;
	vector<int>childrenT;
	vector<pair<int, int>> results;


	//(num, )
	if (stmtNum1 != -1) {
		children = stmtTable.at(stmtNum1).getChildren();
		for (size_t i = 0; i < children.size(); i++) {
			if (stmtNum2 != -1) { // ParentT(2, 6)
				if (stmtNum2 == children.at(i)) {
					results.push_back(std::make_pair(stmtNum1, stmtNum2));					
					break;
				}
			}
			else { // ParentT(2, s/w/if/a/_/c)
				//if (stmtTable.at(stmtNum1).getChildrenT().size() == 0) {
					//extractChildrenT(stmtNum1);
				//}
				childrenT = stmtTable.at(stmtNum1).getChildrenT();
				for (size_t j = 0; j < childrenT.size(); j++) {
					if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || stmtTable.at(childrenT.at(j)).getType() == type2) {
						results.push_back(std::make_pair(stmtNum1, childrenT.at(j)));
					}
				}
				}
			}
		} else if (stmtNum2 != -1) { // ParentT(s/w/if/_ , 4)
			//if (stmtTable.at(stmtNum2).getParentT().size() == 0) {
				//extractParentT(stmtNum2);
			//}
			parentT = stmtTable.at(stmtNum2).getParentT();
			for (size_t i = 0; i < parentT.size();i++) {
				if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(parentT.at(i)).getType()) {
					results.push_back(std::make_pair(parentT.at(i), stmtNum2));
				}
		}
	} else { // ParentT(s/w/_, s/w/a/_/c)
		for (size_t i = 1; i < stmtTable.size(); i++) {
			if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(i).getType()) {
				//if (stmtTable.at(i).getChildrenT().size() == 0) {
					//extractChildrenT(i);
				//}
				childrenT = stmtTable.at(i).getChildrenT();
				if (childrenT.size()== 0) {
					continue;
				}	else {
					for (size_t j = 0; j < childrenT.size(); j++) {
						if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(childrenT.at(j)).getType()) {
							results.push_back(std::make_pair(i, childrenT.at(j)));
						}
					}
				}
			}
		}
	}

	return results;
}

vector<int> PKB::getChildrenT(int stmtNum) {
	return stmtTable[stmtNum].getChildrenT();
}

vector<int> PKB::getModifiesForParser(int stmtNum)
{
	return stmtTable[stmtNum].getModifies();
}

vector<int> PKB::getUsesForParser(int stmtNum)
{
	return stmtTable[stmtNum].getUses();
}

//V
std::vector<pair<int, int>> PKB::getFollowsT(Enum::TYPE type1, int stmt1, Enum::TYPE type2, int stmt2)
{	
	vector<int> follows;
	vector<int> followsT;
	vector<int> followedByT;
	vector<pair<int, int>> results;

	if (stmt1 == -1) {
		if (stmt2 == -1) {
			if (type1 == Enum::TYPE::UNDERSCORE || type1 == Enum::TYPE::STATEMENT) {
				if (type2 == Enum::TYPE::UNDERSCORE || type2 == Enum::TYPE::STATEMENT) {
					// followsT(_,_) or followsT(s1,s2)
					for (size_t i = OFFSET; i < stmtTable.size(); i++) {
					//	if (stmtTable.at(i).getFollowedByT().size() == 0) {
						//	extractFollowedByT(i);
						//}
						followedByT = stmtTable.at(i).getFollowedByT();
						if (followedByT.size()>0) {
						for (size_t j = 0; j < followedByT.size(); j++) {
								results.push_back(make_pair(i, followedByT.at(j)));
							}
						}
					}
				}
				else {
					// follows(_ ,call/if/assign/while) or follows(s1, call/if/assign/while)
					for (size_t i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type2) {
							//if (stmtTable.at(i).getFollowsT().size() == 0) {
								//extractFollowsT(i);
							//}
							followsT = stmtTable.at(i).getFollowsT();
							if (followsT.size() > 0) {
								for (size_t j = 0; j < followsT.size(); j++) {
									results.push_back(make_pair(followsT.at(j),i));
								}
							}
						}
					}
				}
			}
			else {
				if (type2 == Enum::TYPE::UNDERSCORE || type2 == Enum::TYPE::STATEMENT) {
					// follows(call/if/assign/while, _) or follows(call/if/assign/while, s1)
					for (size_t i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type1) {
							//if (stmtTable.at(i).getFollowedByT().size() == 0) {
								//extractFollowedByT(i);
							//}
							followedByT = stmtTable.at(i).getFollowedByT();
							if (followedByT.size() > 0) {
								for (size_t j = 0; j < followedByT.size(); j++) {
									results.push_back(make_pair(i, followedByT.at(j)));
								}
							}
						}
					}
				}
				else {
					// follws(call/if/assign/while, call/if/assign/while)
					for (size_t i = OFFSET; i < stmtTable.size(); i++) {
						if (stmtTable[i].getType() == type1) {
							//if (stmtTable.at(i).getFollowedByT().size() == 0) {
								//extractFollowedByT(i);
							//}
							followedByT = stmtTable.at(i).getFollowedByT();
							if (followedByT.size() > 0) {
								for (size_t j = 0; j < followedByT.size(); j++) {
									if (stmtTable[followedByT.at(j)].getType() == type2) {
										results.push_back(make_pair(i, followedByT.at(j)));
									}
								}
							}
						}
					}
				}
			}

		}
		else {
			// stmt1 is -1, stmt2 is not -1
			//if (stmtTable.at(stmt2).getFollowsT().size() == 0) {
				//extractFollowsT(stmt2);
			//}
			followsT = stmtTable.at(stmt2).getFollowsT();
			if (followsT.size() > 0) {
				for (size_t i = 0; i < followsT.size(); i++) {
					if (stmtTable.at(followsT.at(i)).getType() == type1||(type1 == Enum::TYPE::STATEMENT)) {
						results.push_back(make_pair(followsT.at(i), stmt2));
					}
				}
			}
		}
	}

	else {
		if (stmt2 == -1) {
			// stmt 1 is not -1, stmt 2 is -1
			//if (stmtTable.at(stmt1).getFollowedByT().size() == 0) {
				//extractFollowedByT(stmt1);
			//}
			followedByT = stmtTable.at(stmt1).getFollowedByT();
			if (followedByT.size() > 0) {
				for (size_t i = 0; i < followedByT.size(); i++) {
					if(stmtTable.at(followedByT.at(i)).getType()== type2)
					results.push_back(make_pair(stmt1, followedByT.at(i)));
				}
			}
		}
		else {
			//Both parameters are defined
			//if (stmtTable.at(stmt1).getFollowedByT().size() == 0) {
				//extractFollowedByT(stmt1);
			//}
			followedByT = stmtTable.at(stmt1).getFollowedByT();
			if (followedByT.size() > 0) {
				for (size_t i = 0; i < followedByT.size(); i++) {
					if (followedByT.at(i) == stmt2) {
						results.push_back(make_pair(stmt1, stmt2));
						break;
					}
				}
			}
		}
	}
	return results;

}

vector<pair<int, int>> PKB::getNext(Enum::TYPE type1, int stmtNum1, Enum::TYPE type2, int stmtNum2)
{
	vector<int> nextStmtNos;
	vector<int> prevStmtNos;
	vector<pair<int, int>> results;

	if (stmtNum1 != -1) {
		nextStmtNos = stmtTable.at(stmtNum1).getNext();

		for (size_t i = 0; i < nextStmtNos.size(); i++) {
			if (stmtNum2 != -1) { // Next(2, 6)
				if (stmtNum2 == nextStmtNos.at(i)) {
					results.push_back(std::make_pair(stmtNum1, stmtNum2));
					break;
				}
			}
			else { // Next(2, s/w/a/_/c/if)
				if (nextStmtNos.at(i) > 0) {
					if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(nextStmtNos.at(i)).getType()) {
						results.push_back(std::make_pair(stmtNum1, nextStmtNos.at(i)));
					}
				}
			}
		}
	}
	else if (stmtNum2 != -1) { // Next(s/w,_/a/c/if, 4)
		prevStmtNos = stmtTable.at(stmtNum2).getPrev();

		for(size_t i = 0; i < prevStmtNos.size(); i++) {
			if (prevStmtNos.at(i) > 0) { 
				if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(prevStmtNos.at(i)).getType()) {
					results.push_back(std::make_pair(prevStmtNos.at(i), stmtNum2));
				}
			}
		}
	}
	else { // Next(s/w/_, s/w/a/_/c)
		for (size_t i = 1; i < stmtTable.size(); i++) {
			if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(i).getType()) {
				nextStmtNos = stmtTable.at(i).getNext();

				for (size_t j = 0; j < nextStmtNos.size(); j++) {
					if (nextStmtNos.at(j) > 0) {
						if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(nextStmtNos.at(j)).getType()) {
							results.push_back(std::make_pair(i, nextStmtNos.at(j)));
						}
					}
				}
			}
		}
	}

	return results;
}

//V
vector<pair<int, int>> PKB::getNextT(Enum::TYPE type1, int stmtNum1, Enum::TYPE type2, int stmtNum2)
{
	vector<int> nextT;
	vector<int> prevT;
	vector<pair<int, int>> results;

	if (stmtNum1 != -1) {
		nextT = extractNextT(stmtNum1);

		for (size_t i = 0; i < nextT.size(); i++) {
			if (stmtNum2 != -1) { // Next(2, 6)
				if (stmtNum2 == nextT.at(i)) {
					results.push_back(std::make_pair(stmtNum1, stmtNum2));
					break;
				}
			}
			else { // Next(2, s/w/a/_/c/if)
					if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(nextT.at(i)).getType()) {
						results.push_back(std::make_pair(stmtNum1, nextT.at(i)));
					}
			}
		}
	}
	else if (stmtNum2 != -1) { // Next(s/w,_/a/c/if, 4)
		prevT = extractPrevT(stmtNum2);

		for (size_t i = 0; i < prevT.size(); i++) {
				if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(prevT.at(i)).getType()) {
					results.push_back(std::make_pair(prevT.at(i), stmtNum2));
				}
		}
	}
	else { // Next(s/w/_, s/w/a/_/c)
		for (size_t i = 1; i < stmtTable.size(); i++) {
			if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(i).getType()) {
				nextT = extractNextT(i);

				for (size_t j = 0; j < nextT.size(); j++) {
						if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(nextT.at(j)).getType()) {
							results.push_back(std::make_pair(i, nextT.at(j)));
						}
				}
			}
		}
	}
	return results;
}

//WL
vector<int> PKB::getProcNameInVarTable(int index)
{
	return varTable.at(index).getProcNames();
}

//ZH: tested
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

//V
void PKB::extractParentT(int stmtNum)
{
	DesignExtractor design;
	vector<int> parentCol;
	vector<int> parentT;

	for (size_t i = 0; i < stmtTable.size(); i++) {
		parentCol.push_back(stmtTable.at(i).getParent());
	}
	
	
		parentT = design.extractParentT(parentCol, stmtNum);
	
	setParentT(stmtNum, parentT);
}

void PKB::extractChildrenT(int stmtNum)
{	
	DesignExtractor design;
	vector<vector<int>> childrenCol;
	vector<int> childrenT;

	for (size_t i = 0; i < stmtTable.size(); i++) {
		childrenCol.push_back(stmtTable.at(i).getChildren());
	} 
		childrenT = design.extractChildrenT(childrenCol, stmtNum);
		
	
	 setChildrenT(stmtNum, childrenT);

}

void PKB::extractFollowsT(int stmtNum)
{	
	DesignExtractor design;
	vector<int> followsCol;
	vector<int> followsT;

	for (size_t i = 0; i < stmtTable.size(); i++) {
		followsCol.push_back(stmtTable.at(i).getFollows());
	}
	
	followsT = design.extractFollowsT(followsCol, stmtNum);
	setFollowsT(stmtNum, followsT);
}

void PKB::extractFollowedByT(int stmtNum)
{	
	DesignExtractor design;
	vector<int> followedByCol;
	vector<int> followedByT;

	for (size_t i = 0; i < stmtTable.size(); i++) {
		followedByCol.push_back(stmtTable.at(i).getFollowedBy());
	}
	
		followedByT = design.extractFollowedByT(followedByCol, stmtNum);
	setFollowedByT(stmtNum, followedByT);

}

//V
void PKB::extractCallsT(int stmtNum) {
	DesignExtractor design;
	vector<vector<int>> callsCol;
	vector<int> callsT;

	for (size_t i = 0; i < procTable.size(); i++) {
		callsCol.push_back(getProcCalls(i));
	}

	
		callsT = design.extractCallsT(callsCol, stmtNum);
		
	
	setProcCallsT(stmtNum, callsT);
}

//V
void PKB::extractCalledByT(int stmtNum) {
	DesignExtractor design;
	vector<vector<int>> calledByCol;
	vector<int> calledByT;

	for (size_t i = 0; i < procTable.size(); i++) {
		calledByCol.push_back(getProcCalledBy(i));
	}
	
		calledByT = design.extractCalledByT(calledByCol, stmtNum);
		
	
	setProcCalledByT(stmtNum, calledByT);
}

//V
vector<int> PKB::extractNextT(int stmtNum) {
	DesignExtractor design;
	vector<vector<int>> nextCol;
	vector<int> nextT;

	for (size_t i = 0; i < stmtTable.size(); i++) {
		nextCol.push_back(stmtTable.at(i).getNext());
	}

	
	nextT = design.extractNextT(nextCol, stmtNum);
	return nextT;
}

//V
vector<int> PKB::extractPrevT(int stmtNum) {
	DesignExtractor design;
	vector<vector<int>> prevCol;
	vector<int> prevT;
	for (size_t i = 0; i < stmtTable.size(); i++) {
		prevCol.push_back(stmtTable.at(i).getPrev());
	}

	
		prevT = design.extractPrevT(prevCol, stmtNum);

		return prevT;
}

//V
void PKB::extractProcExtraModifiesUses() {
	DesignExtractor design;
	vector<vector<int>> calls;
	vector<vector<int>> modifiesCol;
	vector<vector<int>> usesCol;
	vector<vector<int>> updatedModifies;
	vector<vector<int>> updatedUses;

	for (int i = 0; i < procTable.size(); i++) {
		modifiesCol.push_back(getProcModified(i));
		usesCol.push_back(getProcUsed(i));
		calls.push_back(getProcCalls(i));
	}

	updatedModifies = design.extractProcModifiesUses(calls, modifiesCol);
	updatedUses = design.extractProcModifiesUses(calls, usesCol);
	for (int j = 0; j < procTable.size(); j++) {
		setProcModifies(j, updatedModifies.at(j));
		setProcUses(j, updatedUses.at(j));
	}
}

//V
vector<int> PKB::getStmtNumProcCalled(int index) {
	return procTable[index].getStmtNum();
}

vector<int> PKB::getFollowedByT(int stmtNum) {
	return stmtTable.at(stmtNum).getFollowedByT();
}


//V
int PKB::getParent(int stmtNum)
{
	return stmtTable[stmtNum].getParent();
}

//V
std::vector<int> PKB::getChildren(int stmtNum)
{
	return stmtTable[stmtNum].getChildren();
}

//V
int PKB::getFollows(int stmtNum)
{
	return stmtTable[stmtNum].getFollows();
}

//V
vector<int> PKB::getFollowsT(int stmtNum) {
	return stmtTable[stmtNum].getFollowsT();
}

//V
int PKB::getFollowedBy(int stmtNum)
{
	return stmtTable[stmtNum].getFollowedBy();
}

//ZH
int PKB::getProcIndex(string procName){

	if (procTable.size() == 0) {
		return NOT_FOUND;
	}

	for (size_t i = 0; i < procTable.size(); i++) {
		if (procTable[i].getName() == procName) {
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

vector<int> PKB::getProcModified(int procIndex)
{
	return procTable[procIndex].getModified();
}

vector<int> PKB::getProcUsed(int procIndex)
{
	return procTable[procIndex].getUsed();
}

vector<int> PKB::getProcCalls(int procIndex)
{
	return procTable[procIndex].getCalls();
}

vector<int> PKB::getProcCalledBy(int procIndex)
{
	return procTable[procIndex].getCalledBy();
}

vector<int> PKB::getCallsT(int procIndex)
{
	return procTable[procIndex].getCallsT();
}

vector<int> PKB::getCalledByT(int procIndex)
{
	return procTable[procIndex].getCalledByT();
}


//ZH : tested
int PKB::getVarIndex(string varName){

	for (size_t i = 0; i < varTable.size(); i++) {
		if (varTable[i].getVarName() == varName) {
			return i;
		}
	}

	return NOT_FOUND;
}

//ZH : tested
string PKB::getVarName(int index){
	return varTable[index].getVarName();
}

//ZH : tested
int PKB::getType(int index) {
	return stmtTable[index].getType();
}

//ZH
int PKB::getNoOfProc() {
	return procTable.size();
}

//ZH
int PKB::getNoOfVar() {
	return varTable.size();
}

int PKB::getConstantIndex(int constant)
{
	for (int i = 0; i < constantTable.size(); i++) {
		if (constantTable[i].getConstant() == constant) {
			return i;
		}
	}

	return NOT_FOUND;
}

int PKB::getConstantValue(int index) {
	if (!constantTable.empty()) {
		return constantTable[index].getConstant();
	} 

	return NOT_FOUND;
}

int PKB::getNoOfConstants() {
	return constantTable.size();
}

int PKB::getControlVar(int stmtIndex)
{
	return stmtTable[stmtIndex].getControlVarIndex();
}
