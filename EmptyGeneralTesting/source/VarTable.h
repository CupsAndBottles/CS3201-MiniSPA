#pragma once
#include "Variable.h"

class VarTable
{
private:
	vector<Variable> varTable;
public:
	VarTable();
	~VarTable();
	int insertVar(int index, string VarName, int procNames, int usedBy, int modifiedBy);
	int getIndex(string varName);
	int getNoOfVar();
	vector<string> getAllVar();
	string getVarName(int index);
	vector<int> getStmtNumUsingVar(string var);
	vector<int> getStmtForModified(string var);
	vector<int> getProcNames(string var);
	bool isVariableName(string varName);
};

