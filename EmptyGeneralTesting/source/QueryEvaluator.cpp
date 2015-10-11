#pragma once
#include "QueryEvaluator.h"

const string EMPTY_STRING = "";
const int WILDCARD = -1;
const int NOT_FOUND = -1;

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

QueryEvaluator::QueryEvaluator(PKB &inputPKB) {
	this->pkb = &inputPKB;
	
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
	vector<vector<string>> intermediateResult;
	list<string> result;
	bool isTrueClause;

	if (!tree.getIsValid()) { // variables not found in program
		list<string> emptyResult{ EMPTY_STRING };
		return emptyResult;
	}

	suchThat = tree.getSuchThatTree();
	pattern = tree.getPatternTree();
	select = tree.getResultTree();

	for (size_t i = 0; i < suchThat.size(); i++) {
		isTrueClause = evaluateSuchThat(suchThat[i]);
		if (!isTrueClause) {
			list<string> emptyResult{ EMPTY_STRING };
			return emptyResult;
		}
	}

	for (size_t i = 0; i < pattern.size(); i++) {
		isTrueClause = evaluatePattern(pattern[i]);
		if (!isTrueClause) {
			list<string> emptyResult{ EMPTY_STRING };
			return emptyResult;
		}
	}

	for (size_t i = 0; i < select.size(); i++) {
		intermediateResult.push_back(evaluateSelect(select[i]));
	}

	result = permutateResult(intermediateResult);

	return result;
}

list<string> QueryEvaluator::permutateResult(vector<vector<string>> intermediateResult) {
	list<string> printedResults;

	printedResults = permutateResultSubset(intermediateResult);

	return printedResults;
}

vector<string> QueryEvaluator::permutateResultPair(vector<string> firstSet, vector<string> secondSet) {
	string toBeDisplayed = string();
	vector<string> mergedPair;

	for (size_t i = 0; i < firstSet.size(); i++) {
		for (size_t j = 0; j < secondSet.size(); j++) {
			toBeDisplayed = string();
			toBeDisplayed = firstSet.at(i) + ", " + secondSet.at(j);
			mergedPair.push_back(toBeDisplayed);
		}
	}

	return mergedPair;
}

list<string> QueryEvaluator::permutateResultSubset(vector<vector<string>> intermediateResult) {
	list<string> stringedResults;

	if (intermediateResult.size() == 0) {
		return stringedResults;
	}
	if (intermediateResult.size() == 1) {
		stringedResults = convertVectorToList(intermediateResult.front());
		return stringedResults;
	}
	else {
		int lastSetToMerge;
		int numSyn = intermediateResult.size();
		bool isOdd = false;
		vector<vector<string>> mergedResults;

		if (numSyn % 2 == 1) {
			isOdd = true;
		}
		else {
			isOdd = false;
		}

		if (isOdd) {
			lastSetToMerge = intermediateResult.size() - 2; // leave the last set out.
		}
		else {
			lastSetToMerge = intermediateResult.size() - 1;
		}

		for (int i = 0; i < lastSetToMerge; i+=2) {
			mergedResults.push_back(permutateResultPair(intermediateResult.at(i), intermediateResult.at(i + 1)));
		}


		if (isOdd) {
			mergedResults.push_back(intermediateResult.back()); // will terminate, eventually it will be 3 sets (if more than 3) which merge to become 1.
		}

		return permutateResultSubset(mergedResults);
	}
}

list<string> QueryEvaluator::convertVectorToList(vector<string> mergedResults) {
	list<string> listedResults;

	for (size_t i = 0; i < mergedResults.size(); i++) {
		listedResults.push_back(mergedResults.at(i));
	}

	return listedResults;
}

vector<string> QueryEvaluator::evaluateSelect(Clauses select) {
	vector<string> resultForSyn;
	bool hasCommonSyn = false;

	string synonym = select.getParentStringVal();
	Enum::TYPE type = select.getParent().getType();

	for (size_t i = 0; i < this->results.size(); i++) {
		if ((results[i].getSyn() == synonym) && (results[i].getType() == type)) {
			hasCommonSyn = true;
			for (size_t j = 0; j < results[i].getResult().size(); j++) {
				resultForSyn.push_back(convertToString(results[i].getResult().at(j), type));
			}
		}
	}
	
	if (!hasCommonSyn) {
		switch (type) {
		case Enum::TYPE::STATEMENT:
			for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
				resultForSyn.push_back(to_string(i));
			}
			break;
		case Enum::TYPE::ASSIGN:
			for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
				if (pkb->getType(i) == Enum::TYPE::ASSIGN) {
					resultForSyn.push_back(to_string(i));
				}
			}
			break;
		case Enum::TYPE::WHILE:
			for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
				if (pkb->getType(i) == Enum::TYPE::WHILE) {
					resultForSyn.push_back(to_string(i));
				}
			}
			break;
		case Enum::TYPE::PROCEDURE:
			for (int i = 0; i < pkb->getNoOfProc(); i++) {
				resultForSyn.push_back(pkb->getProcName(i));
			}
			break;
		case Enum::TYPE::VARIABLE:
			for (int i = 0; i < pkb->getNoOfVar(); i++) {
				resultForSyn.push_back(pkb->getVarName(i));
			}
		default:
			break;
		}
	}

	return resultForSyn;
}

