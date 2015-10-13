#include "ParserTypeWithSyn.h"
#include "ParserOfType.h"
#include <regex>
#include <string>
#include <iostream>
#include <locale> 
#include "QueryTree.h"
#include "ParserException.h"
#include "Validation.h"
#include "PKB.h"


using namespace std;

ParserTypeWithSyn::ParserTypeWithSyn(PKB &querypkb, vector<string> selectSynonym, vector<vector<string>> suchThatSynonym,
	
	vector<vector<string>> withSynonym,
	vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym)
{
	this->pkb = &querypkb;
	parseSelectTypeWithSyn(selectSynonym, type, synonym);
	parseSuchThatTypeWithSyn(suchThatSynonym, type, synonym);
	parseWithTypeWithSyn(withSynonym, type, synonym);
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

vector<vector<string>> ParserTypeWithSyn::getWithSynAndType()
{
	return withSynAndType;
}


void ParserTypeWithSyn::parseSelectTypeWithSyn(vector<string> selectSynonym, vector<string> type, vector<string> synonym)
{
	selectSynAndType.push_back(vector <string>());
	selectSynAndType.push_back(vector <string>());
	selectSynAndType.push_back(vector <string>());
	selectSynAndType.push_back(vector <string>());

	vector<vector<string>> temp;

	ParserOfType parserOfType;
	for (int i = 0; i < selectSynonym.size(); i++) {
		temp = parserOfType.setClauseType(0, selectSynonym.at(i), type, synonym);
		if (temp.size() == 0) {
			throw ParserException("Select synonym unidentified");
		}
		selectSynAndType[0].insert(selectSynAndType[0].end(), temp[0].begin(), temp[0].end());
		selectSynAndType[1].insert(selectSynAndType[1].end(), temp[1].begin(), temp[1].end());
		selectSynAndType[2].insert(selectSynAndType[2].end(), temp[2].begin(), temp[2].end());
		selectSynAndType[3].insert(selectSynAndType[3].end(), temp[3].begin(), temp[3].end());
	}

	if (selectSynonym.size() == 0) {
		throw ParserException("No Select condition");
	}
}

void ParserTypeWithSyn::parseSuchThatTypeWithSyn(vector<vector<string>> suchThatSynonym, vector<string> type, vector<string> synonym)
{
	suchThatSynAndType.push_back(vector <string>());
	suchThatSynAndType.push_back(vector <string>());
	suchThatSynAndType.push_back(vector <string>());
	suchThatSynAndType.push_back(vector <string>());
	int index;
	vector<vector<string>> temp;
	ParserOfType parserOfType;
	string indication = "";
	bool ifSame = false;
	for (std::size_t i = 0; i < suchThatSynonym.size(); i++) {
		if (suchThatSynonym[i].size() != 3) {
			throw ParserException("unidentified such that synonym");
		}
		temp = parserOfType.setClauseType(1, suchThatSynonym[i].at(0), type, synonym);
		//std::cout << "suchThatSynonym[i].at(0) = " << suchThatSynonym[i].at(0) << '\n';
		//std::cout << "temp1 = " << temp.size() << '\n';
		if (temp.size() == 0) {
			throw ParserException("SuchThat synonym unidentified");
		}
		
		suchThatSynAndType[0].insert(suchThatSynAndType[0].end(), temp[0].begin(), temp[0].end());
		suchThatSynAndType[1].insert(suchThatSynAndType[1].end(), temp[1].begin(), temp[1].end());
		suchThatSynAndType[2].insert(suchThatSynAndType[2].end(), temp[2].begin(), temp[2].end());
		suchThatSynAndType[3].insert(suchThatSynAndType[3].end(), temp[3].begin(), temp[3].end());

		for (std::size_t k = 1; k < 3; k++) {
			if (suchThatSynonym[i].at(0).compare("Modifies") == 0 && k == 1 || suchThatSynonym[i].at(0).compare("Uses") == 0 && k == 1 || suchThatSynonym[i].at(0).compare("Calls") == 0) {
				indication = "procedure";
			}
			else if (suchThatSynonym[i].at(0).compare("Modifies") == 0 && k == 2 || suchThatSynonym[i].at(0).compare("Uses") == 0 && k == 2) {
				indication = "variable";
			}
			temp = parserOfType.setType(*pkb, 1, suchThatSynonym[i].at(k), type, synonym, indication);
			if (temp.size() == 0) {
			//	std::cout << "suchThatSynonym[i].at(0) = " << suchThatSynonym[i].at(k) << '\n';
			//	std::cout << "temp2 = " << temp.size() << '\n';
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
	ParserOfType parserOfType;
	vector<vector<string>> temp;
	withSynAndType.push_back(vector <string>());
	withSynAndType.push_back(vector <string>());
	withSynAndType.push_back(vector <string>());
	withSynAndType.push_back(vector <string>());
	withSynAndType.push_back(vector <string>());
	int pos;
	Validation validation;
	
	for (std::size_t i = 0; i < withSynonym.size(); i++) {
		//std::cout << "size = " << withSynonym[i].size() << '\n';
		if (withSynonym[i].size() == 4) {
			withSynonym[i] = arrangeSyn(withSynonym[i]);
		}
		for (int k = 0; k < withSynonym[i].size(); k++) {
			if (k == 0 || (withSynonym[i].size() == 4 && k == 2)) {
				pos = parserOfType.isBeingDeclared(withSynonym[i].at(k), synonym);

				if (pos == -1) {
					throw ParserException("With synonym unidentified");
				}
				else {
					withSynAndType[0].push_back(withSynonym[i].at(k));
					withSynAndType[1].push_back(type.at(pos));
					withSynAndType[2].push_back("-1");
				}
			//	std::cout << "withSynAndType[0] = " << withSynAndType[0].at(0) << '\n';
			}
			else if (withSynonym[i].size() > 2 && (k == 1 || k == 3)) {
				int size = withSynAndType[1].size() - 1;
				if (withSynonym[i].at(k).compare("stmt#") == 0) {
					withSynAndType[3].push_back("1");
				}
				else {
					withSynAndType[3].push_back("0");
				}
				validation.withValidation(withSynAndType[1].at(size), withSynonym[i].at(k));
			}
			else if ((withSynonym[i].size() == 2 && k == 1) || k == 2) {
				int size = withSynAndType[1].size() - 1;
				if (withSynonym[i].at(k-1).compare("procName") == 0) {
				//	std::cout << "withSynAndType[0]1.1 = " << withSynAndType[1].at(size) << '\n';
					temp = parserOfType.setProcedureTypeAndSyn(*pkb, withSynonym[i].at(k), withSynonym[i].at(k - 1));
				//	std::cout << "temp[0] = " << temp[0].at(0) << '\n';
				}
				else if (withSynonym[i].at(k - 1).compare("varName") == 0) {
					temp = parserOfType.setVariableTypeAndSyn(*pkb, withSynonym[i].at(k));
				}
				else if (withSynonym[i].at(k - 1).compare("value") == 0 || withSynonym[i].at(k - 1).compare("stmt#") == 0) {
					temp = parserOfType.setDigitTypeAndSyn(withSynonym[i].at(k - 1),  withSynonym[i].at(k));
				}
				else if (withSynAndType[1].at(size).compare("prog_line") == 0) {
					temp = parserOfType.setDigitTypeAndSyn(withSynAndType[1].at(size), withSynonym[i].at(k));
				}
				if (temp.size() == 0) {
					throw ParserException("With synonym unidentified");
				}
				
				withSynAndType[0].insert(withSynAndType[0].end(), temp[0].begin(), temp[0].end());
				withSynAndType[1].insert(withSynAndType[1].end(), temp[1].begin(), temp[1].end());
				withSynAndType[2].insert(withSynAndType[2].end(), temp[2].begin(), temp[2].end());
				withSynAndType[3].insert(withSynAndType[3].end(), temp[3].begin(), temp[3].end());
				//std::cout << "withSynAndType[0]1 = " << withSynAndType[0].at(1) << '\n';
			}
		}
		if (withSynonym[i].size() == 4) {
			int size1 = withSynAndType[1].size() - 1;
			int size2 = withSynAndType[1].size() - 2;
			validation.withValidation(withSynAndType[1].at(size1), withSynAndType[1].at(size2));
		}
	}
}

void ParserTypeWithSyn::parsePatternTypeWithSyn(vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym)
{
	ParserOfType parserOfType;
	vector<vector<string>> temp;
	patternSynAndType.push_back(vector <string>());
	patternSynAndType.push_back(vector <string>());
	patternSynAndType.push_back(vector <string>());
	patternSynAndType.push_back(vector <string>());

	for (std::size_t i = 0; i < patternSynonym.size(); i++) {
		temp = parserOfType.setClauseType(2, patternSynonym[i].at(0), type, synonym);
		if (temp.size() == 0) {
			throw ParserException("Pattern synonym unidentified");
		}
		
			patternSynAndType[0].insert(patternSynAndType[0].end(), temp[0].begin(), temp[0].end());
			patternSynAndType[1].insert(patternSynAndType[1].end(), temp[1].begin(), temp[1].end());
			patternSynAndType[2].insert(patternSynAndType[2].end(), temp[2].begin(), temp[2].end());
			patternSynAndType[3].insert(patternSynAndType[3].end(), temp[3].begin(), temp[3].end());

		for (int k = 1; k < patternSynonym[i].size(); k++) {
			temp = parserOfType.setType(*pkb, 2, patternSynonym[i].at(k), type, synonym, "variable");
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
	if(suchThatSynAndType.size() > 0 && patternSynAndType.size() > 0) {
		for (int i = 0; i < suchThatSynAndType[0].size(); i++) {
			for (int j = 0; j < patternSynAndType[0].size(); j++) {
				if (suchThatSynAndType[0].at(i).compare(patternSynAndType[0].at(j)) == 0 && temp.compare(suchThatSynAndType[0].at(i)) != 0 &&
					suchThatSynAndType[0].at(i).compare("_") != 0) {
					temp = suchThatSynAndType[0].at(i);
					number++;
				}
			}
		}
		if (number > 1) {
			throw ParserException("Only 1 common synonym should be evaluated");
		}
	}
}

vector<string> ParserTypeWithSyn::arrangeSyn(vector<string> syn)
{
	vector<string> temp; 
	temp.push_back(syn.at(0));
	temp.push_back(syn.at(2));
	temp.push_back(syn.at(3));
	temp.push_back(syn.at(1));
	
	return temp;
}
