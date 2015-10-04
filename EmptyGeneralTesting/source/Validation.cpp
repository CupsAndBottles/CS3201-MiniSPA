#include "Validation.h"
#include "RelTable.h"
#include "ParserException.h"
#include<vector>
#include<string>
#include <iostream>

using namespace std;

Validation::Validation(vector<vector<string>> suchThatSynAndType, vector<vector<string>> patternSynAndType)
{
	grammarValidation(suchThatSynAndType);
	patternValidation(patternSynAndType);
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
				RelTable relTableClass(relName);
				ag1 = relTableClass.getAg1Synonym();
				ag2 = relTableClass.getAg2Synonym();
			}
			else if (i % 3 == 1) {
				if (!std::regex_match(suchThatSynAndType[1].at(i), synonym.at(ag1))) {
					throw ParserException("Grammar is wrong for " + suchThatSynAndType[0].at(0) + " of " + suchThatSynAndType[0].at(i));
				}
			}
			else {
				if (!std::regex_match(suchThatSynAndType[1].at(i), synonym.at(ag2))) {
					throw ParserException("Grammar is wrong for " + suchThatSynAndType[0].at(0) + " of " + suchThatSynAndType[0].at(i));
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
			string relName = "Pattern" + patternSynAndType[1].at(i);
			RelTable relTableClass(relName);
			ag1 = relTableClass.getAg1Synonym();
			ag2 = relTableClass.getAg2Synonym();
			noOfArgs = relTableClass.getArgNo();


			if (!std::regex_match(patternSynAndType[1].at(i + 1), synonym.at(ag1))) {
				throw ParserException("Grammar is wrong for " + patternSynAndType[0].at(0) + " of " + patternSynAndType[0].at(i));
			}
			else if (!std::regex_match(patternSynAndType[1].at(i + 2), synonym.at(ag2))) {
				throw ParserException("Grammar is wrong for pattern of " + patternSynAndType[0].at(0));

			}
			else if (patternSynAndType[1].at(i).compare("if") == 0 && !std::regex_match(patternSynAndType[1].at(i + 3), synonym.at(ag2))) {
				throw ParserException("Grammar is wrong for pattern of " + patternSynAndType[0].at(0));

			}

			if (patternSynAndType[1].at(i).compare("if") == 0) {
				i++;
			}
		}
	}
}
