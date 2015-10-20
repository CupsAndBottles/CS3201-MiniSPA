#include "QueryTree.h"
#include "Clauses.h"
#include <string>
#include <iostream> 

using namespace std;

QueryTree::QueryTree()
{
}



QueryTree::~QueryTree()
{
}

QueryTree & QueryTree::return_created_Object()
{
	return QueryTree();
}



void QueryTree::startPlotting(vector<vector<string>> resultData, vector<vector<string>> suchThatData, vector<vector<string>> patternData, vector<vector<string>> withData)
{
	if (resultData.size() > 0) {
		addResultTree(resultData[0], resultData[1]);
	}
	if (suchThatData.size() > 0) {
		addSuchThatTree(suchThatData[0], suchThatData[1], suchThatData[2]);
	}
	if (patternData.size() > 0) {
		addPatternTree(patternData[0], patternData[1], patternData[2], patternData[3]);
	}

	if (withData.size() > 0) {
		addWithTree(withData[0], withData[1], withData[2], withData[3]);
	}
}


vector<Clauses> QueryTree::getSuchThatTree()
{
	return suchThatTree;
}

vector<Clauses> QueryTree::getPatternTree()
{
	return patternTree;
}

vector<Clauses> QueryTree::getResultTree()
{
	return resultTree;
}

vector<Clauses> QueryTree::getWithTree()
{
	return withTree;
}

void QueryTree::setSuchThatTree(Clauses clause)
{
	suchThatTree.push_back(clause);
}

void QueryTree::setPatternTree(Clauses clause)
{
	patternTree.push_back(clause);
}

void QueryTree::setResultTree(Clauses clause)
{
	 resultTree.push_back(clause);
}

void QueryTree::setWithTree(Clauses clause)
{
	withTree.push_back(clause);
}

bool QueryTree::getIsValid()
{
	return isValid;
}

void QueryTree::addResultTree(vector<string> syn, vector<string> type)
{
	//vector<Clauses> resultTree;
	for (std::size_t i = 0; i < syn.size(); i++) {
		Clauses clause;
		clause.setParentStringVal(syn.at(i));
		clause.setParentType(type.at(i));
		setResultTree(clause);
	/*
		resultTree.push_back(Clauses());
		resultTree.at(i).setParentStringVal(syn.at(i));
		resultTree.at(i).setParentType(type.at(i));
		*/
	}
}

void QueryTree::addWithTree(vector<string> stringVal, vector<string> type, vector<string> intVal, vector<string> isStmt)
{
	int z = 0;

	for (std::size_t i = 0; i < stringVal.size(); i = i + 2) {
		Clauses clause;
		clause.setLeftCType(type.at(i));
		clause.setLeftCIsExpression(false);
		clause.setLeftCIntValue(atoi(intVal.at(i).c_str()));
		clause.setLeftCStringValue(stringVal.at(i));
		clause.setLeftCIsStmt(isStmt.at(i));

		clause.setRightCType(type.at(i + 1));
		clause.setRightCIsExpression(false);
		clause.setRightCIntValue(atoi(intVal.at(i + 1).c_str()));
		clause.setRightCStringValue(stringVal.at(i + 1));
		clause.setRightCIsStmt(isStmt.at(i + 1));
		setWithTree(clause);
		/*
		withTree.push_back(Clauses());
		withTree.at(z).setLeftCType(type.at(i));
		withTree.at(z).setLeftCIsExpression(false);
		withTree.at(z).setLeftCIntValue(atoi(intVal.at(i).c_str()));
		withTree.at(z).setLeftCStringValue(stringVal.at(i));
		withTree.at(z).setLeftCIsStmt(isStmt.at(i));

		withTree.at(z).setRightCType(type.at(i + 1));
		withTree.at(z).setRightCIsExpression(false);
		withTree.at(z).setRightCIntValue(atoi(intVal.at(i + 1).c_str()));
		withTree.at(z).setRightCStringValue(stringVal.at(i + 1));
		withTree.at(z).setRightCIsStmt(isStmt.at(i + 1));
		z++;
		*/
	}
	//for (int j = 0; j < withTree.size(); j++) {
	
		//std::cout << "withTreeLeft = " << withTree.at(j).getLeftCStringValue() << '\n';
//	}
}

