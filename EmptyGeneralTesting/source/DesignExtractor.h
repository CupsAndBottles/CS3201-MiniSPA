#pragma once
#ifndef DESIGNEXTRACTOR_H
#define DESIGNEXTRACTOR_H
#include <vector>
#include "PKB.h"

class DesignExtractor {


private:
	//static PKB *pkb;
	std::vector<int> ParentT;
	std::vector<int> ChildrenT;
	std::vector<int> FollowsT;
	std::vector<int> FollowedByT;
	void extractChildrenTRec(std::vector<int>);

public:
	DesignExtractor();
	~DesignExtractor();
	std::vector<int> extractParentT(int);
	std::vector<int> extractChildrenT(int);
	std::vector<int> extractFollowsT(int);
	std::vector<int> extractFollowedByT(int);
};

#endif