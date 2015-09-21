#pragma once
#include "Stmt.h"
class StmtTable
{
private:
	vector<Stmt> stmtTable;
public:
	StmtTable();
	~StmtTable();
	void insertStmt(int t, int p, int c, int fBy, int f, vector<int> pT, vector<int> cT, vector<int> fT, vector<int> fByT, int u, int m, string rExpr);
	void insertChildren(int index, int child);
	void insertUses(int index, int uses);
	void insertParent(int index, int parent);
	void insertFollows(int index, int follows);
	void insertFollowedBy(int index, int followedBy);
	int getType(int index);
	int getNoOfStmts();
	int getParent(int index);
	vector<int> getChildren(int index);
	int getFollowedBy(int index);
	int getFollows(int index);
	vector<int> getParentT(int index);
	vector<int> getChildrenT(int index);
	vector<int> getFollowedByT(int index);
	vector<int> getFollowsT(int index);
	vector<int> getUses(int index);
	int getModifies(int index);
	string getRightExpression(int index);
	void setFollowsT(int index, vector<int> followsT);
	void setFollowedByT(int index, vector<int> followedByT);
	void setParentT(int index, vector<int> parentT);
	void setChildrenT(int index, vector<int> childrenT);
};

