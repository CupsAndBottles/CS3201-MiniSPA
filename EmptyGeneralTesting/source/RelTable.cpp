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
	//std::cout << "relname = " << relName << '\n';
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

	Relationship follow, parent, modifies, use, followT, parentT, useT, While, If, assign, call, next, affect, procName, varName, stmt, value, constant, stringOfWith, stmt1, withWhile, withIf, withAssign;

	call.noArgs = 2;
	call.ag1Synonym = 8;
	call.ag2Synonym = 8;
	relTable.insert(std::pair<string, Relationship>("calls", call));
	relTable.insert(std::pair<string, Relationship>("calls*", call));

	next.noArgs = 2;
	next.ag1Synonym = 2;
	next.ag2Synonym = 2;
	relTable.insert(std::pair<string, Relationship>("next", next));
	relTable.insert(std::pair<string, Relationship>("next*", next));

	affect.noArgs = 2;
	affect.ag1Synonym = 9;
	affect.ag2Synonym = 9;
	relTable.insert(std::pair<string, Relationship>("affects", affect));
	relTable.insert(std::pair<string, Relationship>("affects*", affect));
	
	follow.noArgs = 2;
	follow.ag1Synonym = 2;
	follow.ag2Synonym = 2;
	relTable.insert(std::pair<string, Relationship>("follows", follow));

	followT.noArgs = 2;
	followT.ag1Synonym = 2;
	followT.ag2Synonym = 2;
	relTable.insert(std::pair<string, Relationship>("follows*", followT));


	parent.noArgs = parentT.noArgs = 2;
	parent.ag1Synonym = parentT.ag1Synonym = 0;
	parent.ag2Synonym = parentT.ag2Synonym = 1;
	relTable.insert(std::pair<string, Relationship>("parent", parent));
	relTable.insert(std::pair<string, Relationship>("parent*", parentT));

	modifies.noArgs = 2;
	modifies.ag1Synonym = 3;
	modifies.ag2Synonym = 4;
	relTable.insert(std::pair<string, Relationship>("modifies", modifies));

	use.noArgs = 2;
	use.ag1Synonym = 3;
	use.ag2Synonym = 4;
	relTable.insert(std::pair<string, Relationship>("uses", use));

	assign.noArgs = 2;
	assign.ag1Synonym = 4;
	assign.ag2Synonym = 7;
	relTable.insert(std::pair<string, Relationship>("Patternassign", assign));

	While.noArgs = 2;
	While.ag1Synonym = 5;
	While.ag2Synonym = 6;
	relTable.insert(std::pair<string, Relationship>("Patternwhile", While));

	If.noArgs = 3;
	If.ag1Synonym = 5;
	If.ag2Synonym = 6;
	relTable.insert(std::pair<string, Relationship>("Patternif", If));

	stmt.ag1Synonym = 10;
	relTable.insert(std::pair<string, Relationship>("Withstmt#", stmt));

	procName.ag1Synonym = 11;
	relTable.insert(std::pair<string, Relationship>("WithprocName", procName));

	varName.ag1Synonym = 5;
	relTable.insert(std::pair<string, Relationship>("WithvarName", varName));

	value.ag1Synonym = 12;
	relTable.insert(std::pair<string, Relationship>("Withvalue", value));

	constant.ag1Synonym = 13;
	relTable.insert(std::pair<string, Relationship>("Withconstant", constant));

	stringOfWith.ag1Synonym = 14;
	relTable.insert(std::pair<string, Relationship>("Withprocedure", stringOfWith));
	relTable.insert(std::pair<string, Relationship>("Withvariable", stringOfWith));

	withAssign.ag1Synonym = 15;
	relTable.insert(std::pair<string, Relationship>("Withassign", withAssign));

	stmt1.ag1Synonym = 16;
	relTable.insert(std::pair<string, Relationship>("Withstmt", stmt1));

	withWhile.ag1Synonym = 17;
	relTable.insert(std::pair<string, Relationship>("Withwhile", withWhile));

	withIf.ag1Synonym = 18;
	relTable.insert(std::pair<string, Relationship>("Withif", withIf));

}


