#pragma once
#include "DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor()
{
}

DesignExtractor::~DesignExtractor() {
}

std::vector<int> DesignExtractor::extractParentT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	vector<int> parentCol = col;
	int parent;

	while (true) {
		parent = col.at(stmtNum);
		if (parent == -1) {
			break;
		}
		else {
			ParentT.push_back(parent);
			stmtNum = parent;
		}
	}
	return ParentT;
}

std::vector<int> DesignExtractor::extractChildrenT(vector<vector<int>> col,int stmtNum) {
	std::vector<int> children = col.at(stmtNum);
	int child;

	child = children.at(0);
	if (child == -1) {
		ChildrenT.push_back(-1);
	}
	else {
		extractChildrenTRec(children,col);
	}

	return ChildrenT;
}

void DesignExtractor::extractChildrenTRec(std::vector<int> children, vector<vector<int>> col) {

	for (int i = 0; i < (int)children.size(); i++) {
		int child = children.at(i);
		if (child == -1) {
			return;
		}
		else {
			ChildrenT.push_back(child);
			extractChildrenTRec(col.at(child), col);
		}
	}
}

std::vector<int> DesignExtractor::extractFollowsT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	int num;

	while (true) {
		num = col.at(stmtNum);
		if (num == -1) {
			break;
		}
		else {
			FollowsT.push_back(num);
			stmtNum = num;
		}
	}
	return FollowsT;
}

std::vector<int> DesignExtractor::extractFollowedByT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	int num;

	while (true) {
		num = col.at(stmtNum);
		if (num == -1) {
			break;
		}
		else {
			FollowedByT.push_back(num);
			stmtNum = num;
		}
	}

	return FollowedByT;
}
