#include "Procedure.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;




Procedure::Procedure()
{
}

Procedure::~Procedure()
{
}

void Procedure::setProcName(string pName)
{
	procName = pName;
}

void Procedure::setStartNo(int stmtNum)
{
	if (stmtNum != 0) {
		startStmtNum = stmtNum;
	}
}

void Procedure::setEndNo(int stmtNum)
{
	if (stmtNum != 0) {
		endStmtNum = stmtNum;
	}
}

void Procedure::setModifiedVar(int modify) {
	//cout << "Procname: " << procName << "\n";
	//cout << "Modified: " << modify << "\n";
	varModifiedList.push_back(modify);

}

void Procedure::setUsedVar(int use) {
	varUsedList.push_back(use);
}

void Procedure::setCalls(int procIndex)
{	
	callsList.push_back(procIndex);
}

void Procedure::setCalledBy(int procIndex)
{
	calledByList.push_back(procIndex);
}

void Procedure::setCallsT(vector<int> procList)
{
	callsTList = procList;
}

void Procedure::setCalledByT(vector<int> procList)
{
	calledByTList = procList;
}

void Procedure::setStmtNum(int stmtNum)
{
	stmtsProcCalled.push_back(stmtNum);
}

vector<int> Procedure::getStmtNum()
{
	return stmtsProcCalled;
}

//V
void Procedure::setUpdatedModifies(vector<int> modifies)
{
	varModifiedList = modifies;
}

//V
void Procedure::setUpdatedUses(vector<int> uses)
{
	varUsedList = uses;
}

vector<int> Procedure::getUsed() {
	return varUsedList;
}

vector<int> Procedure::getModified() {
	return varModifiedList;
}
string Procedure::getName() {
	return procName;
}

int Procedure::getIndexNum(string procNum) {
	return index;
}

int Procedure::getStartNo() {
	return startStmtNum;
}

int Procedure::getEndNo() {
	return endStmtNum;
}

vector<int> Procedure::getCalls()
{
	return callsList;
}

vector<int> Procedure::getCalledBy()
{
	return calledByList;
}

vector<int> Procedure::getCallsT()
{
	return callsTList;
}

vector<int> Procedure::getCalledByT()
{
	return calledByTList;
}


void Procedure::sortVectors(vector<int> list) {
	sort(list.begin(), list.end());
}