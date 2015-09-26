#pragma once
#include <string>
#include <vector>
#include "Procedure.h"

using namespace std;

class ProcTable
{
public:
	ProcTable();
	~ProcTable();
	int setProcName(string pName);
	int getProcIndexNo(string procName);
	string getProcName(int index);
	int getStartStmtNo(string procName);
	int getEndStmtNo(string procName);
	vector<string> getAllProcNames();
	int getNoOfProc();
	vector<int> getVarUsed(string procName);
	vector<int> getVarModified(string procName);
	void setModify(int procIndex, vector<int> modifiedVar);
	void setUses(int procIndex, vector<int> usedVar);
	void setStartStmtNo(int procIndex, int stmtNum);
	void setEndStmtNo(int procIndex, int stmtNum);
	bool isProcName(string procName);

private:
	vector<Procedure> procTable;
};

