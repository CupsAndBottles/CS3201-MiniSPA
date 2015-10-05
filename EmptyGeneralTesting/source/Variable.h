#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Variable
{
public:
	Variable(string varName,int procIndex, int uStmtLst, int mStmtLst);

	~Variable();
	int getindex();
	string getVarName();
	vector<int> getModifiedBy();
	vector<int> getUsedBy();
	vector<int> getProcNames();

	void insertIntoModify(int modify);
	void insertIntoUses(int use);
	void insertIntoProc(int procName);
	void setVarName(string varName);
	void sortVectors(vector<int> list);

private:
	int index;
	string varName;
	vector<int> procNamesList;
	vector<int> usedByList;
	vector<int> modifiedByList;

};