void QueryTree::addSuchThatTree(vector<string> stringVal, vector<string> type, vector<string> intVal)
{
	int z = 0;

	for (std::size_t i = 0; i < stringVal.size(); i = i + 3) {
		Clauses clause;
		clause.setParentStringVal(stringVal.at(i));

		clause.setLeftCType(type.at(i + 1));
		clause.setLeftCIsExpression(false);
		clause.setLeftCIntValue(atoi(intVal.at(i + 1).c_str()));
		clause.setLeftCStringValue(stringVal.at(i + 1));

		clause.setRightCType(type.at(i + 2));
		clause.setRightCIsExpression(false);
		clause.setRightCIntValue(atoi(intVal.at(i + 2).c_str()));
		clause.setRightCStringValue(stringVal.at(i + 2));
		setSuchThatTree(clause);
		/*
		suchThatTree.push_back(Clauses());
		suchThatTree.at(z).setParentStringVal(stringVal.at(i));

		suchThatTree.at(z).setLeftCType(type.at(i + 1));
		suchThatTree.at(z).setLeftCIsExpression(false);
		suchThatTree.at(z).setLeftCIntValue(atoi(intVal.at(i + 1).c_str()));
		suchThatTree.at(z).setLeftCStringValue(stringVal.at(i + 1));

		suchThatTree.at(z).setRightCType(type.at(i + 2));
		suchThatTree.at(z).setRightCIsExpression(false);
		suchThatTree.at(z).setRightCIntValue(atoi(intVal.at(i + 2).c_str()));
		suchThatTree.at(z).setRightCStringValue(stringVal.at(i + 2));
		z++;
		*/
	}

}

void QueryTree::addPatternTree(vector<string> stringVal, vector<string> type, vector<string> intVal, vector<string> isExpression)
{
	int z = 0;
	for (std::size_t i = 0; i < stringVal.size(); i = i + 3) {
		Clauses clause;
		clause.setParentStringVal(stringVal.at(i));
		clause.setParentType(type.at(i));

		clause.setLeftCType(type.at(i + 1));
		clause.setLeftCIsExpression(isItExpression(isExpression.at(i + 1)));
		clause.setLeftCIntValue(atoi(intVal.at(i + 1).c_str()));
		clause.setLeftCStringValue(stringVal.at(i + 1));

		clause.setRightCType(type.at(i + 2));
		clause.setRightCIsExpression(isItExpression(isExpression.at(i + 2)));
		clause.setRightCIntValue(atoi(intVal.at(i + 2).c_str()));
		clause.setRightCStringValue(stringVal.at(i + 2));
		
		setPatternTree(clause);
		if (type.at(i).compare("if") == 0) {
			i++;
		}
		/*
		patternTree.push_back(Clauses());
		patternTree.at(z).setParentStringVal(stringVal.at(i));
		patternTree.at(z).setParentType(type.at(i));

		patternTree.at(z).setLeftCType(type.at(i + 1));
		patternTree.at(z).setLeftCIsExpression(isItExpression(isExpression.at(i + 1)));
		patternTree.at(z).setLeftCIntValue(atoi(intVal.at(i + 1).c_str()));
		patternTree.at(z).setLeftCStringValue(stringVal.at(i + 1));

		patternTree.at(z).setRightCType(type.at(i + 2));
		patternTree.at(z).setRightCIsExpression(isItExpression(isExpression.at(i + 2)));
		patternTree.at(z).setRightCIntValue(atoi(intVal.at(i + 2).c_str()));
		patternTree.at(z).setRightCStringValue(stringVal.at(i + 2));
		z++;
		if (type.at(i).compare("if") == 0) {
			i++;
		}
		*/
	}
		
}

bool QueryTree::isItExpression(string expression) {
	if (expression.compare("1") == 0) {
		return true;
	}
	else {
		return false;
	}
}
