#include "ParserForPattern.h"
#include "ParserException.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>

using namespace std;


ParserForPattern::~ParserForPattern()
{
}

ParserForPattern::ParserForPattern(vector<vector<string>> type, vector<vector<string>> synonym, int index)
{
	parsePattern(type, synonym, index);
}

void ParserForPattern::parsePattern(vector<vector<string>> type, vector<vector<string>> synonym, int pos)
{

	vector<string> patternSubset;
	std::string toBeSplit = synonym[0].at(pos);
	//std::cout << "synonym = " << synonym[0].at(pos) << '\n';
	patternSubset = split(toBeSplit, '(');
	toBeSplit = getToBeSplit(patternSubset, 0);
	//std::cout << "im patternParser's toBeSplit = " << toBeSplit << '\n';

	for (std::size_t i = 0; i < 2; i++) {
		char charSplitWith = toBeSplitWith[i];
		std::size_t found = synonym[0].at(pos).find(charSplitWith);
		if (found != std::string::npos || toBeSplit.compare("ERROR") == 0) {
			patternSubset = split(toBeSplit, charSplitWith);
			toBeSplit = getToBeSplit(patternSubset, i);
			//	std::cout << "im patternParser's toBeSplit = " << toBeSplit <<'\n'; 
		}
		else {
			throw ParserException("Wrong synax for writting pattern.");
		}

		if (i == 0 && patternSynonym.at(0).compare("if") == 0) {
			i--;
		}
	}

	patternSynonym = removeUnwanted(patternSynonym);
	validatePattern();
	
}

void ParserForPattern::validatePattern() {
	if (patternSynonym.at(0).compare("if") == 0 && patternSynonym.at(2).compare("_") != 0 && patternSynonym.at(3).compare("_") != 0) {
		
	}
}

vector<string> ParserForPattern::removeUnwanted(vector<string> PatternSynonym)
{
	std::regex toBeRemove("<|>|\\s+|;|\\s");
	for (std::size_t i = 0; i < PatternSynonym.size(); i++) {
		PatternSynonym.at(i) = std::regex_replace(PatternSynonym.at(i), toBeRemove, "");
		if (PatternSynonym.at(i).compare("") == 0) {
			throw ParserException("Empty synonym in such that or pattern is not allowed");
		}
		//	std::cout << "PatternSynonym = " << PatternSynonym.at(i) << '\n';
	}
	return PatternSynonym;
}

string ParserForPattern::getToBeSplit(vector<string> PatternSubset, int i)
{
	string toBeSplit;
	std::cout << "im patternParser's added = " << i << '\n';
	if (PatternSubset.size() == 2) {
		toBeSplit = PatternSubset.at(1);
		patternSynonym.push_back(PatternSubset.at(0));
	}
	else if (i == 1) {
		toBeSplit = "";
		patternSynonym.push_back(PatternSubset.at(0));
	}
	else {
		toBeSplit = "ERROR";
	}
	return toBeSplit;
}

vector<string> ParserForPattern::split(const string & s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

vector<string>& ParserForPattern::split(const string & s, char delim, vector<string>& elems)
{
	stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> ParserForPattern::getPatternSynonym()
{
	return patternSynonym;
}





