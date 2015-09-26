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
	vector<int> getStmtNumUsingVar(int varIndex);
	vector<int> getStmtForModified(int varIndex);
	vector<int> getProcNames(int varIndex);
	bool isVariableName(string varName);
};

