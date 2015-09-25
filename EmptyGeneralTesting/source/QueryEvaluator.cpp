#pragma once
#include "QueryEvaluator.h"

const string EMPTY_STRING = "";
const int WILDCARD = -1;
const int NOT_FOUND = -1;
const int UNDERSCORE = -2;

const string RELATIONSHIP_CALLS = "Calls";
const string RELATIONSHIP_FOLLOWS = "Follows";
const string RELATIONSHIP_FOLLOWST = "Follows*";
const string RELATIONSHIP_MODIFIES = "Modifies";
const string RELATIONSHIP_PARENT = "Parent";
const string RELATIONSHIP_PARENTT = "Parent*";
const string RELATIONSHIP_USES = "Uses";

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
	vector<pair<int, int>> results;
	string relationship = clause.getParentStringVal();
	Details firstParam = clause.getLeftChild();
	Details secondParam = clause.getRightChild();

	int indexForFirstParam = firstParam.getIntValue();
	int indexForSecondParam = secondParam.getIntValue();

	if (indexForFirstParam == UNDERSCORE) {
		indexForFirstParam = WILDCARD;
	}

	if (indexForSecondParam == UNDERSCORE) {
		indexForSecondParam = WILDCARD;
	}

	if (relationship == RELATIONSHIP_CALLS) {
		results = this->pkb->getCalls(indexForFirstParam, indexForSecondParam);
	}
	else if (relationship == RELATIONSHIP_FOLLOWS) {
		results = this->pkb->getFollows(firstParam.getType(), indexForFirstParam, secondParam.getType(), indexForSecondParam);
	}
	else if (relationship == RELATIONSHIP_FOLLOWST) {
		results = this->pkb->getFollowsT(firstParam.getType(), indexForFirstParam, secondParam.getType(), indexForSecondParam);
	}
	else if (relationship == RELATIONSHIP_MODIFIES) {
		results = this->pkb->getModifies(firstParam.getType(), indexForFirstParam, secondParam.getType(), indexForSecondParam);
	}
	else if (relationship == RELATIONSHIP_PARENT) {
		results = this->pkb->getParent(firstParam.getType(), indexForFirstParam, secondParam.getType(), indexForSecondParam);
	}
	else if (relationship == RELATIONSHIP_PARENTT) {
		results = this->pkb->getParentT(firstParam.getType(), indexForFirstParam, secondParam.getType(), indexForSecondParam);
	}
	else if (relationship == RELATIONSHIP_USES) {
		results = this->pkb->getUses(firstParam.getType(), indexForFirstParam, secondParam.getType(), indexForSecondParam);
	}
	else {

	}

	storeResultsForSyn(clause, results);

	if (!results.empty()) {
		return true;
	}
	else {
		return false;
	}
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
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			if (!clause.getRightChild().getIsExpression) {		// pattern a(_, x ) 
				for (int i = 1; i < this->pkb->getNumStmt(); i++) {
					if (this->pkb->getRightExpr(i) == expr)
						intermediateResult.push_back(i);
				}
			}
			else {		// pattern a(_, _x_)
				for (int i = 1; i < this->pkb->getNumStmt(); i++) {
					if (this->pkb->getRightExpr(i).find(expr) != NOT_FOUND) {
						intermediateResult.push_back(i);
					}
				}
			}
		}
	}
	else { //left child is a variable
		vector<pair<int,int>> stmtLst = this->pkb.getModifies(TYPE::ASSIGN, WILDCARD , TYPE::VARIABLE, clause.getLeftChild().getIntValue());
		if (clause.getRightCType() == TYPE::UNDERSCORE) { // a(v, _)
			for (int i = 0; i < stmtLst.size(); i++) {
				intermediateResult.push_back(stmtLst[i].first);
			}
		}
		else {
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			if (!clause.getRightCIsExpression()) { // a(v, x + y)
				for (int i = 0; i < stmtLst.size(); i++) {  
					if (this->pkb->getRightExpr(stmtLst[i].first) == expr) {
						intermediateResult.push_back(stmtLst[i].first);
					}
				}
			}
			else { // a(v, _x+y_)
				for (int i = 0; i < stmtLst.size(); i++) {
					if (this->pkb->getRightExpr(i).find(expr) != NOT_FOUND) {
						intermediateResult.push_back(stmtLst[i].first);
					}
				}
			}
		}
	}
	
	if (intermediateResult.size() != 0) {
		storeResults(intermediateResult, clause.getParentStringVal, TYPE::ASSIGN);
		return true;
	}
	else {
		return false;
	}
}

void QueryEvaluator::storeResultsForSyn(Clauses clause, vector<pair<int, int>> results) {
	vector<int> firstSynResults;
	vector<int> secondSynResults;

	Details firstParam = clause.getLeftChild();
	Details secondParam = clause.getRightChild();

	if (firstParam.getIntValue == WILDCARD) {
		for (vector<pair<int, int>>::iterator it = results.begin(); it != results.end(); it++) {
			firstSynResults.push_back(it->first);
		}
		storeResults(firstSynResults, firstParam.getStringValue(), firstParam.getType());
	}

	if (secondParam.getIntValue == WILDCARD) {
		for (vector<pair<int, int>>::iterator it = results.begin(); it != results.end(); it++) {
			secondSynResults.push_back(it->second);
		}
		storeResults(secondSynResults, secondParam.getStringValue(), secondParam.getType());
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

string QueryEvaluator::convertToShuntingYard(string statement) {
	//modify and uses - modify => a = b+c, a is modified. Uses= b and c
	list<char> output;
	stack<char> stack;
	output.clear();
	string outputString;

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
		outputString = outputString + stackTop;
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