#pragma once
#include <vector>
using namespace std;
class Stmt
{
	//add followsT, parentT, etc
public:
	Stmt();
	~Stmt();

	void setStmtType(int t);
	void setParent(int p);
	void setChildren(int child);
	void setUsedVar(vector<int> usedList);
	void setUsedConstant(vector<int> usedConstant);
	void setModifiedVar(vector<int> modifiedList);
	void setRightExpr(string rightExpr);
	void setFollows(int followStmt);
	void setFollowedBy(int followedByStmt);
	void setFollowsT(vector<int> followsTList);
	void setFollowedByT(vector<int> followedByTList);
	void setParentT(vector<int> parentTList);
	void setChildrenT(vector<int> childrenTList);

	int getType();
	int getParent();

	vector<int> getChildren();
	vector<int> getChildrenT();
	vector<int> getParentT();
	vector<int> getUses();

	vector<int> getModifies();

	vector<int> getFollowsT();

	vector<int> getFollowedByT();

	vector<int> setUsedConstant();

	string getRightExpression();

	int getFollows();

	int getFollowedBy();

private:
	int index;
	int type;
	int parent;
	vector<int> parentT;
	vector<int> childrenList;
	vector<int> childrenT;
	vector<int> usedConstantList;
	int followedBy;
	int follows;
	vector<int> followedByT;
	vector<int> followsT;
	vector<int> usedVarList;
	vector<int> modifiedVarList;
	string rightExpression;
};

