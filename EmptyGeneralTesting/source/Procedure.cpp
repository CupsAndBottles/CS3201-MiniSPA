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

vector<int> Procedure::getUsed() {
	return varUsedList;
}

vector<int> Procedure::getModified() {
	return varModifiedList;
}

void Procedure::sortVectors(vector<int> list) {
	sort(list.begin(), list.end());
}

void Procedure::setModifiedVar(vector<int> modify) {
	varModifiedList = modify;
}

void Procedure::setUsedVar(vector<int> use) {
	varUsedList = use;
}