string QueryEvaluator::convertToString(int index, Enum::TYPE type) {

	// No UNDERSCORE TYPE / CONSTANT
	switch (type) {
	case Enum::TYPE::ASSIGN:
		return to_string(index);
		break;
	case Enum::TYPE::STATEMENT:
		return to_string(index);
		break;
	case Enum::TYPE::PROCEDURE:
		return this->pkb->getProcName(index);
		break;
	case Enum::TYPE::WHILE:
		return to_string(index);
		break;
	case Enum::TYPE::IF:
		return to_string(index);
		break;
	case Enum::TYPE::VARIABLE:
		return this->pkb->getVarName(index);
		break;
	case Enum::TYPE::CALLS:
		return to_string(index);
		break;
	default:
		cout << "Convert to String, no TYPE matches" << endl;
	}

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

	if (indexForFirstParam == Enum::TYPE::UNDERSCORE) {
		indexForFirstParam = WILDCARD;
	}

	if (indexForSecondParam == Enum::TYPE::UNDERSCORE) {
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
	Enum::TYPE type = clause.getParentType();
	
	switch(type) {
	case Enum::TYPE::ASSIGN : 
		return evaluateAssign(clause);
		break;
	case Enum::TYPE::IF :
		break;
	case Enum::TYPE::WHILE :
		break;
	default:
		break;
	}
}

bool QueryEvaluator::evaluateAssign(Clauses clause) {
	vector<int> intermediateResult;
	
	// if left child is underscore
	if (clause.getLeftChild().getType() == Enum::TYPE::UNDERSCORE) {
		if (clause.getRightChild().getType() == Enum::TYPE::UNDERSCORE) {
			return true;
		}
		else{ 			
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			if (!clause.getRightChild().getIsExpression()) {		// pattern a(_, x ) 
				for (int i = 1; i < this->pkb->getNoOfStmt(); i++) {
					if (this->pkb->getRightExpr(i) == expr)
						intermediateResult.push_back(i);
				}
			}
			else {		// pattern a(_, _x_)
				for (int i = 1; i < this->pkb->getNoOfStmt(); i++) {
					if (this->pkb->getRightExpr(i).find(expr) != NOT_FOUND) {
						intermediateResult.push_back(i);
					}
				}
			}
		}
	}
	else { //left child is a variable
		vector<pair<int,int>> stmtLst = this->pkb->getModifies(Enum::TYPE::ASSIGN, WILDCARD , Enum::TYPE::VARIABLE, clause.getLeftChild().getIntValue());
		if (clause.getRightCType() == Enum::TYPE::UNDERSCORE) { // a(v, _)
			for (size_t i = 0; i < stmtLst.size(); i++) {
					intermediateResult.push_back(stmtLst[i].first);
			}
		}
		else {
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			if (!clause.getRightCIsExpression()) { // a(v, x + y)
				for (size_t i = 0; i < stmtLst.size(); i++) {  
					if (this->pkb->getRightExpr(stmtLst[i].first) == expr) {
							intermediateResult.push_back(stmtLst[i].first);
					}
				}
			}
			else { // a(v, _x+y_)
				for (size_t i = 0; i < stmtLst.size(); i++) {
					if (this->pkb->getRightExpr(stmtLst[i].first).find(expr) != NOT_FOUND) {
							intermediateResult.push_back(stmtLst[i].first);
					}
				}
			}
		}
	}
	
	if (intermediateResult.size() != 0) {
		storeResults(intermediateResult, clause.getParentStringVal(), Enum::TYPE::ASSIGN);
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

	if (firstParam.getIntValue() == WILDCARD) {
		for (size_t i = 0; i < results.size(); i++) {
			firstSynResults.push_back(results[i].first);
		}

		sort(firstSynResults.begin(), firstSynResults.end());
		firstSynResults.erase(unique(firstSynResults.begin(), firstSynResults.end()), firstSynResults.end());
		storeResults(firstSynResults, firstParam.getStringValue(), firstParam.getType());
	}

	if (secondParam.getIntValue() == WILDCARD) {
		for (size_t i = 0; i < results.size(); i++) {
			secondSynResults.push_back(results[i].second);
		}
		sort(secondSynResults.begin(), secondSynResults.end());
		secondSynResults.erase(unique(secondSynResults.begin(), secondSynResults.end()), secondSynResults.end());
		storeResults(secondSynResults, secondParam.getStringValue(), secondParam.getType());
	}
}

void QueryEvaluator::storeResults(vector<int> intermediateResult, string syn, Enum::TYPE type) {
	bool isPresentInResults = false;

	for (size_t i = 0; i < this->results.size(); i++) {
		// Syn found in vector<Synonym> results
		if (this->results[i].getSyn() == syn) {
			isPresentInResults = true;
			results[i].addResult(intermediateResult);
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
	string s = "";
	output.clear();
	string outputString;

	statement.erase(remove_if(statement.begin(), statement.end(), isspace), statement.end());

	for (char c : statement) {
		char charac = c;
		if (c == ';') {
			//addToParent(index);
		}
		if (c == '}') {

			//pushCloseBracket(index);
			break;
		}
		if (isOperator(charac))
		{
			char o1 = charac;

			if (!stack.empty())
			{
				char o2 = stack.top();

				while (isOperator(o2) && isPriority(o2) >= isPriority(o1))
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
				cout << "error";
			}
		}
		else
		{
			if (charac == '=') {
				//output.pop_back();

			}
			else {
				output.push_back(charac);
				s = "" + charac;
			}
		}
	}
	while (!stack.empty())
	{
		char stackTop = stack.top();
		if (stackTop == ')' || stackTop == '(')
		{
			//Error();
		}
		output.push_back(stackTop);
		stack.pop();
	}

	for (list<char>::iterator it = output.begin(); it != output.end(); ++it) {

		outputString.push_back(*it);
	}
	return outputString;
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