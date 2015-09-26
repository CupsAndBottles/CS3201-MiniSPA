#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor() {

	//pkb = PKB::getInstanceOf();
}

DesignExtractor::~DesignExtractor() {
}

std::vector<int> DesignExtractor::extractParentT(int stmtNum) {
	int index = stmtNum;
	int parent;

	while (true) {
		parent = PKB::getInstanceOf()->getParent(stmtNum);
		if (parent == 1) {
			break;
		}
		else {
			ParentT.push_back(parent);
			stmtNum = parent;
		}
	}
	PKB::getInstanceOf()->setParentT(index, ParentT);

	return ParentT;
}

std::vector<int> DesignExtractor::extractChildrenT(int stmtNum) {
	std::vector<int> children = PKB::getInstanceOf()->getChildren(stmtNum);
	int child;

	child = children.at(0);
	if (child == -1) {
		ChildrenT.push_back(-1);
	}
	else {
		extractChildrenTRec(children);
	}
	PKB::getInstanceOf()->setChildrenT(stmtNum, ChildrenT);
	return ChildrenT;
}

void DesignExtractor::extractChildrenTRec(std::vector<int> children) {

	for (int i = 0; i < (int)children.size(); i++) {
		int child = children.at(i);
		if (child == -1) {
			return;
		}
		else {
			ChildrenT.push_back(child);
			extractChildrenTRec(PKB::getInstanceOf()->getChildren(child));
		}
	}
}

std::vector<int> DesignExtractor::extractFollowsT(int stmtNum) {
	int index = stmtNum;
	int num;

	while (true) {
		num = PKB::getInstanceOf()->getFollows(stmtNum);
		if (num != -1) {
			break;
		}
		else {
			FollowsT.push_back(num);
			stmtNum = num;
		}
	}
	PKB::getInstanceOf()->setFollowsT(index, FollowsT);
	return FollowsT;
}

std::vector<int> DesignExtractor::extractFollowedByT(int stmtNum) {
	int index = stmtNum;
	int num;

	while (true) {
		num = PKB::getInstanceOf()->getFollowedBy(stmtNum);
		if (num == -1) {
			break;
		}
		else {
			FollowedByT.push_back(num);
			stmtNum = num;
		}
	}
	PKB::getInstanceOf()->setFollowedByT(index, FollowedByT);
	return FollowedByT;
}
