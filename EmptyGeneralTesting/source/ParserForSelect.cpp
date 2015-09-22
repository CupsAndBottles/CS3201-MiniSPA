#include "ParserForSelect.h"
#include "ParserException.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

ParserForSelect::ParserForSelect(vector<vector<string> > type, vector<vector<string> > synonym, int index)
{
	parseSelect(type, synonym, index);

}


ParserForSelect::~ParserForSelect()
{
}


void ParserForSelect::parseSelect(vector<vector<string>> type, vector<vector<string>> synonym, int pos)
{
	vector<string> commaSubset;
	std::size_t found = synonym[0].at(pos).find(",");
	if (found != std::string::npos) {
		commaSubset = split(synonym[0].at(pos), ',');
	}
	else {
		commaSubset.push_back(synonym[0].at(pos));
	}
	selectSynonym = removeUnwanted(commaSubset);
}

vector<string> ParserForSelect::split(const std::string & s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

vector<string>& ParserForSelect::split(const string & s, char delim, vector<string>& elems)
{
	stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> ParserForSelect::removeUnwanted(vector<string> commaSubset)
{
	std::regex toBeRemove("<|>|\\s+|;|\\s");
	for (int i = 0; i < commaSubset.size(); i++) {
		commaSubset.at(i) = (std::regex_replace(commaSubset.at(i), toBeRemove, ""));
		//	std::cout << "commasubset = " << commaSubset.at(i) << '\n';
		if (commaSubset.at(i).compare("") == 0) {
			throw ParserException("Type synonym for Select");
		}
	}
	return commaSubset;
}

vector<string> ParserForSelect::getSelectSynonym()
{
	return selectSynonym;
}





