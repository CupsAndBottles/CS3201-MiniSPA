#include "ProcTable.h"
#include <string>
#include <vector>

using namespace std;

ProcTable::ProcTable()
{
}

ProcTable::~ProcTable()
{
}

//insert new procedure with index number
int ProcTable::insertProc(string pName, int startNum, int endNum, int modify, int use)
{
	int index = getProcIndexNo(pName);
	if (index == -1) {
		procTable.push_back(Procedure(pName, startNum, endNum, modify, use));
		index = procTable.size() + 1;
	}
	else {
		for (unsigned int i = 0; i < procTable.size();i++) {
			if (procTable[i].getName() == pName) {
				procTable[i].insertIntoModify(modify);
				procTable[i].insertIntoUses(use);

			}
		}
	}
	return index;
}

//-----------------------getter methods---------------------
int ProcTable::getProcIndexNo(string procName)
{
	int index = -1;
	for (unsigned int i = 0; i < procTable.size();i++) {
		if (procTable[i].getName() == procName) {
			index = i;
		}
	}
	return index;
}


string ProcTable::getProcName(int index)
{
	string name = procTable[index].getName();
	return name;
}


int ProcTable::getStartStmtNo(string procName)
{
	int firstStmtNo;
	for (unsigned int i = 0; i < procTable.size();i++) {
		if (procTable[i].getName() == procName) {
			firstStmtNo = procTable[i].getStartNo();
		}
	}
	return firstStmtNo;
}

int ProcTable::getEndStmtNo(string procName)
{
	int endStmtNo;
	for (unsigned int i = 0; i < procTable.size();i++) {
		if (procTable[i].getName() == procName) {
			endStmtNo = procTable[i].getEndNo();
		}
	}
	return endStmtNo;
}

vector<string> ProcTable::getAllProcNames()
{
	vector<string> allProc;
	for (unsigned int i = 0; i < procTable.size(); i++) {
		allProc.push_back(procTable[i].getName());
	}
	return allProc;
}

int ProcTable::getNoOfProc()
{
	int size = procTable.size();
	return size;
}

vector<int> ProcTable::getVarUsed(string procName)
{
	vector<int> varUsed;
	for (unsigned int i = 0; i < procTable.size();i++) {
		if (procTable[i].getName() == procName) {
			varUsed = procTable[i].getUsed();
		}
	}
	return varUsed;
}

vector<int> ProcTable::getVarModified(string procName)
{
	vector<int> varModified;
	for (unsigned int i = 0; i < procTable.size();i++) {
		if (procTable[i].getName() == procName) {
			varModified = procTable[i].getModified();
		}
	}
	return varModified;
}

void ProcTable::setStartStmtNo(int stmtNum, string procName)
{
	for (unsigned int i = 0; i < procTable.size();i++) {
		if (procTable[i].getName() == procName) {
			procTable[i].setStartNo(stmtNum);
		}
	}
}

void ProcTable::setEndStmtNo(int stmtNum, string procName)
{
	for (unsigned int i = 0; i < procTable.size();i++) {
		if (procTable[i].getName() == procName) {
			procTable[i].setEndNo(stmtNum);
		}
	}
}

bool ProcTable::isProcName(string procName) {
	bool isName = false;
	for (unsigned int i = 0; i < procTable.size();i++) {
		if (procTable[i].getName() == procName) {
			isName = true;
		}
	}
	return isName;
}
