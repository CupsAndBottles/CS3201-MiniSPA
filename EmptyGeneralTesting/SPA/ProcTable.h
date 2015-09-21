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
	int insertProc(string pName, int startNum, int endNum, int modify, int use);
	int getProcIndexNo(string procName);
	string getProcName(int index);
	int getStartStmtNo(string procName);
	int getEndStmtNo(string procName);
	vector<string> getAllProcNames();
	int getNoOfProc();
	vector<int> getVarUsed(string procName);
	vector<int> getVarModified(string procName);
	void setStartStmtNo(int stmtNum, string procName);
	void setEndStmtNo(int stmtNum, string procName);
	bool isProcName(string procName);

private:
	vector<Procedure> procTable;
};

