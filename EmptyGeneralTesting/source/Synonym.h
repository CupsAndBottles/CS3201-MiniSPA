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
	bool operator <(const Synonym& s2) const;
	int resultSize();
	int getSize() const { return result.at(0).size(); }

private:
	vector<string> syn;
	vector<vector<int>> result;
	vector<Enum::TYPE> type;
	void addSyn(vector<string> syn);
	void storeResult(vector<vector<int>> resultsToStore);
	void addType(vector<Enum::TYPE> type);

};
