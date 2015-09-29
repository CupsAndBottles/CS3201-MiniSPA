#include "ParserTypeWithSyn.h"
#include "ParserOfType.h"
#include <regex>
#include <string>
#include <iostream>
#include <locale> 
#include "QueryTree.h"
#include "ParserException.h"


using namespace std;

ParserTypeWithSyn::ParserTypeWithSyn(vector<string> selectSynonym, vector<vector<string>> suchThatSynonym,
	vector<vector<string>> withSynonym,
	vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym)
{
	parseSelectTypeWithSyn(selectSynonym, type, synonym);
	parseSuchThatTypeWithSyn(suchThatSynonym, type, synonym);
	//parseWithTypeWithSyn(withSynonym, type, synonym);
	parsePatternTypeWithSyn(patternSynonym, type, synonym);
	checkCommonSynonym();
}

ParserTypeWithSyn::~ParserTypeWithSyn()
{
}

vector<vector<string>> ParserTypeWithSyn::getSelectSynAndType()
{
	return selectSynAndType;
}

vector<vector<string>> ParserTypeWithSyn::getSuchThatSynAndType()
{
	return suchThatSynAndType;
}

vector<vector<string>> ParserTypeWithSyn::getPatternSynAndType()
{
	return patternSynAndType;
}


void ParserTypeWithSyn::parseSelectTypeWithSyn(vector<string> selectSynonym, vector<string> type, vector<string> synonym)
{
	vector<vector<string>> temp;
	std::cout << selectSynonym.at(0) << '\n';
	ParserOfType parserOfType;
	for (int i = 0; i < selectSynonym.size(); i++) {
		temp = parserOfType.setClauseType(0, selectSynonym.at(i), type, synonym);
		if (temp.size() == 0) {
			throw ParserException("Select synonym unidentified");
		}

		//std::copy(temp.begin(), temp.end(), std::back_inserter(selectSynAndType));
		selectSynAndType.insert(selectSynAndType.end(), temp.begin(), temp.end());
	}

}

void ParserTypeWithSyn::parseSuchThatTypeWithSyn(vector<vector<string>> suchThatSynonym, vector<string> type, vector<string> synonym)
{
	int index;
	vector<vector<string>> temp;
	ParserOfType parserOfType;
	bool ifSame = false;
	for (std::size_t i = 0; i < suchThatSynonym.size(); i++) {
		if (suchThatSynonym[i].size() != 3) {
			throw ParserException("unidentified such that synonym");
		}

		temp = parserOfType.setClauseType(1, suchThatSynonym[i].at(0), type, synonym);
		if (temp.size() == 0) {
			throw ParserException("SuchThat synonym unidentified");
		}
		suchThatSynAndType.insert(suchThatSynAndType.end(), temp.begin(), temp.end());
		for (std::size_t k = 1; k < 3; k++) {
			temp = parserOfType.setType(1, suchThatSynonym[i].at(k), type, synonym);
			if (temp.size() == 0) {
				throw ParserException("SuchThat synonym unidentified");
			}

			suchThatSynAndType[0].insert(suchThatSynAndType[0].end(), temp[0].begin(), temp[0].end());
			suchThatSynAndType[1].insert(suchThatSynAndType[1].end(), temp[1].begin(), temp[1].end());
			suchThatSynAndType[2].insert(suchThatSynAndType[2].end(), temp[2].begin(), temp[2].end());
			suchThatSynAndType[3].insert(suchThatSynAndType[3].end(), temp[3].begin(), temp[3].end());
		
		}
	}
}

void ParserTypeWithSyn::parseWithTypeWithSyn(vector<vector<string>> withSynonym, vector<string> type, vector<string> synonym)
{

}

void ParserTypeWithSyn::parsePatternTypeWithSyn(vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym)
{
	//patternSynAndType.push_back(vector<string>()); //stringVal
	//patternSynAndType.push_back(vector<string>()); //type
	//patternSynAndType.push_back(vector<string>()); //intVal
	//patternSynAndType.push_back(vector<string>()); //isExpression(1 true)
	
	ParserOfType parserOfType;
	vector<vector<string>> temp;
	for (std::size_t i = 0; i < patternSynonym.size(); i++) {
		temp = parserOfType.setClauseType(2, patternSynonym[i].at(0), type, synonym);
		if (temp.size() == 0) {
			throw ParserException("Pattern synonym unidentified");
		}
		if (i == 0) {
			patternSynAndType.insert(patternSynAndType.end(), temp.begin(), temp.end());
		}
		else {
			patternSynAndType[0].insert(patternSynAndType[0].end(), temp[0].begin(), temp[0].end());
			patternSynAndType[1].insert(patternSynAndType[1].end(), temp[1].begin(), temp[1].end());
			patternSynAndType[2].insert(patternSynAndType[2].end(), temp[2].begin(), temp[2].end());
			patternSynAndType[3].insert(patternSynAndType[3].end(), temp[3].begin(), temp[3].end());
		}

		for (int k = 1; k < patternSynonym[i].size(); k++) {
			temp = parserOfType.setType(2, patternSynonym[i].at(k), type, synonym);
			if (temp.size() == 0) {
				throw ParserException("Pattern synonym unidentified");
			}
			patternSynAndType[0].insert(patternSynAndType[0].end(), temp[0].begin(), temp[0].end());
			patternSynAndType[1].insert(patternSynAndType[1].end(), temp[1].begin(), temp[1].end());
			patternSynAndType[2].insert(patternSynAndType[2].end(), temp[2].begin(), temp[2].end());
			patternSynAndType[3].insert(patternSynAndType[3].end(), temp[3].begin(), temp[3].end());
		}
	}

}

void ParserTypeWithSyn::checkCommonSynonym()
{
	int number = 0;
	string temp = "";
	for (int i = 0; i < suchThatSynAndType[0].size(); i++) {
		for (int j = 0; j < patternSynAndType[0].size(); j++) {
			if (suchThatSynAndType[0].at(i).compare(patternSynAndType[0].at(j)) == 0 && temp.compare(suchThatSynAndType[0].at(i)) != 0 &&
				suchThatSynAndType[0].at(i).compare("_") != 0) {
				temp = suchThatSynAndType[0].at(i);
				number++;
			}
		}

		if (number > 1) {
			throw ParserException("Only 1 common synonym should be evaluated");
		}
	}
}
