#include "Procedure.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


Procedure::Procedure(string pName, int startNum, int endNum, int modify, int use)
{
	procName = pName;
	if (startNum != 0) {
		startStmtNum = startNum;
	}
	if (endNum != 0) {
		endStmtNum = endNum;
	}
	varModified = modify;
	varUsed = use;
}

Procedure::Procedure()
{
}

Procedure::~Procedure()
{
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

void Procedure::insertIntoModify(int modify) {
	sortVectors(varModifiedList);
	if (modify != 0 && modify != -1 && !binary_search(varModifiedList.begin(), varModifiedList.end(), modify)) {
		varModifiedList.push_back(modify);
	}
}

void Procedure::insertIntoUses(int use) {
	sortVectors(varModifiedList);
	if (use != 0 && use != -1 && !binary_search(varModifiedList.begin(), varModifiedList.end(), use)) {
		varModifiedList.push_back(use);
	}
}