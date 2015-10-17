#pragma once
#include <vector>
#include <string>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Procedure
{
public:
	Procedure();
	~Procedure();
	void setProcName(string pName);
	string getName();
	int getIndexNum(string procNum);
	int getStartNo();
	int getEndNo();
	vector<int> getCalls();
	vector<int> getCalledBy();
	vector<int> getCallsT();
	vector<int> getCalledByT();
	void setStartNo(int stmtNum);
	void setEndNo(int stmtNum);
	vector<int> getUsed();
	vector<int> getModified();
	void sortVectors(vector<int> list);
	void setModifiedVar(vector<int> modify);
	void setUsedVar(vector<int> use);
	void setCalls(int procIndex);
	void setCalledBy(int procIndex);
	void setCallsT(vector<int> procList);
	void setCalledByT(vector<int> procList);
	void setStmtNum(int stmtNum);
	vector<int> getStmtNum();
	
	void setUpdatedModifies(vector<int> modifies);
	void setUpdatedUses(vector<int> uses);

private:
	int index;
	string procName;
	int startStmtNum;
	int endStmtNum;
	int varModified;
	int varUsed;
	vector<int> varModifiedList;
	vector<int> varUsedList;
	vector<int> callsList;
	vector<int> calledByList;
	vector<int> callsTList;
	vector<int> calledByTList;
	vector<int> stmtsProcCalled;
};
