#pragma once
#include <vector>
#include "Enum.h"
using namespace std;
class Stmt
{
	//add followsT, parentT, etc
public:
	Stmt();
	~Stmt();
	
	void setStmtType(Enum::TYPE t);
	void setParent(int p);
	void setChildren(int child);
	void setUsedVar(int usedVar);
	void setUsedConstant(vector<int> usedConstant);
	void setModifiedVar(int modifiedVar);
	void setRightExpr(string rightExpr);
	void setFollows(int followStmt);
	void setFollowedBy(int followedByStmt);
	void setFollowsT(vector<int> followsTList);
	void setFollowedByT(vector<int> followedByTList);
	void setParentT(vector<int> parentTList);
	void setChildrenT(vector<int> childrenTList);

	void setNext(vector<int> nList);

	void setNextT(vector<int> nTList);

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

	vector<int> getNext();

	vector<int> getNextT();

	int getFollows();

	int getFollowedBy();

private:
	int index;
	Enum::TYPE type;
	int parent=0;
	vector<int> parentT;
	vector<int> childrenList;
	vector<int> childrenT;
	vector<int> usedConstantList;
	int followedBy=0;
	int follows=0;
	vector<int> followedByT;
	vector<int> followsT;
	vector<int> usedVarList;
	vector<int> modifiedVarList;
	vector<int> nextList;
	vector<int> nextTList;
	string rightExpression;
};

