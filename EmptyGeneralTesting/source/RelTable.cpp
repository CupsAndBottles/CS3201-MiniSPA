#include "RelTable.h"
#include "ParserException.h"
#include <string>
#include <vector>
#include <map>
#include <regex>

using namespace std;


RelTable::RelTable(string relName)
{
	addRelIntoTable();
	getRelationship(relName);
}


RelTable::~RelTable()
{
}

void RelTable::getRelationship(string relName)
{
	std::map<string, Relationship>::iterator relationshipRule = RelTable::relTable.find(relName);

	if (relationshipRule == relTable.end()) {
		throw ParserException("Invalid relationship");
	}
	else {
		value = relationshipRule->second;
	}

}

int RelTable::getArgNo()
{
	return value.noArgs;
}

int RelTable::getAg1VarType()
{
	return value.ag1VarType;
}

int RelTable::getAg1Synonym()
{
	return value.ag1Synonym;
}

int RelTable::getAg2VarType()
{
	return value.ag2VarType;
}

int RelTable::getAg2Synonym()
{
	return value.ag2Synonym;
}

void RelTable::addRelIntoTable()
{

	Relationship follow, parent, modifies, use, followT, parentT, useT;
	follow.noArgs = 2;
	follow.ag1Synonym = 2;
	follow.ag2Synonym = 2;
	relTable.insert(std::pair<string, Relationship>("Follows", follow));

	followT.noArgs = 2;
	followT.ag1Synonym = 2;
	followT.ag2Synonym = 2;
	relTable.insert(std::pair<string, Relationship>("Follows*", followT));


	parent.noArgs = parentT.noArgs = 2;
	parent.ag1Synonym = parentT.ag1Synonym = 0;
	parent.ag2Synonym = parentT.ag2Synonym = 1;
	relTable.insert(std::pair<string, Relationship>("Parent", parent));
	relTable.insert(std::pair<string, Relationship>("Parent*", parentT));

	modifies.noArgs = 2;
	modifies.ag1Synonym = 3;
	modifies.ag2Synonym = 4;
	relTable.insert(std::pair<string, Relationship>("Modifies", modifies));

	use.noArgs = 2;
	use.ag1Synonym = 3;
	use.ag2Synonym = 4;
	relTable.insert(std::pair<string, Relationship>("Uses", use));


}


