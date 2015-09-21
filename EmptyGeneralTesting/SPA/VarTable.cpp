#include "VarTable.h"
#include <vector>

using namespace std;


VarTable::VarTable()
{

}


VarTable::~VarTable()
{
}
int VarTable::insertVar(int index, string VarName, int procNames, int usedBy, int modifiedBy) {
	index = getIndex(VarName);
	if (index == -1) {
		varTable.push_back(Variable(VarName, procNames, usedBy, modifiedBy));
		index = varTable.size() + 1;
	}
	else {
		for (unsigned int i = 0; i < varTable.size();i++) {
			if (varTable[i].getVarName() == VarName) {
				varTable[i].insertIntoModify(procNames);
				varTable[i].insertIntoUses(usedBy);
				varTable[i].insertIntoProc(modifiedBy);
			}
		}

	}
	return index;
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