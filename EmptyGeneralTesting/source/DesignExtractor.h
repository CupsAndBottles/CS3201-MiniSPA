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
	vector<int> CallsT;
	vector<int> CalledByT;
	vector<int> NextT;
	vector<int> PrevT;
	void extractRec(vector<int>, vector<vector<int>>,string);
	void extractRecCallsTCalledByT(vector<int>, vector<vector<int>>, string);

public:
	DesignExtractor();
	~DesignExtractor();
	vector<int> extractParentT(vector<int>, int);
	vector<int> extractChildrenT(vector<vector<int>>, int);
	vector<int> extractFollowsT(vector<int>, int);
	vector<int> extractFollowedByT(vector<int>,int);
	vector<int> extractCallsT(vector<vector<int>>, int);
	vector<int> extractCalledByT(vector<vector<int>>, int);
	vector<int> extractPrevT(vector<vector<int>>, int);
	vector<int> extractNextT(vector<vector<int>>, int);
	vector<int> extractExtraProcModifiesUses(vector<int>,vector<int>,vector<vector<int>>);
};

#endif