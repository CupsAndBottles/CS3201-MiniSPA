#include "Constant.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Constant::Constant()
{
}

Constant::~Constant()
{
}

int Constant::getConstant()
{
	return constantValue;
}

vector<int> Constant::getStmtUsed()
{
	return usedByStmt;
}

void Constant::setConstantValue(int value)
{
	constantValue = value;
}

void Constant::insertIntoStmtUsed(int use)
{
	usedByStmt.push_back(use);
}