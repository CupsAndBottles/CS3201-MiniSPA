#include "ParserTypeWithSyn.h"
#include <regex>
#include <string>
#include <iostream>
#include <locale> 
#include "QueryTree.h"
#include "PKB.h"
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

		std::cout << "enterParserTypeSelect" << '\n';

	selectSynAndType.push_back(vector <string>());
	selectSynAndType.push_back(vector <string>());

	bool ifSame = false;
	for (std::size_t i = 0; i < selectSynonym.size(); i++) {
		for (std::size_t j = 0; j < synonym.size(); j++) {
				std::cout << "selectSynonym.at(i) = " << selectSynonym.at(i) << '\n';
			  std::cout << "select222) = " << synonym.at(j) << '\n';
			if (selectSynonym.at(i).compare(synonym.at(j)) == 0) {
					std::cout << "ININININININININININ" << '\n';
				selectSynAndType[0].push_back(selectSynonym.at(i));
				selectSynAndType[1].push_back(type.at(j));

				ifSame = true;
				break;
			}
			else if (selectSynonym.at(i).compare("BOOLEAN") == 0) {
				selectSynAndType[0].push_back(selectSynonym.at(i));
				selectSynAndType[1].push_back("BOOLEAN");

				ifSame = true;
				break;
			}
		}
		if (!ifSame) {
			throw ParserException("select synonym unidentified");
		}
	}

}

void ParserTypeWithSyn::parseSuchThatTypeWithSyn(vector<vector<string>> suchThatSynonym, vector<string> type, vector<string> synonym)
{
	int index;
	suchThatSynAndType.push_back(vector <string>()); //stringVal
	suchThatSynAndType.push_back(vector <string>()); //type
	suchThatSynAndType.push_back(vector <string>()); //intVal

	bool ifSame = false;
	for (std::size_t i = 0; i < suchThatSynonym.size(); i++) {
		if (suchThatSynonym[i].size() != 3) {
			throw ParserException("unidentified such that synonym");
		}
	//	std::cout << "suchthatmodifies = " << suchThatSynonym[i].at(0) << '\n';

		suchThatSynAndType[0].push_back(suchThatSynonym[i].at(0));
		suchThatSynAndType[1].push_back("");
		suchThatSynAndType[2].push_back("");
		for (std::size_t k = 1; k < 3; k++) {

			std::cout << "selectSynonym.at(k) = " << suchThatSynonym[i].at(k) << '\n';

			std::size_t found = suchThatSynonym[i].at(k).find("\"");
			if (ifDigit(suchThatSynonym[i].at(k))) {
				//	std::cout << "isDigit = " << '\n';
				if (suchThatSynonym[i].at(k)[0] == '\"' && suchThatSynonym[i].at(k)[suchThatSynonym[i].at(k).length() - 1] == '\"') {
					suchThatSynonym[i].at(k) = suchThatSynonym[i].at(k).substr(1, suchThatSynonym[i].at(k).length() - 2);
				}

				suchThatSynAndType[0].push_back(suchThatSynonym[i].at(k));
				suchThatSynAndType[1].push_back("prog_line");
				suchThatSynAndType[2].push_back(suchThatSynonym[i].at(k));
				ifSame = true;
			}
			else if (found != std::string::npos) {
				if (suchThatSynonym[i].at(k).length() == 3) {
					//index = getIndex(suchThatSynonym[i].at(k));
					//suchThatSynAndType[2].push_back(std::to_string(index));
					suchThatSynAndType[0].push_back(suchThatSynonym[i].at(k));
					suchThatSynAndType[1].push_back("variable");
					suchThatSynAndType[2].push_back("-1");
					ifSame = true;
				}
				else {
					//index = getProcIndexNum(suchThatSynonym[i].at(k));
					//suchThatSynAndType[2].push_back(std::to_string(index));
					suchThatSynAndType[0].push_back(suchThatSynonym[i].at(k));
					suchThatSynAndType[1].push_back("procedure");
					suchThatSynAndType[2].push_back("-1");
					ifSame = true;
				}
			}
			else if (suchThatSynonym[i].at(k).compare("_") == 0) {
				suchThatSynAndType[0].push_back(suchThatSynonym[i].at(k));
				suchThatSynAndType[1].push_back("_");
				suchThatSynAndType[2].push_back("-2");
				ifSame = true;
			}
			else {
				ifSame = matchWithKeyword(suchThatSynonym[i].at(k), type, synonym);
				suchThatSynAndType[2].push_back("-1");
				//std::cout << "pos = " << k <<'\n';
			}
			if (!ifSame) {
				//"type in unidentify synonym"
				throw ParserException("such that synonym unidentified");
			}
			else {
				ifSame = false;
			}
		}
	}
	/*
	std::cout << "1 suchThatSynAndType = " << suchThatSynAndType[0].at(0) << '\n';
	std::cout << "suchThatSynAndType = " << suchThatSynAndType[0].at(1) << '\n';
	std::cout << "suchThatSynAndType = " << suchThatSynAndType[1].at(1) << '\n';
	std::cout << "suchThatSynAndType = " << suchThatSynAndType[0].at(2) << '\n';
	std::cout << "suchThatSynAndType = " << suchThatSynAndType[1].at(2) << '\n';
	*/
}

