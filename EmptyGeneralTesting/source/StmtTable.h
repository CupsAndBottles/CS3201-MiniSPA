#pragma once
#include "Stmt.h"
class StmtTable
{
private:
	vector<Stmt> stmtTable;
public:
	StmtTable();
	~StmtTable();
	void setStmtType(int index,int type);
	void setChildren(int index, vector<int> children);
	void setUsedVar(int index, vector<int> uses);
	void setParent(int index, int parent);
	void setFollows(int index, int follows);
	void setFollowedBy(int index, int followedBy);
	void setRightExpr(int index, string rightExpr);
	void setModified(int index, vector<int> modifiedVar);
	void setUsedConstant(int index, vector<int> usedConstant);

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
	vector<int> getModifies(int index);
	string getRightExpression(int index);
	void setFollowsT(int index, vector<int> followsT);
	void setFollowedByT(int index, vector<int> followedByT);
	void setParentT(int index, vector<int> parentT);
	void setChildrenT(int index, vector<int> childrenT);
};

