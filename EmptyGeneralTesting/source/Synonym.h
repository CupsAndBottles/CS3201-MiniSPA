#pragma once
#include <string>
#include <set>

enum class TYPE { ASSIGN, STATEMENT, PROCEDURE, UNDERSCORE, WHILE, IF, VARIABLE, CONSTANT, CALLS };
using namespace std;

class Synonym
{
public:
	Synonym();
	Synonym(TYPE type, string syn);
	~Synonym();
	string getSyn();
	set<int> getResult();
	void addResult(int index);
	void setSyn(string syn);
	void setType(TYPE type);
	TYPE getType();

private:
	string syn;
	set<int> result;
	TYPE type;
};
