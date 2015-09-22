#pragma once
#include <string>
#include <set>

using namespace std;

class Synonym
{
public:
	Synonym();
	~Synonym();
	string getSyn();
	set<int> getResult();
	void addResult(int index);

private:
	string syn;
	set<int> result;

};

