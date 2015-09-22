#pragma once
#pragma once
#include <string>
#include <vector>
#include <map>
#include <regex>

using namespace std;

class RelTable
{

	struct Relationship {
		std::string relName;
		int noArgs;
		int ag1VarType;
		int ag2VarType;
		int ag1Synonym;
		int ag2Synonym;
	};
public:
	RelTable(string relName);
	~RelTable();
	Relationship value;
	std::map<std::string, Relationship> relTable;

	void getRelationship(std::string relName);

	int getArgNo();

	int getAg1VarType();

	int getAg1Synonym();

	int getAg2VarType();

	int getAg2Synonym();

	void addRelIntoTable();
};