void ParserTypeWithSyn::parseWithTypeWithSyn(vector<vector<string>> withSynonym, vector<string> type, vector<string> synonym)
{

}

void ParserTypeWithSyn::parsePatternTypeWithSyn(vector<vector<string>> patternSynonym, vector<string> type, vector<string> synonym)
{
	patternSynAndType.push_back(vector<string>()); //stringVal
	patternSynAndType.push_back(vector<string>()); //type
	patternSynAndType.push_back(vector<string>()); //intVal
	patternSynAndType.push_back(vector<string>()); //isExpression(1 true)
	int index;
	bool ifSame = false;

	for (std::size_t i = 0; i < patternSynonym.size(); i++) {
		for (std::size_t j = 0; j < synonym.size(); j++) {
			if (patternSynonym[i].at(0).compare(synonym.at(j)) == 0) {
				//std::cout << "selectSynonym.at(i) = " << selectSynonym.at(i) << '\n';
				checkType(type.at(j));
				patternSynAndType[0].push_back(patternSynonym[i].at(0));
				patternSynAndType[1].push_back(type.at(j));
				patternSynAndType[2].push_back("-1");
				patternSynAndType[3].push_back("0");
				ifSame = true;
				break;
			}
		}
		if (!ifSame) {
			throw ParserException("Pattern synonym unidentified");
		}
		ifSame = false;

		// next iteration add one more equation type 
		if (patternSynonym[i].at(1).compare("_") == 0) {
			patternSynAndType[0].push_back(patternSynonym[i].at(1));
			patternSynAndType[1].push_back("_");
			patternSynAndType[2].push_back("-2");
			patternSynAndType[3].push_back("0");
			ifSame = true;
		}
		else if (checkIfVariable(patternSynonym[i].at(1))) {
			//index = getIndex(patternSynonym[i].at(1));
			//patternSynAndType[2].push_back(std::to_string(index));
			patternSynAndType[0].push_back(patternSynonym[i].at(1));
			patternSynAndType[1].push_back("variable");
			patternSynAndType[2].push_back("-1");
			patternSynAndType[3].push_back("0");
			ifSame = true;
		}
		else if (ifDigit(patternSynonym[i].at(1))) {
			if (patternSynonym[i].at(1)[0] == '\"' && patternSynonym[i].at(1)[patternSynonym[i].at(1).length() - 1] == '\"') {
				patternSynonym[i].at(1) = patternSynonym[i].at(1).substr(1, patternSynonym[i].at(1).length() - 2);
			}
			patternSynAndType[0].push_back(patternSynonym[i].at(1));
			patternSynAndType[1].push_back("constant");
			patternSynAndType[2].push_back(patternSynonym[i].at(1));
			patternSynAndType[3].push_back("0");
			ifSame = true;
		}
		else {
			for (std::size_t j = 0; j < synonym.size(); j++) {
				if (patternSynonym[i].at(1).compare(synonym.at(j)) == 0) {
					//	std::cout << "patternSynonym.at(i) = " << patternSynonym[i].at(1) << '\n';
					patternSynAndType[0].push_back(patternSynonym[i].at(1));
					patternSynAndType[1].push_back(type.at(j));
					patternSynAndType[2].push_back("-1");
					patternSynAndType[3].push_back("0");
					ifSame = true;
					break;
				}
			}
		}
		if (!ifSame) {
			throw ParserException("Pattern synonym unidentified");
		}
		ifSame = false;

		patternSynonym[i].at(2) = checkExpression(patternSynonym[i].at(2));
		if (patternSynonym[i].at(2).compare("_") == 0 && patternSynonym[i].at(1).compare("_") != 0) {
			patternSynAndType[0].push_back(patternSynonym[i].at(2));
			patternSynAndType[1].push_back("_");
			patternSynAndType[2].push_back("-2");
			
			ifSame = true;
		}
		else if (checkIfVariable(patternSynonym[i].at(2))) {
			//index = getIndex(patternSynonym[i].at(2));
			//patternSynAndType[2].push_back(std::to_string(index));
			patternSynAndType[0].push_back(patternSynonym[i].at(2));
			patternSynAndType[1].push_back("variable");
			patternSynAndType[2].push_back("-1");
			ifSame = true;
		}
		else if (ifDigit(patternSynonym[i].at(2))) {
			if (patternSynonym[i].at(2)[0] == '\"' && patternSynonym[i].at(2)[patternSynonym[i].at(2).length() - 1] == '\"') {
				patternSynonym[i].at(2) = patternSynonym[i].at(2).substr(1, patternSynonym[i].at(2).length() - 2);
			}
			patternSynAndType[0].push_back(patternSynonym[i].at(2));
			patternSynAndType[1].push_back("constant");
			patternSynAndType[2].push_back(patternSynonym[i].at(2));
			ifSame = true;
		}
		else if (isContainsPunc(patternSynonym[i].at(2))) {
			patternSynAndType[0].push_back(patternSynonym[i].at(2));
			patternSynAndType[1].push_back("equation");
			patternSynAndType[2].push_back("-1");
			ifSame = true;
		}
		else {
			for (std::size_t j = 0; j < synonym.size(); j++) {
				if (patternSynonym[i].at(2).compare(synonym.at(j)) == 0) {
					//std::cout << "selectSynonym.at(i) = " << selectSynonym.at(i) << '\n';
					patternSynAndType[0].push_back(patternSynonym[i].at(2));
					patternSynAndType[1].push_back(type.at(j));
					patternSynAndType[2].push_back("-1");
					ifSame = true;
					break;
				}
			}
		}
		if (!ifSame) {
			throw ParserException("Pattern synonym unidentified");
		}
		ifSame = false;
		//std::cout << "pattern in type = " << patternSynAndType[3].at(1) << '\n';

	}

}

