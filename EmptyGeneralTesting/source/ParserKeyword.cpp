#include "ParserKeyword.h"
#include "ParserException.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

ParserKeyword::ParserKeyword(vector<vector<string> > type, vector<vector<string> > synonym)
{
	parseKeyword(type, synonym);

}


ParserKeyword::~ParserKeyword()
{
}


void ParserKeyword::parseKeyword(vector<vector<string>> type, vector<vector<string>> synonym)
{
	vector<string> commaSubset;
	for (std::size_t i = 0; i < synonym[1].size(); i++) {
		std::size_t found = synonym[1].at(i).find(",");
		if (found != std::string::npos) {
			vector<string> temp = split(synonym[1].at(i), ',');
			commaSubset.insert(commaSubset.end(), temp.begin(), temp.end());
			addType(temp, type, i);
		}
		else {
			commaSubset.push_back(synonym[1].at(i));
			keywordType.push_back(type[1].at(i));
		}
	}
	keywordSynonym = removeUnwanted(commaSubset);
}

vector<string> ParserKeyword::split(const std::string & s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

vector<string>& ParserKeyword::split(const string & s, char delim, vector<string>& elems)
{
	stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> ParserKeyword::removeUnwanted(vector<string> commaSubset)
{
	std::regex toBeRemove("<|>|\\s+|;|\\s");
	for (size_t i = 0; i < commaSubset.size(); i++) {
		commaSubset.at(i) = std::regex_replace(commaSubset.at(i), toBeRemove, "");
		if (commaSubset.at(i).compare("") == 0) {
			throw ParserException("Input less declaration");
		}
		//	std::cout << "keywordParser = " << commaSubset.at(i) << '\n';
	}
	return commaSubset;
}

void ParserKeyword::addType(vector<string> temp, vector<vector<string>> type, int i)
{
	for (size_t j = 0; j < temp.size(); j++) {
		keywordType.push_back(type[1].at(i));
	}
}

vector<string> ParserKeyword::getKeywordSynonym()
{
	return keywordSynonym;
}

vector<string> ParserKeyword::getKeywordType()
{
	return keywordType;
}



