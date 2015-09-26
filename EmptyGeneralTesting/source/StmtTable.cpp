#include "StmtTable.h"
#include "Stmt.h"

StmtTable::StmtTable()
{
}


StmtTable::~StmtTable()
{
}

void StmtTable::setStmtType(int type)
{
	int index = stmtTable.size()+1;
	stmtTable[index].setStmtType(type);
}

void StmtTable::setChildren(int index, int child) {
	stmtTable[index].setChildren(child);
}

void StmtTable::setUsedVar(int index, vector<int> usesList) {
	stmtTable[index].setUsedVar(usesList);
}

void StmtTable::setParent(int index, int parent)
{
	stmtTable[index].setParent(parent);
}

void StmtTable::setFollows(int index, int follows) {
	stmtTable[index].setFollows(follows);
}

void StmtTable::setFollowedBy(int index, int followedBy) {
	stmtTable[index].setFollowedBy(followedBy);
}

void StmtTable::setRightExpr(int index, string rightExpr)
{
	stmtTable[index].setRightExpr(rightExpr);
}

void StmtTable::setModified(int index, vector<int> modifiedVar)
{
	stmtTable[index].setModifiedVar(modifiedVar);
}

void StmtTable::setUsedConstant(int index, vector<int> usedConstant)
{
	stmtTable[index].setUsedConstant(usedConstant);
}

int StmtTable::getType(int index)
{
	int type = stmtTable[index].getType();
	return type;
}

int StmtTable::getNoOfStmts()
{
	int noOfStmts = stmtTable.size();
	return noOfStmts;
}

//negative if no parent
int StmtTable::getParent(int index)
{
	int parent = stmtTable[index].getParent();
	return parent;
}


vector<int> StmtTable::getChildren(int index)
{
	vector<int> children = stmtTable[index].getChildren();
	return children;
}

int StmtTable::getFollowedBy(int index)
{
	int followedBy = stmtTable[index].getFollowedBy();
	return followedBy;
}

int StmtTable::getFollows(int index)
{
	int followeds = stmtTable[index].getFollows();
	return followeds;
}

vector<int> StmtTable::getParentT(int index)
{
	vector<int> parentT = stmtTable[index].getParentT();;
	return parentT;
}

vector<int> StmtTable::getChildrenT(int index)
{
	vector<int> childrenT = stmtTable[index].getChildrenT();
	return childrenT;
}

vector<int> StmtTable::getFollowedByT(int index)
{
	vector<int> followedByT = stmtTable[index].getFollowedByT();
	return followedByT;
}

vector<int> StmtTable::getFollowsT(int index)
{
	vector<int> followedsT = stmtTable[index].getFollowsT();
	return followedsT;
}

vector<int> StmtTable::getUses(int index)
{
	vector<int> varUsed = stmtTable[index].getUses();
	return varUsed;
}
 vector<int> StmtTable::getModifies(int index)
{
	vector<int> varModified = stmtTable[index].getModifies();;
	return varModified;
}

string StmtTable::getRightExpression(int index)
{
	string expr = stmtTable[index].getRightExpression();
	return expr;
}

//-----------------------------setters for special type---------------------------------------

void StmtTable::setFollowsT(int index, vector<int> followsT) {
	stmtTable[index].setFollowsT(followsT);
}

void StmtTable::setFollowedByT(int index, vector<int> followedByT) {
	stmtTable[index].setFollowedByT(followedByT);
}

void StmtTable::setParentT(int index, vector<int> parentT) {
	stmtTable[index].setParentT(parentT);
}

void StmtTable::setChildrenT(int index, vector<int> childrenT) {
	stmtTable[index].setChildrenT(childrenT);
}