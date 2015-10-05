#include "ParserOfType.h"
#include "ParserException.h"
#include "PKB.h"
#include "ParserForPQL.h"
#include <iostream>
#include <regex>
#include<vector>
#include<string>

using namespace std;

ParserOfType::ParserOfType()
{
}


ParserOfType::~ParserOfType()
{
}

vector<vector<string>> ParserOfType::setType(PKB &querypkb, int clauseType, string synonym, vector<string> type, vector<string> synonymType)
{
	this->pkb = &querypkb; 
	vector<vector<string>> synAndType;
	int index;
	synAndType.push_back(vector <string>()); //stringVal
	synAndType.push_back(vector <string>()); //type
	synAndType.push_back(vector <string>()); //intVal
	synAndType.push_back(vector <string>()); //isExpression

	synonym = removeOpenComma(synonym);
	string isSubExpression = checkSubExpression(synonym);
	synonym = removeUnwanted(synonym);

	if (isSynDigit(synonym) && clauseType == 1) {
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("prog_line");
		synAndType[2].push_back(synonym);
		synAndType[3].push_back(isSubExpression);
		return synAndType;
	}
	else if (isSynDigit(synonym) && clauseType == 2) {
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("constant");
		synAndType[2].push_back(synonym);
		synAndType[3].push_back(isSubExpression);
		return synAndType;
	}
	else if (isBeingDeclared(synonym, synonymType) != -1) {
		int pos = isBeingDeclared(synonym, synonymType);
		synAndType[0].push_back(synonym);
		synAndType[1].push_back(type.at(pos));
		synAndType[2].push_back("-1");
		synAndType[3].push_back(isSubExpression);
		return synAndType;
	}
	else if (synonym.compare("_") == 0) {
		synAndType[0].push_back("_");
		synAndType[1].push_back("_");
		synAndType[2].push_back("-1");
		synAndType[3].push_back(isSubExpression);
		return synAndType;
	}
	else if (isExpression(synonym)) {
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("equation");
		synAndType[2].push_back("-1");
		synAndType[3].push_back(isSubExpression);
		return synAndType;
	}
	else if (isVariable(synonym)) {
		index = pkb->getVarIndex(synonym);
		synAndType[2].push_back(std::to_string(index));
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("variable");
		synAndType[3].push_back(isSubExpression);
		return synAndType;
	}
	else if (isProcedure(synonym)) {
		index = pkb->getProcIndex(synonym);
		synAndType[2].push_back(std::to_string(index));
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("procedure");
	//	synAndType[2].push_back("-1");
		synAndType[3].push_back(isSubExpression);
		return synAndType;
	}


	return vector<vector<string>>();
}


// 0 -> select 1--> such that 2--> pattern 3--> with


vector<vector<string>> ParserOfType::setClauseType(int clauseType, string synonym, vector<string> type, vector<string> synonymType)
{
	vector<vector<string>> synAndType;
	synAndType.push_back(vector <string>()); //stringVal
	synAndType.push_back(vector <string>()); //type
	synAndType.push_back(vector <string>()); //intVal
	synAndType.push_back(vector <string>()); //isExpression
	if (clauseType == 0 || clauseType == 2) {
		int typeDeclared = isBeingDeclared(synonym, synonymType);
		if (typeDeclared != -1) {

			synAndType[0].push_back(synonym);
			synAndType[1].push_back(type.at(typeDeclared));
			synAndType[2].push_back("-1");
			synAndType[3].push_back("0");
	
			return synAndType;
		}
		else if (synonym.compare("BOOLEAN") == 0) {
			synAndType[0].push_back(synonym);
			synAndType[1].push_back("BOOLEAN");
			synAndType[2].push_back("-1");
			synAndType[3].push_back("0");
			return synAndType;
		}
	}
	else if (clauseType == 1) {
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("");
		synAndType[2].push_back("-1");
		synAndType[3].push_back("0");
		return synAndType;
	}

	return vector<vector<string>>();

}

bool ParserOfType::isSynDigit(string syn)
{
	bool ifDigit = true;
	int start = 0, end = syn.length();
	for (std::size_t i = start; i < end; i++) {
		if (!isdigit(syn[i])) {
			//		std::cout << "ifDigit = " << syn[i] << '\n';
			ifDigit = false;
			break;
		}
	}
	return ifDigit;
}

int ParserOfType::isBeingDeclared(string syn, vector<string> synType)
{
	for (std::size_t j = 0; j < synType.size(); j++) {
		if (syn.compare(synType.at(j)) == 0) {
			std::cout << "syn = " << syn << '\n';
			return j;
		}
	}

	return -1;
}

string ParserOfType::checkSubExpression(string arg2) {
	if (arg2.at(0) == ('_') && arg2.at(arg2.length() - 1) == ('_') && arg2.length() > 2) {
		return "1";
	}
	else if (arg2.at(0) == ('_') && arg2.at(arg2.length() - 1) != ('_') && arg2.length() > 2) {
		throw ParserException("Pattern arguments of subexpression must be _..._");
	}
	else if (arg2.at(0) != ('_') && arg2.at(arg2.length() - 1) == ('_') && arg2.length() > 2) {
		throw ParserException("Pattern arguments of subexpression must be _..._");
	}
	else if (arg2.at(0) == ('_') && arg2.at(arg2.length() - 1) == ('_') && arg2.length() == 2) {
		throw ParserException("Pattern arguments of subexpression must be _..._");
	}
	else {
		return "0";
	}
}

string ParserOfType::removeUnwanted(string syn)
{
	while (syn.length() > 1 && ((syn[0] == '\"' && syn[syn.length() - 1] == '\"') || (syn[0] == '_' && syn[syn.length() - 1] == '_'))) {
		syn = syn.substr(1, syn.length() - 2);
	}
	return syn;
}

string ParserOfType::removeOpenComma(string syn)
{
	if (syn.length() > 1 && ((syn[0] == '\"' && syn[syn.length() - 1] == '\"'))) {
		return syn.substr(1, syn.length() - 2);
	}
	return syn;
}

bool ParserOfType::isVariable(string syn)
{
	int index = pkb->getVarIndex(syn);
	if (index == -1) {
		return false;
	}
	return true;
}

bool ParserOfType::isProcedure(string syn)
{
	std::locale loc;
	int index = pkb->getProcIndex(syn);
		if (index == -1) {
			//std::cout << "syn[i] in = " << syn[i] << '\n';
			return false;
	}
	return true;
}

bool ParserOfType::isExpression(std::string syn)
{
	std::size_t found = syn.find("+");
	std::size_t found1 = syn.find("-");
	std::size_t found2 = syn.find("*");

	if (found == std::string::npos && found1 == std::string::npos && found2 == std::string::npos) {
		return false;
	}
	return true;
}


