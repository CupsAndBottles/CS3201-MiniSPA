#pragma once
#include <string>
#include <set>
#include <vector>

enum class TYPE { ASSIGN, STATEMENT, PROCEDURE, UNDERSCORE, WHILE, IF, VARIABLE, CONSTANT, CALLS };
using namespace std;

class Synonym
{
public:
	Synonym();
	Synonym(TYPE type, string syn, vector<int>& resultsToStore);
	~Synonym();
	void addResult(vector<int>& resultToBeAdded);
	string getSyn();
	vector<int> getResult();
	void setSyn(string syn);
	void setType(TYPE type);
	TYPE getType();

private:
	string syn;
	//set<int> result;
	vector<int> result;
	TYPE type;
};
