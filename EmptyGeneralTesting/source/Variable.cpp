#include "Variable.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Variable::Variable()
{
}


Variable::~Variable()
{
}


void Variable::setVarName(string varName) {
	this->varName = varName;
}

void Variable::setProcNames(int proc) {
	procNamesList.push_back(proc);
}

void Variable::setUsedBy(int stmtLst) {
	if (stmtLst != 0) {
		usedByList.push_back(stmtLst);
	}
}

void Variable::setModifiedBy(int stmtLst) {
	if (stmtLst != 0) {
		modifiedByList.push_back(stmtLst);
	}
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

vector<int> Variable::setProcNames() {
	return this->procNamesList;
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
void Variable::insertIntoModify(int modify) {
	sortVectors(modifiedByList);
	if (modify != 0 && !binary_search(modifiedByList.begin(), modifiedByList.end(), modify)) {
		modifiedByList.push_back(modify);
	}
}

void Variable::insertIntoUses(int use) {
	sortVectors(usedByList);
	if (use != 0 && !binary_search(usedByList.begin(), usedByList.end(), use)) {
		usedByList.push_back(use);
	}
}

void Variable::insertIntoProc(int procName) {
	sortVectors(procNamesList);
	if (procName != 0 && !binary_search(procNamesList.begin(), procNamesList.end(), procName)) {
		procNamesList.push_back(procName);
	}
}