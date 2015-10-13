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

vector<vector<string>> ParserOfType::setType(PKB &querypkb, int clauseType, string synonym, vector<string> type, vector<string> synonymType, string indication)
{
	this->pkb = &querypkb; 
	vector<vector<string>> synAndType;
	int index;
	synAndType.push_back(vector <string>()); //stringVal
	synAndType.push_back(vector <string>()); //type
	synAndType.push_back(vector <string>()); //intVal
	synAndType.push_back(vector <string>()); //isExpression

	std::size_t found = synonym.find("\"");
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
		index = pkb->getConstantIndex(atoi(synonym.c_str()));
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("constant");
		synAndType[2].push_back(std::to_string(index));
		synAndType[3].push_back(isSubExpression);
	
		return synAndType;
	}
	else if (found == std::string::npos && isBeingDeclared(synonym, synonymType) != -1) {
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
	else if (isVariable(synonym) && isProcedure(synonym)) {
		if (indication.compare("variable") == 0) {
			index = pkb->getVarIndex(synonym);
			synAndType[2].push_back(std::to_string(index));
			synAndType[0].push_back(synonym);
			synAndType[1].push_back("variable");
			synAndType[3].push_back(isSubExpression);
			return synAndType;
		}
		else if (indication.compare("procedure") == 0) {
			index = pkb->getProcIndex(synonym);
			synAndType[2].push_back(std::to_string(index));
			synAndType[0].push_back(synonym);
			synAndType[1].push_back("procedure");
			synAndType[3].push_back(isSubExpression);
			return synAndType;
		}
		else {
			index = pkb->getProcIndex(synonym);
			synAndType[2].push_back(std::to_string(index));
			synAndType[0].push_back(synonym);
			synAndType[1].push_back("procedure");
			synAndType[3].push_back(isSubExpression);
			return synAndType;
		}
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
		synAndType[3].push_back(isSubExpression);
		return synAndType;
	}
	return vector<vector<string>>();
}

// 0 -> select 1--> such that 2--> pattern 3--> wit
vector<vector<string>> ParserOfType::setClauseType(int clauseType, string synonym, vector<string> type, vector<string> synonymType)
{
	vector<vector<string>> synAndType;
	synAndType.push_back(vector <string>()); //stringVal
	synAndType.push_back(vector <string>()); //type
	synAndType.push_back(vector <string>()); //intVal
	synAndType.push_back(vector <string>()); //isExpression
	if (clauseType == 0 || clauseType == 2 || clauseType == 3) {
		int typeDeclared = isBeingDeclared(synonym, synonymType);
	//	std::cout << "typeDeclared = " << typeDeclared << '\n';
	//	std::cout << "synonym = " << synonym << '\n';
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
			std::cout << "hello" << '\n';
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

//0 -> progLine, 1 -> constant
vector<vector<string>> ParserOfType::setDigitTypeAndSyn(string clauseType, string synonym) {
	vector<vector<string>> synAndType;

	synAndType.push_back(vector <string>()); //stringVal
	synAndType.push_back(vector <string>()); //type
	synAndType.push_back(vector <string>()); //intVal
	synAndType.push_back(vector <string>()); //isExpression

	std::cout << "clauseType = " << clauseType << '\n';
	if (isSynDigit(synonym) && (clauseType.compare("stmt#") == 0 || clauseType.compare("prog_line") == 0)) {
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("prog_line");
		synAndType[2].push_back(synonym);
		synAndType[3].push_back("1");
		return synAndType;
	}
	else if (isSynDigit(synonym) && clauseType.compare("value") == 0) {
		int index = pkb->getConstantIndex(atoi(synonym.c_str()));
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("constant");
		synAndType[2].push_back(std::to_string(index));
		synAndType[3].push_back("0");
		return synAndType;
	}
	else {
		throw ParserException("2nd argument in with clauses should be integer since 1st argument take in an integer");
	}
	return vector<vector<string>>();
}

vector<vector<string>> ParserOfType::setVariableTypeAndSyn(PKB &querypkb, string synonym) {
	this->pkb = &querypkb;
	vector<vector<string>> synAndType;
	int index;
	synAndType.push_back(vector <string>()); //stringVal
	synAndType.push_back(vector <string>()); //type
	synAndType.push_back(vector <string>()); //intVal
	synAndType.push_back(vector <string>()); //isExpression
	std::size_t found = synonym.find("\"");
	synonym = removeOpenComma(synonym);
	
	if (found != std::string::npos && isVariable(synonym)) {
		int index = pkb->getVarIndex(synonym);
		synAndType[2].push_back(std::to_string(index));
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("variable");
		synAndType[3].push_back("0");
		return synAndType;
	}
	else {
		throw ParserException("\"var\" in with clauses unidentified");
	}
	return vector<vector<string>>();
}

vector<vector<string>> ParserOfType::setProcedureTypeAndSyn(PKB &querypkb, string synonym, string clauseType) {
	this->pkb = &querypkb;
	vector<vector<string>> synAndType;
	int index;
	synAndType.push_back(vector <string>()); //stringVal
	synAndType.push_back(vector <string>()); //type
	synAndType.push_back(vector <string>()); //intVal
	synAndType.push_back(vector <string>()); //isExpression
	std::size_t found = synonym.find("\"");
	synonym = removeOpenComma(synonym);
	
	if (found != std::string::npos && isProcedure(synonym)) {
		int index = pkb->getProcIndex(synonym);
		synAndType[2].push_back(std::to_string(index));
		synAndType[0].push_back(synonym);
		synAndType[1].push_back("procedure");
		synAndType[3].push_back("0");
		return synAndType;
	}
	else {
		throw ParserException("\"proc\" in with clauses unidentified");
	}
	return vector<vector<string>>();
}

vector<vector<string>> ParserOfType::setDeclaredSynAndType(string synonym, vector<string> type, vector<string> synonymType) {
	int typeDeclared = isBeingDeclared(synonym, synonymType);
	vector<vector<string>> synAndType;
	synAndType.push_back(vector <string>()); //stringVal
	synAndType.push_back(vector <string>()); //type
	synAndType.push_back(vector <string>()); //intVal
	synAndType.push_back(vector <string>()); //isExpression

	if (typeDeclared != -1) {

		synAndType[0].push_back(synonym);
		synAndType[1].push_back(type.at(typeDeclared));
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
	int index = pkb->getProcIndex(syn);

		if (index == -1) {
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


