#include "Stmt.h"


Stmt::Stmt()
{
}

Stmt::~Stmt()
{
}

void Stmt::setStmtType(int t)
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