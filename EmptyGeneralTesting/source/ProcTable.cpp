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

int ProcTable::setProcName(string pName)
{
	int index;
	if (procTable.empty()) {
		index = 0;
	}
	else {
		index = procTable.size() + 1;
	}
	procTable[index].setProcName(pName);
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

//-------------------------------setter methods-------------------------------

void ProcTable::setModify(int procIndex, vector<int> modifiedVar)
{
	procTable[procIndex].setModifiedVar(modifiedVar);
}

void ProcTable::setUses(int procIndex, vector<int> usedVar)
{
	procTable[procIndex].setUsedVar(usedVar);

}

void ProcTable::setStartStmtNo(int procIndex, int stmtNum)
{
			procTable[procIndex].setStartNo(stmtNum);
		
}

void ProcTable::setEndStmtNo(int procIndex, int stmtNum)
{
	procTable[procIndex].setEndNo(stmtNum);
		
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
