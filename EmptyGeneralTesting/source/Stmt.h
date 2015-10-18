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
	void sortVectors(vector<int> list);
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
	void setControlVar(int varIndex);
	void setCallsStmtModifiesVar(vector<int> modifies);
	void setCallsStmtUsesVar(vector<int> uses);

	void setNext(int next);
	void setPrev(int prev);

	void setPrevT(vector<int> prevT);
	void setNextT(vector<int> nextT);

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

	vector<int> getPrev();

	vector<int> getNextT();
	vector<int> getPrevT();
	int getControlVarIndex();
	int getFollows();

	int getFollowedBy();

private:
	int index;
	Enum::TYPE type;
	int parent=0;
	int controlVar=0;
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
	vector<int> prevList;
	vector<int> prevTList;
	string rightExpression;
};

