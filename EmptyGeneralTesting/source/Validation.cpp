#include "Validation.h"
#include "RelTable.h"
#include "ParserException.h"
#include<vector>
#include<string>
#include <iostream>

using namespace std;

Validation::Validation()
{
}


Validation::~Validation()
{
}

void Validation::grammarValidation(vector<vector<string>> suchThatSynAndType)
{
	string relName;
	int ag1 = -1, ag2 = -1, noOfArgs;
	bool isSyn = false;
	if (suchThatSynAndType.size() > 0) {
		for (int i = 0; i < suchThatSynAndType[1].size(); i++) {
			if (suchThatSynAndType[1].at(i).compare("") == 0) {
				relName = suchThatSynAndType[0].at(i);
				std::transform(relName.begin(), relName.end(), relName.begin(), ::tolower);
				RelTable relTableClass(relName);
				ag1 = relTableClass.getAg1Synonym();
				ag2 = relTableClass.getAg2Synonym();
			}
			else if (i % 3 == 1) {
				if (!std::regex_match(suchThatSynAndType[1].at(i), synonym.at(ag1))) {
					throw ParserException("Grammar is wrong for " + relName + " of " + suchThatSynAndType[1].at(i));
				}
			}
			else {
				if (!std::regex_match(suchThatSynAndType[1].at(i), synonym.at(ag2))) {
					throw ParserException("Grammar is wrong for " + relName + " of " + suchThatSynAndType[1].at(i));
				}
			}
		}
	}
}

void Validation::patternValidation(vector<vector<string>> patternSynAndType)
{
	int ag1, ag2, noOfArgs;
	bool isSyn = false;
	if (patternSynAndType.size() > 0) {
		for (std::size_t i = 0; i < patternSynAndType[0].size(); i = i + 3) {
		    std::transform(patternSynAndType[1].at(i).begin(), patternSynAndType[1].at(i).end(), patternSynAndType[1].at(i).begin(), ::tolower);
			string relName = "Pattern" + patternSynAndType[1].at(i);
			RelTable relTableClass(relName);
			ag1 = relTableClass.getAg1Synonym();
			ag2 = relTableClass.getAg2Synonym();
			noOfArgs = relTableClass.getArgNo();


			if (!std::regex_match(patternSynAndType[1].at(i + 1), synonym.at(ag1))) {
				throw ParserException("Grammar is wrong for pattern of" + patternSynAndType[1].at(i) + " of " + patternSynAndType[0].at(i + 1));
			}
			else if (!std::regex_match(patternSynAndType[1].at(i + 2), synonym.at(ag2))) {
				throw ParserException("Grammar is wrong for pattern of " + patternSynAndType[1].at(i));

			}
			else if (patternSynAndType[1].at(i).compare("if") == 0 && (i+3 >= patternSynAndType[0].size() || !std::regex_match(patternSynAndType[1].at(i + 3), synonym.at(ag2)))) {
				throw ParserException("Grammar is wrong for pattern of " + patternSynAndType[0].at(0));

			}

			if (patternSynAndType[1].at(i).compare("if") == 0) {
				i++;
			}
		}
	}
}

void Validation::withValidation(string declaredType, string userType)
{
	string relName = "With" + userType;
	RelTable relTableClass(relName);
	int ag1 = relTableClass.getAg1Synonym();
	if (!std::regex_match(declaredType, synonym.at(ag1))) {
		throw ParserException("Grammar is wrong for with");
	}
}

char easytolower(char in) {
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

