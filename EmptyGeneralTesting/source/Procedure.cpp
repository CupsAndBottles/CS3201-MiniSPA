#include "Procedure.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;




Procedure::Procedure()
{
}

Procedure::~Procedure()
{
}

void Procedure::setProcName(string pName)
{
	procName = pName;
}

void Procedure::setStartNo(int stmtNum)
{
	if (stmtNum != 0) {
		startStmtNum = stmtNum;
	}
}

void Procedure::setEndNo(int stmtNum)
{
	if (stmtNum != 0) {
		endStmtNum = stmtNum;
	}
}

void Procedure::setModifiedVar(vector<int> modify) {
	varModifiedList = modify;
}

void Procedure::setUsedVar(vector<int> use) {
	varUsedList = use;
}

void Procedure::setCalls(int procIndex)
{
	callsList.push_back(procIndex);
}

void Procedure::setCalledBy(int procIndex)
{
	calledByList.push_back(procIndex);
}

vector<int> Procedure::getUsed() {
	return varUsedList;
}

vector<int> Procedure::getModified() {
	return varModifiedList;
}
string Procedure::getName() {
	return procName;
}

int Procedure::getIndexNum(string procNum) {
	return index;
}

int Procedure::getStartNo() {
	return startStmtNum;
}

int Procedure::getEndNo() {
	return endStmtNum;
}

vector<int> Procedure::getCalls(int procIndex)
{
	return callsList;
}

vector<int> Procedure::getCalledBy(int procIndex)
{
	return calledByList;
}


void Procedure::sortVectors(vector<int> list) {
	sort(list.begin(), list.end());
}
