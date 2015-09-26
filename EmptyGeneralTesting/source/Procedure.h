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
	void setStartNo(int stmtNum);
	void setEndNo(int stmtNum);
	vector<int> getUsed();
	vector<int> getModified();

	void sortVectors(vector<int> list);

	void setModifiedVar(vector<int> modify);

	void setUsedVar(vector<int> use);


private:
	int index;
	string procName;
	int startStmtNum;
	int endStmtNum;
	int varModified;
	int varUsed;
	vector<int> varModifiedList;
	vector<int> varUsedList;

};

