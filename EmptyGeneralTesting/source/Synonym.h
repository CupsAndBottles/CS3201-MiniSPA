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
	Synonym(Enum::TYPE type, string syn, vector<int> resultsToStore);
	~Synonym();
	void addResult(vector<int> resultToBeAdded);
	string getSyn();
	vector<int> getResult();
	void setSyn(string syn);
	void setType(Enum::TYPE type);
	Enum::TYPE getType();

private:
	string syn;
	vector<int> result;
	Enum::TYPE type;
};
