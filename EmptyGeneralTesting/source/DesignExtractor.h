#pragma once
#ifndef DESIGNEXTRACTOR_H
#define DESIGNEXTRACTOR_H
#include <vector>
#include "PKB.h"

using namespace std;

class DesignExtractor {


private:
	vector<int> ParentT;
	vector<int> ChildrenT;
	vector<int> FollowsT;
	vector<int> FollowedByT;
	void extractChildrenTRec(vector<int>, vector<vector<int>>);

public:
	DesignExtractor();
	~DesignExtractor();
	vector<int> extractParentT(vector<int>, int);
	vector<int> extractChildrenT(vector<vector<int>>, int);
	vector<int> extractFollowsT(vector<int>, int);
	vector<int> extractFollowedByT(vector<int>,int);
	//vector<int> extractCall
};

#endif