 #include "ParserForSuchThat.h"
#include "ParserException.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

ParserForSuchThat::ParserForSuchThat(vector<vector<string>> type, vector<vector<string>> synonym, int index)
{
	parseSuchThat(type, synonym, index);
}

ParserForSuchThat::~ParserForSuchThat()
{
}

void ParserForSuchThat::parseSuchThat(vector<vector<string>> type, vector<vector<string>> synonym, int pos)
{
	vector<string> suchThatSubset;
	std::string toBeSplit = synonym[0].at(pos);
	//std::cout << "synonym = " << synonym[0].at(pos) << '\n';

	for (std::size_t i = 0; i < 3; i++) {
		char charSplitWith = toBeSplitWith[i];
		std::size_t found = synonym[0].at(pos).find(charSplitWith);
		if (found != std::string::npos || toBeSplit.compare("ERROR") == 0) {
			suchThatSubset = split(toBeSplit, charSplitWith);
			toBeSplit = getToBeSplit(suchThatSubset, i);
			//	std::cout << "i = " << toBeSplit <<'\n'; 
		}
		else {
			throw ParserException("Wrong synax for writting such that.");
		}
	}

	suchThatSynonym = removeUnwanted(suchThatSynonym);


}

vector<string> ParserForSuchThat::removeUnwanted(vector<string> suchThatSynonym)
{
	std::regex toBeRemove("<|>|\\s+|;|\\s");
	for (std::size_t i = 0; i < suchThatSynonym.size(); i++) {
		suchThatSynonym.at(i) = std::regex_replace(suchThatSynonym.at(i), toBeRemove, "");
		if (suchThatSynonym.at(i).compare("") == 0) {
			throw ParserException("Empty synonym in such that is not allowed");
		}
		//	std::cout << "suchThatSynonym = " << suchThatSynonym.at(i) << '\n';
	}
	return suchThatSynonym;
}

string ParserForSuchThat::getToBeSplit(vector<string> suchThatSubset, int i)
{
	string toBeSplit;
	if (suchThatSubset.size() == 2) {
		toBeSplit = suchThatSubset.at(1);
		suchThatSynonym.push_back(suchThatSubset.at(0));
	}
	else if (i == 2) {
		toBeSplit = "";
		suchThatSynonym.push_back(suchThatSubset.at(0));
	}
	else {
		toBeSplit = "ERROR";
	}
	return toBeSplit;
}

vector<string> ParserForSuchThat::split(const string & s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

vector<string>& ParserForSuchThat::split(const string & s, char delim, vector<string>& elems)
{
	stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> ParserForSuchThat::getSuchThatSynonym()
{
	return suchThatSynonym;
}


