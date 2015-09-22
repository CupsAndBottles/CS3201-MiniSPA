#include "ParserForPQL.h"
#include "Validation.h"
#include <regex>
#include <string>
#include <iostream>
#include "ParserException.h"
#include "ParserForSelect.h"
#include "ParserKeyword.h"
#include "ParserForSuchThat.h"
#include "ParserForPattern.h"
#include "ParserForWith.h"
#include "ParserTypeWithSyn.h"

using namespace std;

ParserForPQL::ParserForPQL(string input)
{
	parse(input);
}

ParserForPQL::~ParserForPQL()
{
}

void ParserForPQL::parse(string input)
{
	if (input.compare("") == 0) {
		throw ParserException("empty query");
	}

	getPosition(input, 0);
	getSyn(input, 0);

	getPosition(input, 1);
	getSyn(input, 1);

	validateType();
	parseKeyword();
	parseRespectively();
	startValidate();

	QueryTree queryTree = queryTree.return_created_Object();

//	queryTree.startPlotting(selectSynAndType, suchThatSynAndType, patternSynAndType);

	//setQueryTree(queryTree);
}

void ParserForPQL::setQueryTree(QueryTree queryTree)
{
	this->queryTree = queryTree;
}

QueryTree ParserForPQL::getQueryTree()
{
	return queryTree;
}


//pre-coniditions: 1st stat must be declaration, last stat query and Select 1st word of query
void ParserForPQL::getPosition(string input, int typeNo)
{
	type.push_back(vector <string>());
	position.push_back(vector <int>());
	std::smatch match;

	string matchClause;
	int j = 0; int z = 0;

	for (std::sregex_iterator i = std::sregex_iterator(input.begin(), input.end(), regexForBoth.at(typeNo));
	i != std::sregex_iterator(); ++i) {
		std::smatch match = *i;
		j = addNumOfPos(j, match, typeNo);
		type[typeNo].push_back(match[0]);
		//	std::cout << "type = " << type[typeNo].at(z++) << '\n';

	}

	addInFinalSyn(typeNo, input);
	numberOfPos.push_back(j + 1);
	//std::cout << "number = " << numberOfPos[typeNo] << '\n';
}

int ParserForPQL::addNumOfPos(int number, smatch match, int typeNo)
{
	//std::cout << "j = " << number << '\n';
	if (number == 0) {
		position[typeNo].push_back(match.position() + match.length());
		number++;
	}
	else {
		position[typeNo].push_back(match.position());
		position[typeNo].push_back(match.position() + match.length());
		number = number + 2;

	}
	return number;
}

void ParserForPQL::addInFinalSyn(int typeNo, string input)
{
	if (typeNo == 0) {
		position[typeNo].push_back(input.length());
	}
	else {
		position[typeNo].push_back(position[0].at(0) - 7);
	}
}

void ParserForPQL::getSyn(string input, int typeNo)
{
	synonym.push_back(vector <string>());
	int k = 0;
	for (int i = 0; i < numberOfPos[typeNo]; i += 2) {

		if (i + 1 >= numberOfPos[typeNo]) {
			break;
		}
		//	std::cout << "position = " << position[typeNo].at(i) << '\n';
		//std::cout << "position1 = " << position[typeNo].at(i + 1) << '\n';
		synonym[typeNo].push_back(input.substr(position[typeNo].at(i), position[typeNo].at(i + 1)
			- position[typeNo].at(i)));
		//std::cout << "synonym = " << synonym[typeNo].at(k++) << '\n';
	}

}

void ParserForPQL::parseTypeWithSyn(vector<string> selectSynonym, vector<vector<string>> suchThatSynonym,
	vector<vector<string>> withSynonym,
	vector<vector<string>> patternSynonym)
{
	ParserTypeWithSyn parserTypeWithSyn(selectSynonym, suchThatSynonym,
		withSynonym, patternSynonym, type[1], synonym[1]);
	selectSynAndType = parserTypeWithSyn.getSelectSynAndType();
	suchThatSynAndType = parserTypeWithSyn.getSuchThatSynAndType();
	patternSynAndType = parserTypeWithSyn.getPatternSynAndType();
}

void ParserForPQL::validateType()
{
	if (type[0].size() != synonym[0].size() || type[1].size() != synonym[1].size()) {
		//"Invalid input: declaration is not complete"
		throw ParserException("Invalid input: declaration is not complete");
	}
}

void ParserForPQL::parseRespectively()
{
	vector<vector<string> >::const_iterator row = type.begin();
	vector<string>::const_iterator col;
	int numberOfSuchThat = 0, numberOfWith = 0, numberOfPattern = 0;
	vector<vector<string>> suchThatSynonym;
	vector<vector<string>> withSynonym;
	vector<vector<string>> patternSynonym;
	vector<string> selectSynonym;

	for (col = row->begin(); col != row->end(); ++col) {
		string oneType = *col;
		size_t index = std::distance(row->begin(), col);
		if (oneType.compare("Select") == 0) {
			ParserForSelect parserForSelect(type, synonym, index);
			selectSynonym = parserForSelect.getSelectSynonym();
		}
		else if (oneType.compare("such that") == 0 || oneType.compare("and") == 0) {
			ParserForSuchThat parserForSuchThat(type, synonym, index);
			suchThatSynonym.push_back(parserForSuchThat.getSuchThatSynonym());
		}
		else if (oneType.compare("with") == 0) {
			ParserForWith parserForWith(type, synonym, index);
			withSynonym.push_back(parserForWith.getWithSynonym());
		}
		else if (oneType.compare("pattern") == 0) {
			ParserForPattern parserForPattern(type, synonym, index);
			patternSynonym.push_back(parserForPattern.getPatternSynonym());
		}
	}

	parseTypeWithSyn(selectSynonym, suchThatSynonym, withSynonym, patternSynonym);

}

void ParserForPQL::parseKeyword()
{
	ParserKeyword parserKeyword(type, synonym);
	synonym[1] = parserKeyword.getKeywordSynonym();
	type[1] = parserKeyword.getKeywordType();

}

void ParserForPQL::startValidate()
{
	Validation validation(suchThatSynAndType);
}


