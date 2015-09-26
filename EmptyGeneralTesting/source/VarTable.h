#pragma once
#include "Variable.h"

class VarTable
{
private:
	vector<Variable> varTable;
public:
	VarTable();
	~VarTable();
	int setVarName(string varName);
	void setProcNames(int index, int procIndex);
	void setUsedBy(int index, int stmtNum);
	void setModifiedBy(int index, int stmtNum);
	int getIndex(string varName);
	int getNoOfVar();
	vector<string> getAllVar();
	string getVarName(int index);
	vector<int> getStmtNumUsingVar(string var);
	vector<int> getStmtForModified(string var);
	vector<int> getProcNames(string var);
	bool isVariableName(string varName);
};

