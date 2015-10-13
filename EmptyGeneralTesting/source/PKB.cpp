#pragma once
#include <stddef.h> //defines null
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

void PKB::setProcModified(int index, vector<string> modifiedVar)
{
	vector<int> modifiedVarIndex;
	while (!modifiedVar.empty()) {
		int i = getVarIndex(modifiedVar.back());
		/*cout << "------------------------------------\n";
		cout << "proc index: " << index << "\n";
		cout << "index: " << i << "\n";
		cout << "modified var: "<<modifiedVar.back()<<"\n";*/
		modifiedVar.pop_back();
		modifiedVarIndex.push_back(i);
	}
	procTable[index].setModifiedVar(modifiedVarIndex);

}

void PKB::setProcUses(int index, vector<string> usesVar)
{
	vector<int> usesVarIndex;
	while (!usesVar.empty()) {
		int i = getVarIndex(usesVar.back());
	/*	cout << "------------------------------------\n";
		cout << "proc index: " << index << "\n";
		cout << "index: " << i << "\n";
		cout << "uses var: " << usesVar.back() << "\n";*/
		usesVar.pop_back();
		usesVarIndex.push_back(i);
	}
	procTable[index].setUsedVar(usesVarIndex);
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
		//cout << "set varName: " << varName << "\n";
		//cout << "set varIndex: " << index << "\n";
	}
	return index;
}

//G: get proc index from procTable and set in varTable
void PKB::setProcNames(int index, string procName)
{
	int procIndex = getProcIndex(procName);
	varTable[index].insertIntoProc(procIndex);

}


void PKB::setUsedBy(string varName, int stmtNum)
{
	int varIndex = getVarIndex(varName);
	if (varIndex != -1) {
		varTable[varIndex].insertIntoUses(stmtNum);
	}
	else {
		setVarName(varName);
		setUsedBy(varName, stmtNum);
	}
}

void PKB::setModifiedBy(string varName, int stmtNum)
{
	int varIndex = getVarIndex(varName);
	if (varIndex != -1) {
	varTable[varIndex].insertIntoModify(stmtNum);
	}
	else {
		setVarName(varName);
		setModifiedBy(varName,stmtNum);
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

int PKB::setConstant(int constantValue)
{
	int index = getConstantIndex(constantValue);
	if (index == -1) {
		constantTable.push_back(Constant());
		int size = constantTable.size() - OFFSET;
		constantTable[size].setConstantValue(constantValue);
		index = getConstantIndex(constantValue);
	}
	return index;
}

void PKB::setStmtUsed(int index, int stmtNum)
{
	constantTable[index].insertIntoStmtUsed(stmtNum);	
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
		//cout << "second: " << secondStmt << "\n";
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
	//cout << "Index : " << varIndex << "\n";
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
//PKB-DesignExtractor, called at start a
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
		for (size_t i = 1; i < procTable.size(); i++) {
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
std::vector<pair<int, int>> PKB::getParent(Enum::TYPE type1, int stmtNum1, Enum::TYPE type2, int stmtNum2)
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
				if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(childrenStmtNos.at(i)).getType()) {
					results.push_back(std::make_pair(stmtNum1, childrenStmtNos.at(i)));
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
					if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || type2 == stmtTable.at(childrenStmtNos.at(j)).getType()) {
						results.push_back(std::make_pair(i, childrenStmtNos.at(j)));
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
				if (stmtTable[follow].getType() == type1) {
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
				if (stmtTable[follow].getType() == type2) {
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
				if (stmtTable.at(stmtNum1).getChildrenT().size() == 0) {
					extractChildrenT(stmtNum1);
				}
				childrenT = stmtTable.at(stmtNum1).getChildrenT();
				for (size_t j = 0; j < childrenT.size(); j++) {
					if (type2 == Enum::TYPE::STATEMENT || type2 == Enum::TYPE::UNDERSCORE || stmtTable.at(childrenT.at(j)).getType() == type2) {
						results.push_back(std::make_pair(stmtNum1, childrenT.at(j)));
					}
				}
				}
			}
		} else if (stmtNum2 != -1) { // ParentT(s/w/if/_ , 4)
			if (stmtTable.at(stmtNum2).getParentT().size() == 0) {
				extractParentT(stmtNum2);
			}
			parentT = stmtTable.at(stmtNum2).getParentT();
			for (size_t i = 0; i < parentT.size();i++) {
				if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(parentT.at(i)).getType()) {
					results.push_back(std::make_pair(parentT.at(i), stmtNum2));
				}
		}
	} else { // ParentT(s/w/_, s/w/a/_/c)
		for (size_t i = 1; i < stmtTable.size(); i++) {
			if (type1 == Enum::TYPE::STATEMENT || type1 == Enum::TYPE::UNDERSCORE || type1 == stmtTable.at(i).getType()) {
				if (stmtTable.at(i).getChildrenT().size() == 0) {
					extractChildrenT(i);
				}
				childrenT = stmtTable.at(i).getChildrenT();
				if (childrenT.at(0) == 0) {
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
						if (stmtTable.at(i).getFollowedByT().size() == 0) {
							extractFollowedByT(i);
						}
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
							if (stmtTable.at(i).getFollowsT().size() == 0) {
								extractFollowsT(i);
							}
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
							if (stmtTable.at(i).getFollowedByT().size() == 0) {
								extractFollowedByT(i);
							}
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
							if (stmtTable.at(i).getFollowedByT().size() == 0) {
								extractFollowedByT(i);
							}
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
			if (stmtTable.at(stmt2).getFollowsT().size() == 0) {
				extractFollowsT(stmt2);
			}
			followsT = stmtTable.at(stmt2).getFollowsT();
			if (followsT.size() > 0) {
				for (size_t i = 0; i < followsT.size(); i++) {
					if (stmtTable.at(followsT.at(i)).getType() == type1) {
						results.push_back(make_pair(followsT.at(i), stmt2));
					}
				}
			}
		}
	}

	else {
		if (stmt2 == -1) {
			// stmt 1 is not -1, stmt 2 is -1
			if (stmtTable.at(stmt1).getFollowedByT().size() == 0) {
				extractFollowedByT(stmt1);
			}
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
			if (stmtTable.at(stmt1).getFollowedByT().size() == 0) {
				extractFollowedByT(stmt1);
			}
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
	
	if (parentCol.size() == 0) {
		parentT.push_back(0);
	} else {
		parentT = design.extractParentT(parentCol, stmtNum);
	}
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

	if (childrenCol.size() == 0) {
		childrenT.push_back(0);
	} else {
		childrenT = design.extractChildrenT(childrenCol, stmtNum);
	}
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

	if (followsCol.size() == 0) {
		followsT.push_back(0);
	} else {
		followsT = design.extractFollowsT(followsCol, stmtNum);
	}
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

	if (followedByCol.size() == 0) {
		followedByT.push_back(0);
	} else {
		followedByT = design.extractFollowedByT(followedByCol, stmtNum);
	}
	setFollowedByT(stmtNum, followedByT);

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
	return vector<int>();
}

vector<int> PKB::getProcUsed(int procIndex)
{
	return vector<int>();
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
int PKB::getControlVar(int stmtIndex)
{
	return stmtTable[stmtIndex].getControlVarIndex();
}
