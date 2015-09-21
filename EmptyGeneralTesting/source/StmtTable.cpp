#include "StmtTable.h"
#include "Stmt.h"

StmtTable::StmtTable()
{
}


StmtTable::~StmtTable()
{
}

void StmtTable::insertStmt(int t, int p, int c, int fBy, int f, vector<int> pT, vector<int> cT, vector<int> fT, vector<int> fByT, int u, int m, string rExpr)
{
	if (!stmtTable.empty()) {
		vector<int> empty;
		stmtTable.push_back((Stmt(0, 0, 0, 0, 0, empty, empty, empty, empty, 0, 0, "")));
	}
	stmtTable.push_back(Stmt(t, p, c, fBy, f, pT, cT, fT, fByT, u, m, rExpr));
}

void StmtTable::insertChildren(int index, int child) {
	stmtTable[index].insertChildList(child);
}

void StmtTable::insertUses(int index, int uses) {
	stmtTable[index].insertUsesList(uses);
}

void StmtTable::insertParent(int index, int parent)
{
	stmtTable[index].insertParent(parent);
}

void StmtTable::insertFollows(int index, int follows) {
	stmtTable[index].setFollows(follows);
}

void StmtTable::insertFollowedBy(int index, int followedBy) {
	stmtTable[index].setFollowedBy(followedBy);
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

int StmtTable::getModifies(int index)
{
	int varModified = stmtTable[index].getModifies();;
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