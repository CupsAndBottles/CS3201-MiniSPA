#pragma once
#include "QueryEvaluator.h"

const string EMPTY_STRING = "";
const int WILDCARD = -1;
const int NOT_FOUND = -1;

const int POSITION_FIRSTPARAM = 1;
const int POSITION_SECONDPARAM = 2;


const string RELATIONSHIP_CALLS = "Calls";
const string RELATIONSHIP_CALLST = "Calls*";
const string RELATIONSHIP_FOLLOWS = "Follows";
const string RELATIONSHIP_FOLLOWST = "Follows*";
const string RELATIONSHIP_MODIFIES = "Modifies";
const string RELATIONSHIP_PARENT = "Parent";
const string RELATIONSHIP_PARENTT = "Parent*";
const string RELATIONSHIP_USES = "Uses";
const string STRING_BOOLEAN = "BOOLEAN";
const string STRING_FALSE = "false";
const string STRING_TRUE = "true";

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
	vector<Clauses> with;
	list<string> result;
	list<string> emptyResult = {};
	bool isTrueClause;

	if (!tree.getIsValid()) { // variables not found in program
		return emptyResult;
	}

	suchThat = tree.getSuchThatTree();
	pattern = tree.getPatternTree();
	with = tree.getWithTree();
	select = tree.getResultTree();

	for (size_t i = 0; i < suchThat.size(); i++) {
		isTrueClause = evaluateSuchThat(suchThat[i]);
		if (!isTrueClause) {
			if (select.at(0).getParentStringVal() == STRING_BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			return emptyResult;
		}
	}

	for (size_t i = 0; i < pattern.size(); i++) {
		isTrueClause = evaluatePattern(pattern[i]);
		if (!isTrueClause) {
			if (select.at(0).getParentStringVal() == STRING_BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			return emptyResult;
		}
	}

	for (size_t i = 0; i < with.size(); i++) {
		isTrueClause = evaluateWith(with[i]);
		if (!isTrueClause) {
			if (select.at(0).getParentStringVal() == STRING_BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			return emptyResult;
		}
	}

	if (select.at(0).getParentStringVal() == STRING_BOOLEAN) {
		list<string> trueResult = { STRING_TRUE };
		return trueResult;
	}

	printResults();
	sort(this->results.begin(), this->results.end());
	cout << "after sorting" << endl;
	printResults();

	vector<vector<int>> syn = groupSynonym(this->results);
	
	cout << "after grouping" << endl;

	for (size_t i = 0; i < syn.size(); i++) {
		for (size_t j = 0; j < syn[i].size(); j++) {
			cout << syn[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	vector<vector<int>> synGroup = rearrangeSynonym(syn);
	
	cout << "after rearranging" << endl;

	for (size_t i = 0; i < synGroup.size(); i++) {
		for (size_t j = 0; j < synGroup[i].size(); j++) {
			cout << synGroup[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Before merging" << endl;

	vector<Synonym> afterMerging = mergeWithinGroup(synGroup);

	cout << "After merging" << endl; 

	for (size_t i = 0; i < afterMerging.size(); i++) {
		afterMerging.at(i).printSyn();
	}

	result = evaluateSelect(afterMerging, select);

	return result;
}

vector<Synonym> QueryEvaluator::getResults() {
	return this->results;
}

/**************************** Such That Clauses ****************************/
bool QueryEvaluator::evaluateSuchThat(Clauses clause) {
	vector<pair<int, int>> results;
	int firstParamIndex = NOT_FOUND;
	int secondParamIndex = NOT_FOUND;

	bool hasValidFirstIndex = false;
	bool hasValidSecondIndex = false;

	Enum::TYPE firstParamType = clause.getLeftCType();
	Enum::TYPE secondParamType = clause.getRightCType();

	bool isGivenFirstParam = isGivenParam(clause, POSITION_FIRSTPARAM);
	bool isGivenSecondParam = isGivenParam(clause, POSITION_SECONDPARAM);

	if (isGivenFirstParam) {
		firstParamIndex = checkValidityOfEntities(clause, POSITION_FIRSTPARAM);
		hasValidFirstIndex = (firstParamIndex != NOT_FOUND);
	}
	else {
		hasValidFirstIndex = true;
	}

	if (isGivenSecondParam) {
		secondParamIndex = checkValidityOfEntities(clause, POSITION_SECONDPARAM);
		hasValidSecondIndex = (secondParamIndex != NOT_FOUND);
	}
	else {
		hasValidSecondIndex = true;
	}

	string relationship = clause.getParentStringVal();

	if (hasValidFirstIndex && hasValidSecondIndex) {
		if (relationship == RELATIONSHIP_CALLS) {
			results = this->pkb->getCalls(firstParamIndex, secondParamIndex);
		} else if (relationship == RELATIONSHIP_CALLST) {
		  results = this->pkb->getCallsT(firstParamIndex, secondParamIndex);
		}
		else if (relationship == RELATIONSHIP_FOLLOWS) {
			results = this->pkb->getFollows(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}
		else if (relationship == RELATIONSHIP_FOLLOWST) {
			results = this->pkb->getFollowsT(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}
		else if (relationship == RELATIONSHIP_MODIFIES) {
			results = this->pkb->getModifies(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}
		else if (relationship == RELATIONSHIP_PARENT) {
			results = this->pkb->getParent(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}
		else if (relationship == RELATIONSHIP_PARENTT) {
			results = this->pkb->getParentT(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}
		else if (relationship == RELATIONSHIP_USES) {
			results = this->pkb->getUses(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}
		/*	else if (relationship == RELATIONSHIP_NEXT) {
		results = this->pkb->getNext(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		} else if (relationship == RELATIONSHIP_NEXT*) {
		results = this->pkb->getNext*(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}*/
		else {

		}
	}

	if (!results.empty()) {
		storeResultsForSyn(clause, results);
		return true;
	}
	else {
		return false;
	}
}

bool QueryEvaluator::isGivenParam(Clauses clause, int paramPos) {
	if (paramPos == POSITION_FIRSTPARAM) {
		if (clause.getLeftCType() == Enum::TYPE::PROCEDURE) {
			if (clause.getLeftCIntValue() == NOT_FOUND) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			return (clause.getLeftCIntValue() != NOT_FOUND);
		}
	} else {
		if (clause.getRightCType() == Enum::TYPE::PROCEDURE || clause.getRightCType() == Enum::TYPE::VARIABLE) {
			if (clause.getRightCIntValue() == NOT_FOUND) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			return (clause.getRightCIntValue() != NOT_FOUND);
		}
	}
}

int QueryEvaluator::checkValidityOfEntities(Clauses clause, int paramPos) {
	if (paramPos == POSITION_FIRSTPARAM) {
		if (clause.getLeftCType() == Enum::TYPE::PROCEDURE || clause.getLeftCType() == Enum::TYPE::VARIABLE
			|| clause.getLeftCType() == Enum::TYPE::CALLS) {
			return checkValidityOfStringEntities(clause.getLeftCType(), clause.getLeftCStringValue());
		}
		else {
			return checkValidityOfIntEntities(clause.getLeftCType(), clause.getLeftCIntValue());
		}
	} else {
		if (clause.getRightCType() == Enum::TYPE::PROCEDURE || clause.getRightCType() == Enum::TYPE::VARIABLE
			|| clause.getRightCType() == Enum::TYPE::CALLS) {
			return checkValidityOfStringEntities(clause.getRightCType(), clause.getRightCStringValue());
		}
		else {
			return checkValidityOfIntEntities(clause.getRightCType(), clause.getRightCIntValue());
		}
	}
}

int QueryEvaluator::checkValidityOfStringEntities(Enum::TYPE type, string entityName) {
	switch (type) {
	case Enum::TYPE::PROCEDURE:
		return pkb->getProcIndex(entityName);
		break;
	case Enum::TYPE::VARIABLE:
		return pkb->getVarIndex(entityName);
		break;
	case Enum::TYPE::CALLS:
		return pkb->getProcIndex(entityName);
		break;
	default:
		return NOT_FOUND;
		break;
	}
}

int QueryEvaluator::checkValidityOfIntEntities(Enum::TYPE type, int entityInt) {
	switch (type) {
	case Enum::TYPE::STATEMENT:
		if (0 < entityInt && entityInt <= pkb->getNoOfStmt()) {
			return entityInt;
		}
		else {
			return NOT_FOUND;
		}
		break;
	case Enum::TYPE::ASSIGN:
		if (0 < entityInt && entityInt <= pkb->getNoOfStmt() && pkb->getType(entityInt) == Enum::TYPE::ASSIGN) {
			return entityInt;
		}
		else {
			return NOT_FOUND;
		}
		break;
	case Enum::TYPE::WHILE:
		if (0 < entityInt && entityInt <= pkb->getNoOfStmt() && pkb->getType(entityInt) == Enum::TYPE::WHILE) {
			return entityInt;
		}
		else {
			return NOT_FOUND;
		}
		break;
	case Enum::TYPE::IF:
		if (0 < entityInt && entityInt <= pkb->getNoOfStmt() && pkb->getType(entityInt) == Enum::TYPE::IF) {
			return entityInt;
		}
		else {
			return NOT_FOUND;
		}
		break;
	case Enum::TYPE::CALLS:
		if (0 < entityInt && entityInt <= pkb->getNoOfStmt() && pkb->getType(entityInt) == Enum::TYPE::CALLS) {
			return entityInt;
		}
		else {
			return NOT_FOUND;
		}
		break;
	case Enum::TYPE::CONSTANT:
		return pkb->getConstantIndex(entityInt) != NOT_FOUND;
		break;
	default:
		return NOT_FOUND;
		break;
	}
}

/**************************** Pattern Clauses ****************************/

bool QueryEvaluator::evaluatePattern(Clauses clause) {
	Enum::TYPE type = clause.getParentType();

	switch (type) {
	case Enum::TYPE::ASSIGN:
		return evaluateAssign(clause);
		break;
	case Enum::TYPE::IF:
		return evaluateIf(clause);
		break;
	case Enum::TYPE::WHILE:
		return evaluateWhile(clause);
		break;
	default:
		break;
	}
}

bool QueryEvaluator::evaluateWhile(Clauses clause) {
	vector<int> intermediateResult;
	int controlVariable = pkb->getVarIndex(clause.getLeftCStringValue());

	if (clause.getLeftCType() == Enum::TYPE::UNDERSCORE) {
		//pattern while(_, _)
		for (size_t i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::WHILE) {
				intermediateResult.push_back(i);
			}
		}
	}
	else if (controlVariable != NOT_FOUND) {
		//pattern while (x, _)
		for (size_t i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::WHILE) {
				if (pkb->getControlVar(i) == controlVariable) {
					intermediateResult.push_back(i);
				}
			}
		}
	}

	if (intermediateResult.size() != 0) {
		vector<Enum::TYPE> type = { clause.getParentType() };
		vector<string> syn = { clause.getParentStringVal() };
		vector<vector<int>> resultsToStore;
		resultsToStore.push_back(intermediateResult);
		storeResults(type, syn, resultsToStore);
		return true;
	}
	else {
		return false;
	}
}

bool QueryEvaluator::evaluateIf(Clauses clause) {
	vector<int> intermediateResult;
	int controlVariable = pkb->getVarIndex(clause.getLeftCStringValue());

	if (clause.getLeftCType() == Enum::TYPE::UNDERSCORE) {
		//pattern if(_, _, _)
		for (size_t i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::IF) {
				intermediateResult.push_back(i);
			}
		}
	}
	else if (controlVariable != NOT_FOUND) {
		//pattern if (x, _, _)
		for (size_t i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::IF) {
				if (pkb->getControlVar(i) == controlVariable) {
					intermediateResult.push_back(i);
				}
			}
		}
	}

	if (intermediateResult.size() != 0) {
		vector<Enum::TYPE> type = { clause.getParentType() };
		vector<string> syn = { clause.getParentStringVal() };
		vector<vector<int>> resultsToStore;
		resultsToStore.push_back(intermediateResult);
		storeResults(type, syn, resultsToStore);
		return true;
	}
	else {
		return false;
	}

}



bool QueryEvaluator::evaluateAssign(Clauses clause) {
	vector<int> intermediateResult;

	// if left child is underscore
	if (clause.getLeftCType() == Enum::TYPE::UNDERSCORE) {
		if (clause.getRightCType() == Enum::TYPE::UNDERSCORE) {
			return true;
		}
		else {
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			//cout << expr << endl;
			if (!clause.getRightCIsExpression()) {		
				// pattern a(_, x ) 
				for (int i = 1; i <= this->pkb->getNoOfStmt(); i++) {
					//cout << i << "." << endl;
					//cout << pkb->getRightExpr(i) << endl;
					if (this->pkb->getRightExpr(i) == expr)
						intermediateResult.push_back(i);
				}
			}
			else {		
				// pattern a(_, _x_)
				//cout << "here" << endl;
				// cout << this->pkb->getNoOfStmt() << endl;
				for (int i = 1; i <= this->pkb->getNoOfStmt(); i++) {
					if (this->pkb->getRightExpr(i).find(expr) != NOT_FOUND) {
						intermediateResult.push_back(i);
					}
					//cout << "there" << endl;
					//cout << i << endl;
					//cout << "RightExpr:" << pkb->getRightExpr(i) << endl;
				}
			}
		}
	}
	else { //left child is a variable
		int leftExpression = pkb->getVarIndex(clause.getLeftCStringValue());
		vector<pair<int, int>> stmtLst = this->pkb->getModifies(Enum::TYPE::ASSIGN, WILDCARD, Enum::TYPE::VARIABLE, leftExpression);
		//cout << clause.getLeftCStringValue() << endl;
		//cout << leftExpression << endl;
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
						//cout << i << endl;
						//cout << pkb->getRightExpr(i);
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
		vector<Enum::TYPE> type = { clause.getParentType() };
		vector<string> syn = { clause.getParentStringVal() };
		vector<vector<int>> resultsToStore;
		resultsToStore.push_back(intermediateResult);
		storeResults(type, syn, resultsToStore);
		return true;
	}
	else {
		return false;
	}
}

string QueryEvaluator::convertToShuntingYard(string statement) {
	
	string outputString;
	list<char> output;
	stack<char> stack;
	output.clear();
	string s;

	statement.erase(remove_if(statement.begin(), statement.end(), isspace), statement.end());

	for (char c : statement) {
		char charac = c;
		if (c == ';') {
			s = "";
		}
		if (c == '}') {
			//	break;
		}
		if (isOperator(charac))
		{
			char o1 = charac;
			s = "";
			output.push_back(' ');
			if (!stack.empty())
			{
				char o2 = stack.top();

				while (isOperator(o2) && isPriority(o2) >= isPriority(o1))
				{
					stack.pop();

					if (isOperator(o2)) {

						output.push_back(o2);
						output.push_back(' ');
					}
					else if (o2 != '}') {
						output.push_back(o2);
					}


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
				if (topCharac != '}') {
					output.push_back(topCharac);
					stack.pop();
				}

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
			}
		}
		else
		{
			if (charac == '=') {
				output.clear();
				s = "";

			}
			else {
				if (charac != '}') {
					output.push_back(charac);
					s.push_back(charac);
				}
			}
		}
	}
	while (!stack.empty())
	{
		char stackTop = stack.top();
		if (stackTop == ')' || stackTop == '(')
		{
		}
		if (stackTop != '}') {
			output.push_back(' ');
			output.push_back(stackTop);
			//output.push_back('');
		}
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
	if (o == '+' || o == '-' || o == '*') {
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

/**************************** With Clauses ****************************/
bool QueryEvaluator::evaluateWith(Clauses clause) {
	if (isGivenParam(clause, POSITION_SECONDPARAM)) {
		int entityIndex = checkValidityOfEntities(clause, POSITION_SECONDPARAM);

		if (entityIndex == NOT_FOUND) {
			return false;
		}
		else {
			vector<vector<int>> results = { {entityIndex} };
			vector<Enum::TYPE> type = { clause.getLeftCType() };
			vector<string> synString = { clause.getLeftCStringValue() };
			storeResults(type, synString, results);
			return true;
		}
	}
	else { // n = c.value / s/a/w/if.stmt# = c.value / v.varName/p.procName/call.procName = p.procName/v.varName/call.procName
		if (clause.getLeftCType() == Enum::TYPE::PROCEDURE || clause.getLeftCType() == Enum::TYPE::VARIABLE) {
			return hasSameAttrNames(clause);
		}
		else if (clause.getLeftCType() == Enum::TYPE::CALLS) {
			if (clause.getLeftCIsStmt()) {
				return evaluateValidityOfIntLeftRef(clause);
			}
			else {
				return hasSameAttrNames(clause);
			}
		}
		else {
			return evaluateValidityOfIntLeftRef(clause);
		}
	}
}

bool QueryEvaluator::evaluateValidityOfIntLeftRef(Clauses clause) {
	switch (clause.getLeftCType()) {
	case Enum::TYPE::CALLS:
		if (clause.getRightCType() == Enum::TYPE::STATEMENT || clause.getRightCType() == Enum::TYPE::CONSTANT
			|| clause.getRightCType() == Enum::TYPE::CALLS) {
			return hasSameAttrValues(clause);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::ASSIGN:
		if (clause.getRightCType() == Enum::TYPE::STATEMENT || clause.getRightCType() == Enum::TYPE::CONSTANT
			|| clause.getRightCType() == Enum::TYPE::ASSIGN) {
			return hasSameAttrValues(clause);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::STATEMENT:
		return hasSameAttrValues(clause);
		break;
	case Enum::TYPE::IF:
		if (clause.getRightCType() == Enum::TYPE::STATEMENT || clause.getRightCType() == Enum::TYPE::CONSTANT
			|| clause.getRightCType() == Enum::TYPE::IF) {
			return hasSameAttrValues(clause);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::WHILE:
		if (clause.getRightCType() == Enum::TYPE::STATEMENT || clause.getRightCType() == Enum::TYPE::CONSTANT
			|| clause.getRightCType() == Enum::TYPE::WHILE) {
			return hasSameAttrValues(clause);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::CONSTANT:
		return hasSameAttrValues(clause);
		break;
		/*	case Enum::TYPE::PROG_LINE:
		return hasSameAttrNum(clause);
		break; */
	default:
		return false;
		break;
	}
}

bool QueryEvaluator::hasSameAttrNames(Clauses clause) {
	vector<string> leftSynResults;
	vector<string> rightSynResults;

	leftSynResults = getAllAttrNames(clause.getLeftCType());
	rightSynResults = getAllAttrNames(clause.getRightCType());
	return getCommonAttrNames(leftSynResults, rightSynResults, clause);
}

bool QueryEvaluator::hasSameAttrValues(Clauses clause) {
	vector<int> leftSynResults;
	vector<int> rightSynResults;

	leftSynResults = getAllAttrValues(clause.getLeftCType());
	rightSynResults = getAllAttrValues(clause.getRightCType());
	return getCommonAttrValues(leftSynResults, rightSynResults, clause);
}

vector<string> QueryEvaluator::getAllAttrNames(Enum::TYPE type) {
	vector<string> allNames;
	switch (type) {
	case Enum::TYPE::PROCEDURE:
		for (int i = 0; i < pkb->getNoOfProc(); i++) {
			allNames.push_back(pkb->getProcName(i));
		}
		break;
	case Enum::TYPE::VARIABLE:
		for (int i = 0; i < pkb->getNoOfVar(); i++) {
			allNames.push_back(pkb->getVarName(i));
		}
		break;
	case Enum::TYPE::CALLS:
		for (int i = 0; i < pkb->getNoOfProc(); i++) {
			vector<int> procedureCalled = pkb->getProcCalls(i);
			for (size_t c = 0; c < procedureCalled.size(); c++) {
				allNames.push_back(pkb->getProcName(procedureCalled.at(c)));
			}
		}

		sort(allNames.begin(), allNames.end());
		allNames.erase(unique(allNames.begin(), allNames.end()), allNames.end());
		break;
	default:
		break;
	}

	return allNames;
}

vector<int> QueryEvaluator::getAllAttrValues(Enum::TYPE type) {
	vector<int> allValues;
	switch (type) {
	case Enum::TYPE::ASSIGN:
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::ASSIGN) {
				allValues.push_back(i);
			}
		}
		break;
	case Enum::TYPE::CONSTANT:
		for(int i = 0; i < pkb->getNoOfConstants(); i++) {
			allValues.push_back(pkb->getConstantValue(i));
		}
		break;
	case Enum::TYPE::IF:
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::IF) {
				allValues.push_back(i);
			}
		}
		break;
	case Enum::TYPE::STATEMENT:
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			allValues.push_back(i);
		}
		break;
	case Enum::TYPE::WHILE:
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::WHILE) {
				allValues.push_back(i);
			}
		}
		break;
	default:
		break;
	}

	return allValues;
}

bool QueryEvaluator::getCommonAttrNames(vector<string> leftResults, vector<string> rightResults, Clauses clause) {
	vector<string> mergedResults;
	for (size_t i = 0; i < leftResults.size(); i++) {
		for (size_t r = 0; r < rightResults.size(); r++) {
			if (leftResults[i] == rightResults[r]) {
				mergedResults.push_back(leftResults[i]);
			}
		}
	}

	vector<Enum::TYPE> type = { clause.getLeftCType(), clause.getRightCType() };
	vector<string> synString = { clause.getLeftCStringValue(), clause.getRightCStringValue() };
	if (mergedResults.empty()) {
		return false;
	}
	else {
		vector<int> resultsForLeftParam = convertNamesToIndexes(mergedResults, clause.getLeftCType());
		vector<int> resultsForRightParam = convertNamesToIndexes(mergedResults, clause.getRightCType());
		vector<vector<int>> resultsToStore = { resultsForLeftParam, resultsForRightParam };
		storeResults(type, synString, resultsToStore);
		return true;
	}
}

bool QueryEvaluator::getCommonAttrValues(vector<int> leftResults, vector<int> rightResults, Clauses clause) {
	vector<int> mergedResults;
	for (size_t i = 0; i < leftResults.size(); i++) {
		for (size_t r = 0; r < rightResults.size(); r++) {
			if (leftResults[i] == rightResults[r]) {
				mergedResults.push_back(leftResults[i]);
			}
		}
	}

	vector<Enum::TYPE> type = { clause.getLeftCType(), clause.getRightCType() };
	vector<string> synString = { clause.getLeftCStringValue(), clause.getRightCStringValue() };
	if (mergedResults.empty()) {
		return false;
	}
	else {
		vector<int> leftResults = mergedResults;
		vector<int> rightResults = mergedResults;

		if (clause.getLeftCType() == Enum::TYPE::CONSTANT) {
			for (size_t i = 0; i < mergedResults.size(); i++) {
				leftResults.push_back(pkb->getConstantIndex(mergedResults.at(i)));
			}
		}

		if (clause.getRightCType() == Enum::TYPE::CONSTANT) {
			for (size_t i = 0; i < mergedResults.size(); i++) {
				rightResults.push_back(pkb->getConstantIndex(mergedResults.at(i)));
			}
		}

		vector<vector<int>> resultsToStore = { leftResults, rightResults };
		storeResults(type, synString, resultsToStore);
		return true;
	}
}

vector<int> QueryEvaluator::convertNamesToIndexes(vector<string> stringResults, Enum::TYPE type) {
	vector<int> indexes;
	switch (type) {
	case Enum::TYPE::PROCEDURE:
		for (size_t i = 0; i < stringResults.size(); i++) {
			indexes.push_back(pkb->getProcIndex(stringResults[i]));
		}
		break;
	case Enum::TYPE::VARIABLE:
		for (size_t i = 0; i < stringResults.size(); i++) {
			indexes.push_back(pkb->getVarIndex(stringResults[i]));
		}
		break;
	case Enum::TYPE::CALLS:
		for (size_t i = 0; i < stringResults.size(); i++) {
			indexes.push_back(pkb->getProcIndex(stringResults[i]));
		}
		break;
	default:
		break;
	}

	return indexes;
}

/**************************** Storing and Converting Results To String ****************************/
void QueryEvaluator::storeResultsForSyn(Clauses clause, vector<pair<int, int>> results) {
	vector<int> firstSynResults;
	vector<int> secondSynResults;
	vector<Enum::TYPE> type;
	vector<string> synString;
	vector<vector<int>> resultsToStore;

	if (clause.getLeftCIntValue() == NOT_FOUND && clause.getLeftCType() != Enum::TYPE::UNDERSCORE) { // store only when syn is not given and not underscore
		for (size_t i = 0; i < results.size(); i++) {
			firstSynResults.push_back(results[i].first);
		}

		if (clause.getRightCIntValue() != NOT_FOUND) { // remove duplicates when syn is given
			sort(firstSynResults.begin(), firstSynResults.end());
			firstSynResults.erase(unique(firstSynResults.begin(), firstSynResults.end()), firstSynResults.end());
		}

		if (!firstSynResults.empty()) {
			type.push_back(clause.getLeftCType());
			synString.push_back(clause.getLeftCStringValue());
			resultsToStore.push_back(firstSynResults);
		}

	}

	if (clause.getRightCIntValue() == NOT_FOUND && clause.getRightCType() != Enum::TYPE::UNDERSCORE) { // store only when syn is not given and not underscore
		for (size_t i = 0; i < results.size(); i++) {
			secondSynResults.push_back(results[i].second);
		}

		if (clause.getLeftCIntValue() != NOT_FOUND) { // remove duplicates when syn is given
			sort(secondSynResults.begin(), secondSynResults.end());
			secondSynResults.erase(unique(secondSynResults.begin(), secondSynResults.end()), secondSynResults.end());
		}

		if (!secondSynResults.empty()) {
			type.push_back(clause.getRightCType());
			synString.push_back(clause.getRightCStringValue());
			resultsToStore.push_back(secondSynResults);
		}
	}

	storeResults(type, synString, resultsToStore);

}

void QueryEvaluator::storeResults(vector<Enum::TYPE> type, vector<string> synString, vector<vector<int>> resultToStore) {
	Synonym syn = Synonym();

	syn.addResult(type, synString, resultToStore);

	this->results.push_back(syn);
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
		return EMPTY_STRING;
		break;
	}

}

/**************************** Merging and Sorting Results ****************************/
vector<vector<int>> QueryEvaluator::rearrangeSynonym(vector<vector<int>> syn) {
	vector<vector<int>> result;

	for (size_t i = 0; i < syn.size(); i++) {
		result.push_back({ syn.at(i).at(0) });
		syn.at(i).erase(syn.at(i).begin());
	}

	for (size_t i = 0; i < syn.size(); i++) {
		for (size_t j = 0; j < syn.at(i).size(); j++) {
			if (hasCommonSyn(this->results.at(result.at(i).size() - 1), this->results.at(syn.at(i).at(j)))) {
				result.at(i).push_back(syn.at(i).at(j));
				syn.at(i).erase(syn.at(i).begin() + j);
				j = 0;
			}
		}
	}

	return result;

}

vector<Synonym> QueryEvaluator::mergeWithinGroup(vector<vector<int>> group) {
	vector<Synonym> mergedResult;
	Synonym syn;

	for (size_t i = 0; i < group.size(); i++) {
		if (this->results.empty()) {
			break;
		}

		syn = this->results.at(group[i][0]);
		for (size_t j = 1; j < group.at(i).size() ; j++) {
			if (group.at(i).size() != 1) {
				syn = mergeSyn(syn ,this->results.at(group[i][j]));
				cout << "mergeSyn" << endl;
			}
			else {
				syn = this->results.at(group[i][j]);
			}
		}
		mergedResult.push_back(syn);
		syn = Synonym();
	}

	return mergedResult;
}

/**
Precondition: syn2 is at most containing 2 synonyms
**/
Synonym QueryEvaluator::mergeSyn(Synonym syn1, Synonym syn2) {
	vector<Enum::TYPE> type1 = syn1.getType();
	vector<string> synName1 = syn1.getSyn();
	vector<vector<int>> result1 = syn1.getResult();
	vector<Enum::TYPE> type2 = syn2.getType();
	vector<string> synName2 = syn2.getSyn();
	vector<vector<int>> result2 = syn2.getResult();

	vector<int> empty;

	vector<Enum::TYPE> resultSynType;
	vector<string> resultSynName;
	vector<vector<int>> result;

	vector<pair<int, int>> counter = checkCommonSyn(type1, type2, synName1, synName2);

	if (counter.size() == 2) {
		int syn1row1 = counter.at(0).first;
		int syn2row1 = counter.at(0).second;
		int syn1row2 = counter.at(1).first;
		int syn2row2 = counter.at(1).second;

		for (size_t i = 0; i < result1.size(); i++) {
			result.push_back(empty);
		}

		for (size_t i = 0; i < result1[syn1row1].size(); i++) {
			for (size_t j = 0; j < result2[syn2row1].size(); j++) {
						if (result1[syn1row1][i] == result2[syn2row1][j]) {
							if (result1[syn1row2][i] == result2[syn2row2][j]) {
								for (size_t k = 0; k < result1.size(); k++) {
									cout << result1[k][i] << endl;
									result.at(k).push_back(result1[k][i]); // copy entire column
								}
						}
				}
			}
		}

		for (size_t k = 0; k < result1.size(); k++) {
			cout << "add type 1" << endl;
			resultSynType.push_back(type1[k]);
			resultSynName.push_back(synName1[k]);
		}

	}
	else if (counter.size() == 1) {
		int row1 = counter[0].first;
		int row2 = counter[0].second;

		cout << result1[row1].size() << endl;
		cout << result2[row2].size() << endl;

		// initialize rows1
		size_t numRow1 = 0;
		while (numRow1 < (result1.size())) {
			result.push_back(empty);
			numRow1++;
		}

		//initialize rows2
		size_t numRow2 = 0;
		while (numRow2 < (result2.size() - 1)) {
			result.push_back(empty);
			numRow2++;
		}
		cout << "result size is " << result.size() << endl;

		for (size_t i = 0; i < result1[row1].size(); i++) {
			for (size_t j = 0; j < result2[row2].size(); j++) {
				if (result1[row1][i] == result2[row2][j]) {
					cout << "equal" << endl;
					cout << result1.size() << endl;

					for (size_t k = 0; k < result1.size(); k++) {
						cout << result1[k][i] << endl;
						result.at(k).push_back(result1[k][i]); // copy entire column
					}

					int offset = 0;
					for (size_t k = 0; k < result2.size(); k++) {
						cout << "results 2" << endl;
						cout << result2.size() << endl;
						if (k != row2) {
							result.at(k + numRow1 - offset).push_back(result2[k][j]);
							cout << result2[k][j] << endl;
						}
						else {
							offset = offset + 1;
						}
					}
				}
			}
		}
		
		cout << "done" << endl;

		for (size_t i = 0; i < result.size(); i++) {
			for (size_t j = 0; j < result.at(i).size(); j++) {
				cout << "huh" << endl;
				cout << result[i][j] << " ";
				}
			cout << endl;
		}

		for (size_t k = 0; k < result1.size(); k++) {
			cout << "add type 1" << endl;
			resultSynType.push_back(type1[k]);
			resultSynName.push_back(synName1[k]);
		}
		for (size_t k = 0; k < result2.size(); k++) {
			if (k != row2) {
				resultSynType.push_back(type2[k]);
				resultSynName.push_back(synName2[k]);
			}
		}
	}

	Synonym syn;
	syn.addResult(resultSynType, resultSynName, result);

	return syn;


}

vector<pair<int, int>> QueryEvaluator::checkCommonSyn(vector<Enum::TYPE> type1, vector<Enum::TYPE> type2, vector<string> synName1, vector<string> synName2) {
	vector<pair<int, int>> counter;

	cout << "check common syn" << endl;

	for (size_t i = 0; i < type1.size(); i++) {
		for (size_t j = 0; j < type2.size(); j++) {
			cout << type1[i] << endl;
			cout << type2[j] << endl;
			if (type1[i] == type2[j]) {
				cout << synName1[i] << endl;
				cout << synName2[j] << endl;
				if (synName1[i] == synName2[j]) {
					cout << "true" << endl;
					counter.push_back(make_pair(i, j));
				}
			}
		}
	}

	cout << "finish checking common syn" << endl;

	return counter;
}

vector<vector<int>> QueryEvaluator::groupSynonym(vector<Synonym> result) {
	vector<vector<int>> syn;
	bool isFound = false;
	vector<int> newRow = { 0 };
	syn.push_back(newRow);

	for (size_t i = 1; i < result.size(); i++) {
		for (size_t groupIndex = 0; groupIndex < syn.size(); groupIndex++) {
			for (size_t synIndex = 0; synIndex < syn.at(groupIndex).size(); synIndex++) {
				if (hasCommonSyn(result.at(syn.at(groupIndex).at(synIndex)), result.at(i))) {
					syn.at(groupIndex).push_back(i);
					isFound = true;
					break;
				}
			}
		}
		if (!isFound) {
			vector<int> newRow = { (int)i };
			syn.push_back(newRow);
			i++;
		}
		isFound = false;
	}

	for (size_t i = 0; i < syn.size(); i++) {
		for (size_t j = i + 1; j < syn.size(); j++) {
			if (hasCommonSyn(syn.at(i), syn.at(j))) {
				syn = mergeSyn(syn, i, j);
				i = 0;
				break;
			}
		}
	}

	return syn;
}

vector<vector<int>> QueryEvaluator::mergeSyn(vector<vector<int>> syn, int first, int second) {
	for (size_t i = 0; i < syn.at(second).size(); i++) {
		syn.at(first).push_back(syn.at(second).at(i));
	}
	// Need to minus 1?
	syn.erase(syn.begin() + first);

	return syn;
}

bool QueryEvaluator::hasCommonSyn(vector<int> syn1, vector<int> syn2) {
	for (size_t i = 0; i < syn1.size(); i++) {
		for (size_t j = 0; j < syn2.size(); j++) {
			if (hasCommonSyn(this->results.at(syn1.at(i)), this->results.at(syn2.at(i)))) {
				return true;
			}
		}
	}

	return false;
}

bool QueryEvaluator::hasCommonSyn(Synonym syn1, Synonym syn2) {
	vector<Enum::TYPE> type1 = syn1.getType();
	vector<Enum::TYPE> type2 = syn2.getType();
	vector<string> synName1 = syn1.getSyn();
	vector<string> synName2 = syn2.getSyn();

	for (size_t i = 0; i < type1.size(); i++) {
		for (size_t j = 0; j < type2.size();j++){
			if (type1.at(i) == type2.at(j)) {
				if (synName1.at(i) == synName2.at(j)) {
					return true;
				}
			}
		}
	}

	return false;
}

list<string> QueryEvaluator::evaluateSelect(vector<Synonym> groupedSyns, vector<Clauses> select) {
	list<string> stringedResults;
	nonCommonSyn = select;

	vector<pair<string, vector<int>>> mergedSelectedSyns = getValuesOfSelectedSyns(groupedSyns, select);

	if (!this->nonCommonSyn.empty()) {
		vector<pair<string, vector<int>>> nonCommonSyn = getValuesOfNonCommonSyn(this->nonCommonSyn);

		for (size_t syn = 0; syn < nonCommonSyn.size(); syn++) {
			mergedSelectedSyns = mergeSelectedSyns(mergedSelectedSyns, nonCommonSyn[syn]);
		}
	}

	vector<pair<Enum::TYPE, vector<int>>> arrangedSyns = rearrangeSynOrder(mergedSelectedSyns, select);
	return convertResultsToString(arrangedSyns);
}

vector<pair<string, vector<int>>> QueryEvaluator::getValuesOfNonCommonSyn(vector<Clauses> nonCommon) {
	vector<pair<string, vector<int>>> valuesOfNonCommonSyn;

	for (size_t syn = 0; syn < nonCommon.size(); syn++) {
		Enum::TYPE typeOfSyn = nonCommon[syn].getParentType();

		if (typeOfSyn == Enum::TYPE::VARIABLE || typeOfSyn == Enum::TYPE::PROCEDURE || typeOfSyn == Enum::TYPE::CALLS) {
			valuesOfNonCommonSyn.push_back(make_pair(nonCommon[syn].getParentStringVal(), getStringedAttrIndexes(typeOfSyn)));
		}
		else {
			valuesOfNonCommonSyn.push_back(make_pair(nonCommon[syn].getParentStringVal(), getAllAttrValues(typeOfSyn)));
		}
	}
	return valuesOfNonCommonSyn;
}

vector<int> QueryEvaluator::getStringedAttrIndexes(Enum::TYPE type) {
	vector<int> stringedAttrIndexes;
	switch (type) {
	case Enum::TYPE::PROCEDURE:
		for (int i = 0; i < pkb->getNoOfProc(); i++) {
			stringedAttrIndexes.push_back(i);
		}
		break;
	case Enum::TYPE::VARIABLE:
		for (int i = 0; i < pkb->getNoOfVar(); i++) {
			stringedAttrIndexes.push_back(i);
		}
		break;
	case Enum::TYPE::CALLS:
		for (int i = 0; i < pkb->getNoOfProc(); i++) {
			vector<int> procedureCalled = pkb->getProcCalls(i);
			for (size_t c = 0; c < procedureCalled.size(); c++) {
				stringedAttrIndexes.push_back(procedureCalled.at(c));
			}
		}

		sort(stringedAttrIndexes.begin(), stringedAttrIndexes.end());
		stringedAttrIndexes.erase(unique(stringedAttrIndexes.begin(), stringedAttrIndexes.end()), stringedAttrIndexes.end());
		break;
	default:
		break;
	}

	return stringedAttrIndexes;
}

vector<pair<Enum::TYPE, vector<int>>> QueryEvaluator::rearrangeSynOrder(vector<pair<string, vector<int>>> mergedSelectedSyns, vector<Clauses> select) {
	vector<pair<Enum::TYPE, vector<int>>> arrangedSynValues;

	for (size_t syn = 0; syn < select.size(); syn++) {
		for (size_t mergedSyn = 0; mergedSyn < mergedSelectedSyns.size(); mergedSyn++) {
			if (mergedSelectedSyns[mergedSyn].first == select[syn].getParentStringVal()) {
				arrangedSynValues.push_back(make_pair(select[syn].getParentType(), mergedSelectedSyns[mergedSyn].second));
				break;
			}
		}
	}

	return arrangedSynValues;
}

list<string> QueryEvaluator::convertResultsToString(vector<pair<Enum::TYPE, vector<int>>> arrangedSyns) {
	list<string> stringedResults;
	int numOfValuesPerSyn = arrangedSyns.at(0).second.size();

	for (int values = 0; values < numOfValuesPerSyn; values++) {
		vector<int> valuesOfSyn = arrangedSyns[0].second;
		string combinedValues = convertToString(valuesOfSyn.at(values), arrangedSyns.at(0).first);
		for (size_t syn = 1; syn < arrangedSyns.size(); syn++) {
			combinedValues = combinedValues + " " + convertToString(arrangedSyns[syn].second.at(values), arrangedSyns[syn].first);
		}

		stringedResults.push_back(combinedValues);
	}
	
	return stringedResults;
}

vector<pair<string, vector<int>>> QueryEvaluator::getValuesOfSelectedSyns(vector<Synonym> groupedSyns, vector<Clauses> select) {
	vector<pair<string, vector<int>>> mergedValues;

	for (size_t groupIndex = 0; groupIndex < groupedSyns.size(); groupIndex++) { // in one group
		vector<string> synsInAGroup = groupedSyns[groupIndex].getSyn();
		vector<Enum::TYPE> typesInAGroup = groupedSyns[groupIndex].getType();
		vector<vector<int>> synValuesInAGroup = groupedSyns[groupIndex].getResult();

		int firstGroupIndexAdded = -1;
		for (size_t groupPos = 0; groupPos < synsInAGroup.size(); groupPos++) { // a syn in a group
			for (size_t syn = 0; syn < nonCommonSyn.size(); syn++) { // a selected syn from select tree
				if (synsInAGroup[groupPos] == nonCommonSyn.at(syn).getParentStringVal() && typesInAGroup[groupPos] == nonCommonSyn.at(syn).getParentType()) {
					if (firstGroupIndexAdded == -1 || firstGroupIndexAdded == groupIndex) {
						firstGroupIndexAdded = groupIndex;
						mergedValues.push_back(make_pair(synsInAGroup[groupPos], synValuesInAGroup[groupPos]));
					}
					else {
						mergedValues = mergeSelectedSyns(mergedValues, make_pair(synsInAGroup[groupPos], synValuesInAGroup[groupPos]));
					}

					nonCommonSyn.erase(nonCommonSyn.begin() + syn);
					break;
				}
			}
		}
	}

	return mergedValues;
}

vector<pair<string, vector<int>>> QueryEvaluator::mergeSelectedSyns(vector<pair<string, vector<int>>> mergedValues, pair<string, vector<int>> toBeMerged) {
	vector<pair<string, vector<int>>> newMergedGroup;
	vector<vector<int>> newMergedValues;
	vector<int> valuesFromToBeMerged = toBeMerged.second;
	int newGroupSize = mergedValues.size() + 1;

	if (mergedValues.empty()) {
		newMergedGroup.push_back(toBeMerged);
		return newMergedGroup;
	} 

	newMergedValues.assign(newGroupSize, vector<int>());
	for (size_t values = 0; values < valuesFromToBeMerged.size(); values++) {
		for (size_t valueFromMerged = 0; valueFromMerged < mergedValues.front().second.size(); valueFromMerged++) {

			for (size_t syn = 0; syn < mergedValues.size(); syn++) {
				vector<int> valuesOfSyn = mergedValues[syn].second;
				newMergedValues[syn].push_back(valuesOfSyn.at(valueFromMerged));
			}
			newMergedValues[mergedValues.size()].push_back(valuesFromToBeMerged[values]);
		}
	}

	for (int i = 0; i < newGroupSize; i++) {
		if (i == mergedValues.size()) {
			newMergedGroup.push_back(make_pair(toBeMerged.first, newMergedValues[i]));
		}
		else {
			newMergedGroup.push_back(make_pair(mergedValues[i].first, newMergedValues[i]));
		}
	}

	return newMergedGroup;
}

void QueryEvaluator::printResults() {
	for (size_t i = 0; i < this->results.size(); i++) {
		results[i].printSyn();
	}
}




