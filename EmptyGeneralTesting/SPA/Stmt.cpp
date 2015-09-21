#include "Stmt.h"


Stmt::Stmt()
{
}
//get var index from varTable
Stmt::Stmt(int t, int p, int c, int fBy, int f, vector<int> pT, vector<int> cT, vector<int> fT, vector<int> fByT, int u, int m, string rExpr)
{
	type = t;

	if (p != 0) {
		insertParent(p);
	}
	if (c != 0) {
		insertChildList(c);
	}
	if (u != 0) {
		insertUsesList(u);
	}
	followedBy = fBy;
	follows = f;

	parentT = pT;
	childrenT = cT;
	followsT = fT;
	followedByT = fByT;

	modifies = m;
	rightExpression = rExpr;

}


Stmt::~Stmt()
{
}

void Stmt::insertChildList(int child) {
	if (child != 0) {
		childrenList.push_back(child);
	}
}
void Stmt::insertUsesList(int uses) {
	if (uses != 0) {
		usesList.push_back(uses);
	}
}

void Stmt::insertParent(int p)
{
	parent = p;
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
	return usesList;
}

int Stmt::getModifies() {
	return modifies;
}

vector<int> Stmt::getFollowsT() {
	return followsT;
}

vector<int> Stmt::getFollowedByT() {
	return followedByT;
}

string Stmt::getRightExpression() {
	return rightExpression;
}