#pragma once
#include "QueryEvaluator.h"

const string EMPTY_STRING = "";
const int WILDCARD = -1;

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

	if (!tree.getIsValid()) { // variables not found in program
		list<string> emptyResult{ EMPTY_STRING };
		return emptyResult;
	}

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
	TYPE type = clause.getParentType();
	
	switch(type) {
	case TYPE::ASSIGN : 
		return evaluateAssign(clause);
		break;
	case TYPE::IF :
		break;
	case TYPE::WHILE :
		break;
	default:
		break;
	}
}

bool QueryEvaluator::evaluateAssign(Clauses clause) {
	vector<int> intermediateResult;
	
	// if left child is underscore
	if (clause.getLeftChild().getType == TYPE::UNDERSCORE) {
		if (clause.getRightChild().getType == TYPE::UNDERSCORE) {
			return true;
		}
		else{ 
			// pattern a(_, x ) 
			if (clause.getRightChild().getIsExpression) {
				for (int i = 1; i < this->pkb->getNumStmt(); i++) {
					string expr = convertToShuntingYard(clause.getRightCStringValue());
					if (pkb->getRightExpr(i) == expr)
						intermediateResult.push_back(i);
				}
				storeResults(intermediateResult, clause.getParentStringVal, TYPE::ASSIGN);
			}
			else { // pattern a(_, _x_)

			}
		}
	}
}

void QueryEvaluator::storeResults(vector<int>& intermediateResult, string syn, TYPE type) {
	bool isPresentInResults = false;

	for (int i = 0; i < this->results.size(); i++) {
		// Syn found in vector<Synonym> results
		if (this->results[i].getSyn() == syn) {
			isPresentInResults = true;
			for (int j = 0; j < intermediateResult.size(); i++) {
				this->results[i].addResult(intermediateResult[i]);
			}
		}
	}

	// Syn not found in vector<Synonym> results
	if (!isPresentInResults) {
		this->results.push_back(Synonym(type, syn, intermediateResult));
	}

	cout << "storing results";

}

void QueryEvaluator::convertToShuntingYard(string statement) {
	//modify and uses - modify => a = b+c, a is modified. Uses= b and c
	list<char> output;
	stack<char> stack;
	output.clear();

	for (char c : statement) {
		char charac = c;
		
		if (isOperator(charac))
		{
			char o1 = charac;

			if (!stack.empty())
			{
				char o2 = stack.top();

				while (isOperator(o2) && (isPriority(o2) >= isPriority(o1)))
				{
					stack.pop();
					output.push_back(o2);

					if (!stack.empty())
						o2 = stack.top();
					else
						break;
				}
			}

			stack.push(o1);
		}
		else if (charac == '(')
		{
			stack.push(charac);
		}
		else if (charac == ')')
		{
			char topCharac = stack.top();

			while (topCharac != '(')
			{
				output.push_back(topCharac);
				stack.pop();

				if (stack.empty()) {
					break;
				}
				topCharac = stack.top();
			}

			if (!stack.empty()) {
				stack.pop();
			}
			if (topCharac != '(')
			{
				cout << "error (";
			}
		}
	}
	while (!stack.empty())
	{
		char stackTop = stack.top();
		if (stackTop == ')' || stackTop == '(')
		{
			cout << "error with brackets";
		}
		output.push_back(stackTop);
		stack.pop();
	}

}
//end of method

bool QueryEvaluator::isOperator(char o) {
	bool isOp = false;
	if (o == '+' || o == '-' || o == '/' || o == '*') {
		isOp = true;
	}
	return isOp;
}

int QueryEvaluator::isPriority(const char &c)
{
	if (c == '^')
	{
		return 3;
	}
	if (c == '*' || c == '/')
	{
		return 2;
	}
	if (c == '+' || c == '-')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}