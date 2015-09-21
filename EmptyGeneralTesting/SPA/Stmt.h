#pragma once
#include <vector>
using namespace std;
class Stmt
{
	//add followsT, parentT, etc
public:
	Stmt();
	Stmt(int t, int p, int c, int fBy, int f, vector<int> pT, vector<int> cT, vector<int> fT, vector<int> fByT, int u, int m, string rExpr);
	~Stmt();

	void insertChildList(int child);

	void insertUsesList(int uses);

	void insertParent(int p);

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

	int getModifies();

	vector<int> getFollowsT();

	vector<int> getFollowedByT();

	string getRightExpression();

	int getFollows();

	int getFollowedBy();

private:
	int index;
	int type;
	int parent;
	vector<int> parentT;
	vector<int> childrenList;
	int children;
	vector<int> childrenT;
	int followedBy;
	int follows;
	vector<int> followedByT;
	vector<int> followsT;
	vector<int> usesList;
	int uses;
	int modifies;
	string rightExpression;
};

