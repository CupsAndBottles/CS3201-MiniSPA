#include "Stmt.h"
#include <iostream>
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

void Stmt::setUsedVar(int usedVar)
{
	usedVarList.push_back(usedVar);
}

void Stmt::setUsedConstant(vector<int> usedConstant)
{
	usedConstantList = usedConstant;
}

void Stmt::setModifiedVar(int modifiedList)
{
	modifiedVarList.push_back(modifiedList);
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