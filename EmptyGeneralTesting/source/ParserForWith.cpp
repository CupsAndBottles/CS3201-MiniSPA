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
	vector<string> withSubset;
	std::string toBeSplit = synonym[0].at(pos);
	for (std::size_t i = 0; i < 2; i++) {
		char charSplitWith = toBeSplitWith[i];
		std::size_t found = synonym[0].at(pos).find(charSplitWith);
		
		if (found != std::string::npos) {
			withSubset = split(toBeSplit, charSplitWith);
			toBeSplit = getToBeSplit(withSubset, i);
		}
		if (toBeSplit.compare("") == 0) {
			toBeSplit = synonym[0].at(pos);
		}
	}

	withSynonym = removeUnwanted(withSynonym);
	if(withSynonym.size() == 0) {
		throw ParserException("Wrong synax for writting pattern.");
	}
	//for (int j = 0; j < withSynonym.size(); j++) {
		//std::cout << "withSyn = " << withSynonym.at(j) << '\n';
	//}
}

string ParserForWith::getToBeSplit(vector<string> patternSubset, int i)
{
	string toBeSplit = "";
	if (patternSubset.size() == 2 && i != 1) {
		toBeSplit = patternSubset.at(1);
		withSynonym.push_back(patternSubset.at(0));
	}
	else if (patternSubset.size() == 3) {
		toBeSplit = patternSubset.at(1);
		withSynonym.push_back(patternSubset.at(0));
		withSynonym.push_back(patternSubset.at(2));
	}
	else if (i == 1) {
		withSynonym.push_back(patternSubset.at(0));
		withSynonym.push_back(patternSubset.at(1));
	}
	
	return toBeSplit;
}

vector<string> ParserForWith::removeUnwanted(vector<string> withSynonym)
{
	std::regex toBeRemove("<|>|\\s+|;|\\s");
	for (size_t i = 0; i < withSynonym.size(); i++) {
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
