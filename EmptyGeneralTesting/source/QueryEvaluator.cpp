#pragma once
#include "QueryEvaluator.h"

const string EMPTY_STRING = "";

QueryEvaluator::QueryEvaluator()
{
}

QueryEvaluator::~QueryEvaluator()
{
}

list<string> QueryEvaluator::evaluateQuery(QueryTree tree)
{
	this->tree = tree;
	vector<Clauses> suchThat;
	vector<Clauses> pattern;
	vector<Clauses> select;
	list<string> result;
	bool isTrueClause;

	suchThat = tree.getSuchThatTree();
	pattern = tree.getPatternTree();
	select = tree.getResultTree();

	for (int i = 0; i < suchThat.size(); i++) {
		isTrueClause = evaluateSuchThat(suchThat[i]);
		if (!isTrueClause) {
			list<string> emptyResult{ EMPTY_STRING };
			return emptyResult;
		}
	}

	for (int i = 0; i < pattern.size(); i++) {
		isTrueClause = evaluatePattern(pattern[i]);
		if (!isTrueClause) {
			list<string> emptyResult{ EMPTY_STRING };
			return emptyResult;
		}
	}
	return list<string>();
}

vector<Synonym> QueryEvaluator::getResults(){
	return this->results;
}

bool QueryEvaluator::evaluateSuchThat(Clauses clause) {

}

bool QueryEvaluator::evaluatePattern(Clauses clause) {
	
}