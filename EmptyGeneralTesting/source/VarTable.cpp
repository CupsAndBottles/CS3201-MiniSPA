#include "VarTable.h"
#include <vector>

using namespace std;


VarTable::VarTable()
{

}


VarTable::~VarTable()
{
}

int VarTable::setVarName(string varName)
{
	int index;
	if (varTable.empty()) {
		index = 0;
	}
	else {
		index = varTable.size()+1;
	}

	varTable[index].setVarName(varName);

	return index;
}

void VarTable::setProcNames(int index, int procIndex)
{
	varTable[index].setProcNames(procIndex);
}

void VarTable::setUsedBy(int index, int stmtNum)
{
	varTable[index].setUsedBy(stmtNum);
}

void VarTable::setModifiedBy(int index, int stmtNum)
{
	varTable[index].setModifiedBy(stmtNum);
}




//return -1 if does not exist
int VarTable::getIndex(string varName) {
	int index = -1;
	for (unsigned int i = 0; i < varTable.size();i++) {
		if (varTable[i].getVarName() == varName) {
			index = i;
		}
	}
	return index;
}

int VarTable::getNoOfVar() {
	return varTable.size();
}

vector<string> VarTable::getAllVar() {
	vector<string> allVar;
	for (unsigned int i = 0; i < varTable.size(); i++) {
		allVar.push_back(varTable[i].getVarName());
	}
	return allVar;
}

string VarTable::getVarName(int index) {

	return varTable[index].getVarName();
}

vector<int> VarTable::getStmtNumUsingVar(string var) {
	vector<int> stmts;
	for (unsigned int i = 0; i < varTable.size();i++) {
		if (varTable[i].getVarName() == var) {
			stmts = varTable[i].getUsedBy();
		}
	}
	return stmts;
}

vector<int> VarTable::getStmtForModified(string var) {
	vector<int> stmts;
	for (unsigned int i = 0; i < varTable.size();i++) {
		if (varTable[i].getVarName() == var) {
			stmts = varTable[i].getModifiedBy();
		}
	}
	return stmts;
}

vector<int> VarTable::getProcNames(string var) {
	vector<int> procNames;
	for (unsigned int i = 0; i < varTable.size();i++) {
		if (varTable[i].getVarName() == var) {
			procNames = varTable[i].getProcNames();
		}
	}
	return procNames;
}

bool VarTable::isVariableName(string varName) {
	bool isName = false;
	for (unsigned int i = 0; i < varTable.size();i++) {
		if (varTable[i].getVarName() == varName) {
			isName = true;
		}
	}
	return isName;
}