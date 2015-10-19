#include "Stmt.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

Stmt::Stmt()
{
}


Stmt::~Stmt()
{
}

void Stmt::setStmtType(Enum::TYPE t)
{
	type = t;
}

void Stmt::setParent(int p)
{
	parent = p;

}

void Stmt::setChildren(int child)
{
	childrenList.push_back(child);

}

void Stmt::sortVectors(vector<int> list) {
	sort(list.begin(), list.end());
}


void Stmt::setUsedVar(int usedVar)
{
	sortVectors(usedVarList);
	if (!binary_search(usedVarList.begin(), usedVarList.end(), usedVar)) {
		usedVarList.push_back(usedVar);
	}
	usedVarList.erase(unique(usedVarList.begin(), usedVarList.end()), usedVarList.end());

}

void Stmt::setUsedConstant(vector<int> usedConstant)
{
	usedConstantList = usedConstant;
}

void Stmt::setModifiedVar(int modifiedList)
{
	sortVectors(modifiedVarList);
	if (!binary_search(modifiedVarList.begin(), modifiedVarList.end(), modifiedList)) {
		modifiedVarList.push_back(modifiedList);
	}
	modifiedVarList.erase(unique(modifiedVarList.begin(), modifiedVarList.end()), modifiedVarList.end());

}

void Stmt::setRightExpr(string rightExpr)
{
	rightExpression = rightExpr;
}


void Stmt::setFollows(int followStmt)
{
	follows = followStmt;
}

void Stmt::setFollowedBy(int followedByStmt)
{
	followedBy = followedByStmt;
}

//-------------------------------------------
void Stmt::setFollowsT(vector<int> followsTList) {
	followsT = followsTList;
}

void Stmt::setFollowedByT(vector<int> followedByTList) {
	followedByT = followedByTList;
}

void Stmt::setParentT(vector<int> parentTList) {
	parentT = parentTList;
}

void Stmt::setChildrenT(vector<int> childrenTList) {
	childrenT = childrenTList;
}
void Stmt::setControlVar(int varIndex)
{
	controlVar = varIndex;
}

//V
void Stmt::setCallsStmtModifiesVar(vector<int> modifies)
{
	modifiedVarList = modifies;
}

//V
void Stmt::setCallsStmtUsesVar(vector<int> uses) {
	usedVarList = uses;
}

//V
void Stmt::setModifies(vector<int> var) {
	modifiedVarList = var;
}

//V
void Stmt::setUses(vector<int> var) {
	usedVarList = var;
}

//---------------set next and next*----------------
void Stmt::setNext(int next) {
	nextList.push_back(next);
}
void Stmt::setPrev(int prev) {
	prevList.push_back(prev);
}

void Stmt::setPrevT(vector<int> prevT) {
	prevTList = prevT;
}
void Stmt::setNextT(vector<int> nextT) {
	nextTList = nextT;
}
//---------------------------------------------------
int Stmt::getType() {
	return type;
}

int Stmt::getParent() {
	return parent;
}

vector<int> Stmt::getChildren() {
	return childrenList;
}

vector<int> Stmt::getChildrenT()
{
	return childrenT;
}

vector<int> Stmt::getParentT()
{
	return parentT;
}

int Stmt::getFollows() {
	return follows;
}

int Stmt::getFollowedBy() {
	return followedBy;
}


vector<int> Stmt::getUses() {
	return usedVarList;
}

vector<int> Stmt::getModifies() {
	return modifiedVarList;
}

vector<int> Stmt::getFollowsT() {
	return followsT;
}

vector<int> Stmt::getFollowedByT() {
	return followedByT;
}

vector<int> Stmt::setUsedConstant() {
	return usedConstantList;
}

string Stmt::getRightExpression() {
	return rightExpression;
}

//---------------get next and next*----------------
vector<int> Stmt::getNext() {
	return nextList;
}

vector<int> Stmt::getPrev() {
	return prevList;
}

vector<int> Stmt::getNextT() {
	return nextTList;
}

vector<int> Stmt::getPrevT() {
	return prevTList;
}


int Stmt::getControlVarIndex()
{
	return controlVar;
}