void ParserTypeWithSyn::checkType(string syn)
{

	std::size_t found = syn.find("while");
	std::size_t found1 = syn.find("if");
	std::size_t found2 = syn.find("assign");

	if (found == std::string::npos && found1 == std::string::npos && found2 == std::string::npos) {
		throw ParserException("Pattern must be while or if or assign");
	}
}

bool ParserTypeWithSyn::isContainsPunc(string args) {
	std::size_t found = args.find("+");
	std::size_t found1 = args.find("-");
	std::size_t found2 = args.find("*");

	if (found == std::string::npos && found1 == std::string::npos && found2 == std::string::npos) {
		return false;
	}
	return true;
}
string ParserTypeWithSyn::checkExpression(string arg2) {
	if (arg2.at(0) == ('_') && arg2.at(arg2.length() - 1) == ('_') && arg2.length() > 2) {
		patternSynAndType[3].push_back("1");
		arg2 = arg2.substr(1, arg2.length()-2);
	
	}
	else if (arg2.at(0) == ('_') && arg2.at(arg2.length() - 1) != ('_')  && arg2.length() > 2) {
		throw ParserException("Pattern arguments of subexpression must be _..._");
	}
	else if (arg2.at(0) != ('_')  && arg2.at(arg2.length() - 1) == ('_') && arg2.length() > 2) {
		throw ParserException("Pattern arguments of subexpression must be _..._");
	}
	else if (arg2.at(0) == ('_') && arg2.at(arg2.length() - 1) == ('_') && arg2.length() == 2) {
		throw ParserException("Pattern arguments of subexpression must be _..._");
	}
	else {
		patternSynAndType[3].push_back("0");
	}
	return arg2;
}
bool ParserTypeWithSyn::checkIfVariable(string syn)
{
	bool ifVariable = true;
	if (syn.length() != 3) {
		return false;
	}
	std::locale loc;
	for (std::size_t i = 0; i < syn.length(); i++) {
		if ((i == 0 && syn[i] != '\"') || (i == syn.length() - 1 && syn[i] != '\"') || (!isalpha(syn[i], loc) && i != 0 && i != syn.length() - 1)) {
			//std::cout << "syn[i] in = " << syn[i] << '\n';
			ifVariable = false;
			break;
		}
	}
	return ifVariable;
}


bool ParserTypeWithSyn::ifDigit(string syn)
{
	bool ifDigit = true;
	int start = 0, end = syn.length();
	std::size_t found = syn.find("\"");
	if (syn[0] == '\"' && syn[syn.length() - 1] == '\"') {
		start = 1;
		end = syn.length() - 1;
	}
	for (std::size_t i = start; i < end; i++) {
		if (!isdigit(syn[i])) {
			//		std::cout << "ifDigit = " << syn[i] << '\n';
			ifDigit = false;
			break;
		}
	}
	return ifDigit;
}

bool ParserTypeWithSyn::matchWithKeyword(string syn, vector<string> type, vector<string> synonym)
{
	bool ifSame = false;
	for (std::size_t j = 0; j < synonym.size(); j++) {
		if (syn.compare(synonym.at(j)) == 0) {
			suchThatSynAndType[0].push_back(syn);
			suchThatSynAndType[1].push_back(type.at(j));
			ifSame = true;
			break;
		}
	}
	return ifSame;
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
