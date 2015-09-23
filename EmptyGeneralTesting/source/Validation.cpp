#include "Validation.h"
#include "RelTable.h"
#include "ParserException.h"
#include<vector>
#include<string>
#include <iostream>

using namespace std;

Validation::Validation(vector<vector<string>> suchThatSynAndType)
{
	grammarValidation(suchThatSynAndType);
}


Validation::~Validation()
{
}

void Validation::grammarValidation(vector<vector<string>> suchThatSynAndType)
{
	string relName;
	int ag1 = -1, ag2 = -1;
	bool isSyn = false;
	for (int i = 0; i < suchThatSynAndType[1].size(); i++) {

		if (suchThatSynAndType[1].at(i).compare("") == 0) {
			relName = suchThatSynAndType[0].at(0);
			RelTable relTableClass(relName);
			ag1 = relTableClass.getAg1Synonym();
			ag2 = relTableClass.getAg2Synonym();
			//	std::cout << "relName = " << relTableClass.getRe << '\n';
			//std::cout << "relName = " << ag1 << '\n';
			isSyn = true;
		}
		else if (i % 3 == 1) {
			for (int j = 0; j < 3; j++) {
				if (std::regex_match(suchThatSynAndType[1].at(i), synonym.at(ag1))) {

					isSyn = true;
					break;
				}
			}
		}
		else {
			for (int j = 0; j < 3; j++) {
				//	std::cout << "type1 = " << ag2 << '\n';
				if (std::regex_match(suchThatSynAndType[1].at(i), synonym.at(ag2))) {
					isSyn = true;
					break;
				}
			}
		}
		if (!isSyn) {
			throw ParserException("Grammar is wrong for " + suchThatSynAndType[0].at(0) + " of " + suchThatSynAndType[0].at(i));
		}
		isSyn = false;
	}

}
