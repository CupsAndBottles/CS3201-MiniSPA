#include "Variable.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Variable::Variable(string varName, int procIndex, int uStmtLst, int mStmtLst)
{
	setVarName(varName);
	insertIntoProc(procIndex);
	insertIntoUses(uStmtLst);
	insertIntoModify(mStmtLst);
}

Variable::~Variable()
{
}

void Variable::setVarName(string varName) {
	this->varName = varName;
	//cout << "Varname : " << varName << "\n";
}

void Variable::sortVectors(vector<int> list) {
	sort(list.begin(), list.end());
}

int Variable::getindex() {
	return this->index;
}

string Variable::getVarName() {
	return this->varName;
}


vector<int> Variable::getUsedBy() {
	return this->usedByList;
}

vector<int> Variable::getModifiedBy() {
	return this->modifiedByList;
}

vector<int> Variable::getProcNames() {
	return this->procNamesList;
}

//Bug: weird numbers added. 
void Variable::insertIntoModify(int modify) {
	sortVectors(modifiedByList);
	if (modify != 0 && !binary_search(modifiedByList.begin(), modifiedByList.end(), modify)) {
		modifiedByList.push_back(modify);
	}
	cout << "Varname : " << varName << "\n";
	cout << "Modified: " << modify << "\n";
}

//Bug: weird numbers added. 
void Variable::insertIntoUses(int use) {
	sortVectors(usedByList);
	if (use != 0 && !binary_search(usedByList.begin(), usedByList.end(), use)) {
		usedByList.push_back(use);
	}
//	cout << "Varname : " << varName << "\n";
//	cout << "Used: " << use << "\n";
}

void Variable::insertIntoProc(int procName) {
	sortVectors(procNamesList);
//	cout << "Varname : " << varName << "\n";
//	cout << "Procname: "<<procName << "\n";
	if (procName != 0 && !binary_search(procNamesList.begin(), procNamesList.end(), procName)) {
		procNamesList.push_back(procName);
	}
}