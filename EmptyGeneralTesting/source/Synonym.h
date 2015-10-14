#pragma once
#include <string>
#include <set>
#include <vector>
#include "Enum.h"

using namespace std;

class Synonym
{
public:
	Synonym();
	~Synonym();
	void addResult(vector<Enum::TYPE> type, vector<string> syn, vector<vector<int>> resultsToStore);
	vector<Enum::TYPE> getType();
	vector<string> getSyn();
	vector<vector<int>> getResult();

private:
	vector<string> syn;
	vector<vector<int>> result;
	vector<Enum::TYPE> type;
	void addSyn(vector<string> syn);
	void storeResult(vector<vector<int>> resultsToStore);
	void addType(vector<Enum::TYPE> type);
};
