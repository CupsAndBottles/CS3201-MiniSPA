#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Constant
{
public:
	Constant();

	~Constant();
	int getConstant();
	vector<int> getStmtUsed();

	void setConstantValue(int value);
	void insertIntoStmtUsed(int use);

private:
	int constantValue;
	vector<int> usedByStmt;

};

