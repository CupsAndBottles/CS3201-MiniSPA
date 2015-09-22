#include "ParserForWith.h"
#include "ParserException.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>

using namespace std;


ParserForWith::ParserForWith(vector<vector<string> > type, vector<vector<string> > synonym, int index)
{
	parseWith(type, synonym, index);
}


ParserForWith::~ParserForWith()
{
}

void ParserForWith::parseWith(vector<vector<string>> type, vector<vector<string>> synonym, int pos)
{
	vector<string> equalSubset;
	std::size_t foundFullStop = synonym[0].at(pos).find(".");
	if (foundFullStop != std::string::npos) {
		equalSubset = split(synonym[0].at(pos), '.');
		withSynonym.push_back(equalSubset.at(0));
		std::size_t found = equalSubset.at(1).find("=");
		if (found != std::string::npos) {
			equalSubset = split(equalSubset.at(1), '=');
			withSynonym.insert(withSynonym.end(), equalSubset.begin(), equalSubset.end());
		}
		else {
			throw ParserException("Wrong synax for with");
		}
	}
	else {
		std::size_t found = synonym[0].at(pos).find("=");
		if (found != std::string::npos) {
			equalSubset = split(synonym[0].at(pos), '=');
			withSynonym = equalSubset;
		}
		else {
			throw ParserException("Wrong synax for with");
		}
	}
	withSynonym = removeUnwanted(withSynonym);
}

vector<string> ParserForWith::removeUnwanted(vector<string> withSynonym)
{
	std::regex toBeRemove("<|>|\\s+|;|\\s");
	for (int i = 0; i < withSynonym.size(); i++) {
		withSynonym.at(i) = std::regex_replace(withSynonym.at(i), toBeRemove, "");
		if (withSynonym.at(i).compare("") == 0) {
			throw ParserException("with synonym could not be empty");
		}
		//std::cout << "withSynonym = " << withSynonym.at(i) << '\n';
	}
	return withSynonym;
}

vector<string> ParserForWith::split(const string & s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

vector<string>& ParserForWith::split(const string & s, char delim, vector<string>& elems)
{
	stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> ParserForWith::getWithSynonym()
{
	return withSynonym;
}
