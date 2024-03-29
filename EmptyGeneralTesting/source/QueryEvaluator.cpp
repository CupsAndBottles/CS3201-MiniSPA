#pragma once
#include "QueryEvaluator.h"

const string EMPTY_STRING = "";
const int WILDCARD = -1;
const int NOT_FOUND = -1;
const int IS_SYN = -1;
const int NOT_SYN = 0;

const int POSITION_FIRSTPARAM = 1;
const int POSITION_SECONDPARAM = 2;

const string RELATIONSHIP_AFFECTS = "Affects";
const string RELATIONSHIP_AFFECTST = "Affects*";
const string RELATIONSHIP_CALLS = "Calls";
const string RELATIONSHIP_CALLST = "Calls*";
const string RELATIONSHIP_FOLLOWS = "Follows";
const string RELATIONSHIP_FOLLOWST = "Follows*";
const string RELATIONSHIP_MODIFIES = "Modifies";
const string RELATIONSHIP_NEXT = "Next";
const string RELATIONSHIP_NEXTT = "Next*";
const string RELATIONSHIP_PARENT = "Parent";
const string RELATIONSHIP_PARENTT = "Parent*";
const string RELATIONSHIP_USES = "Uses";
const string STRING_BOOLEAN = "BOOLEAN";
const string STRING_FALSE = "false";
const string STRING_TRUE = "true";
const string STRiNG_UNDERSCORE = "_";

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
	bool isTrueClause = false;

	suchThat = tree.getSuchThatTree();
	pattern = tree.getPatternTree();
	with = tree.getWithTree();
	select = tree.getResultTree();

	for (size_t i = 0; i < suchThat.size(); i++) {
		isTrueClause = evaluateSuchThat(suchThat[i]);
		if (!isTrueClause) {
			if (select.at(0).getParentType() == Enum::TYPE::BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			return emptyResult;
		}
	}

	for (size_t i = 0; i < pattern.size(); i++) {
		isTrueClause = evaluatePattern(pattern[i]);
		if (!isTrueClause) {
			if (select.at(0).getParentType() == Enum::TYPE::BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			return emptyResult;
		}
	}

	for (size_t i = 0; i < with.size(); i++) {
		isTrueClause = evaluateWith(with[i]);
		if (!isTrueClause) {
			if (select.at(0).getParentType() == Enum::TYPE::BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			return emptyResult;
		}
	}

	if (select.at(0).getParentType() == Enum::TYPE::BOOLEAN) {
		if (results.empty() && isTrueClause) {
			return list<string>{STRING_TRUE};
		}
	}
//	printResults();
	sort(this->results.begin(), this->results.end());
//	cout << "after sorting" << endl;
//	printResults();

	vector<vector<int>> syn = groupSynonym(this->results);
	
/*	cout << "after grouping" << endl;

	for (size_t i = 0; i < syn.size(); i++) {
		for (size_t j = 0; j < syn[i].size(); j++) {
			cout << syn[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
*/
	vector<vector<int>> synGroup = rearrangeSynonym(syn);
	
//	cout << "after rearranging" << endl;

/*	for (size_t i = 0; i < synGroup.size(); i++) {
		for (size_t j = 0; j < synGroup[i].size(); j++) {
			cout << synGroup[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Before merging" << endl;
*/
	vector<Synonym> afterMerging = mergeWithinGroup(synGroup);

/*	cout << "After merging" << endl; 

	for (size_t i = 0; i < afterMerging.size(); i++) {
		afterMerging.at(i).printSyn();
	}
*/
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
		else if (relationship == RELATIONSHIP_NEXT) {
			results = this->pkb->getNext(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		} else if (relationship == RELATIONSHIP_NEXTT) {
			results = this->pkb->getNextT(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}
		else if (relationship == RELATIONSHIP_AFFECTS) {
			results = this->pkb->getAffects(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		}
		else if (relationship == RELATIONSHIP_AFFECTST) {
			results = this->pkb->getAffectsT(firstParamType, firstParamIndex, secondParamType, secondParamIndex);
		} 
		else {

		}
	}

	if (!results.empty()) {
		if (firstParamIndex == NOT_FOUND && secondParamIndex == NOT_FOUND) {
			if (clause.getLeftCStringValue() == clause.getRightCStringValue()) {
				if (clause.getLeftCStringValue() != STRiNG_UNDERSCORE) {
					if (relationship == RELATIONSHIP_AFFECTS || relationship == RELATIONSHIP_AFFECTST || relationship == RELATIONSHIP_NEXTT) {
						vector<pair<int, int>> temp = vector<pair<int, int>>();

						for (size_t i = 0; i < results.size(); i++) {
							if (results[i].first == results[i].second) {
								temp.push_back(make_pair(results[i].first, results[i].second));
							}
						}

						if (!temp.empty()) {
							results = temp;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
			}

		}
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
		return false;
		break;
	}
}

bool QueryEvaluator::evaluateWhile(Clauses clause) {
	const int PATTERN_SYN = 0;
	const int VARIABLE_SYN = 1;

	vector<vector<int>> intermediateResult;
	vector<int> patternSyn;
	vector<int> controlSyn;

	int controlVariable = pkb->getVarIndex(clause.getLeftCStringValue());
	int leftCIntValue = clause.getLeftCIntValue();

	if (clause.getLeftCType() == Enum::TYPE::UNDERSCORE) {
		//pattern while(_, _)
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::WHILE) {
				patternSyn.push_back(i);
			}
		}
		intermediateResult = { patternSyn };
	}
	else if (leftCIntValue == IS_SYN) {
		//pattern while (x, _)
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::WHILE) {
				patternSyn.push_back(i);
				controlSyn.push_back(pkb->getControlVar(i));
			}
		}
		intermediateResult = { patternSyn, controlSyn };
	}
	else if (controlVariable != NOT_FOUND) {
		//pattern while ("x", _)
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::WHILE) {
				if (pkb->getControlVar(i) == controlVariable) {
//					cout << "push back" << endl;
					patternSyn.push_back(i);
//					cout << i << endl;
//					cout << "after push back" << endl;
				}
//				cout << patternSyn.size() << endl;
			}
		}
		intermediateResult = { patternSyn };
	}

	if (intermediateResult.at(PATTERN_SYN).size() != 0) {
		vector<Enum::TYPE> type;
		vector<string> syn;
		type.push_back(clause.getParentType());
		syn.push_back(clause.getParentStringVal());
		if (!controlSyn.empty()) {
			type.push_back(Enum::TYPE::VARIABLE);
			syn.push_back(clause.getLeftCStringValue());
		}
		storeResults(type, syn, intermediateResult);
		return true;
	}
	else {
		return false;
	}
}

bool QueryEvaluator::evaluateIf(Clauses clause) {
	const int PATTERN_SYN = 0;
	const int VARIABLE_SYN = 1;
	
	vector<vector<int>> intermediateResult;
	vector<int> patternSyn;
	vector<int> controlSyn;

	int controlVariable = pkb->getVarIndex(clause.getLeftCStringValue());
	int leftCIntValue = clause.getLeftCIntValue();
	//cout << "leftCintValue" << leftCIntValue << endl;
	
	if (clause.getLeftCType() == Enum::TYPE::UNDERSCORE) {
		//pattern if(_, _, _)
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::IF) {
				patternSyn.push_back(i);
			}
		}
		intermediateResult = { patternSyn };
	}
	else if (leftCIntValue == IS_SYN) {
		//cout << "is syn" << endl;
		//pattern if (x, _, _)
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::IF) {
				patternSyn.push_back(i);
				controlSyn.push_back(pkb->getControlVar(i));
			}
		}
		intermediateResult = { patternSyn, controlSyn };
	}
	else if (controlVariable != NOT_FOUND) {
		//pattern if ("x", _, _)
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::IF) {
				if (pkb->getControlVar(i) == controlVariable) {
//					cout << "push back" << endl;
					patternSyn.push_back(i);
//					cout << i << endl;
//					cout << "after push back" << endl;
				}
//				cout << patternSyn.size() << endl;
			}
		}
		intermediateResult = { patternSyn };
	}

	if (intermediateResult.at(PATTERN_SYN).size() != 0) {
		vector<Enum::TYPE> type;
		vector<string> syn;
		type.push_back(clause.getParentType());
		syn.push_back(clause.getParentStringVal());
		if (!controlSyn.empty()) {
			type.push_back(Enum::TYPE::VARIABLE);
			syn.push_back(clause.getLeftCStringValue());
		}
		storeResults(type, syn, intermediateResult);
		return true;
	}
	else {
		return false;
	}

}



bool QueryEvaluator::evaluateAssign(Clauses clause) {
	vector <vector<int>> intermediateResult;
	vector<int> assignResult;
	vector<int> varResult;

	// if left child is underscore
	if (clause.getLeftCType() == Enum::TYPE::UNDERSCORE) {
		if (clause.getRightCType() == Enum::TYPE::UNDERSCORE) {
			return true;
		}
		else {
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			if (!clause.getRightCIsExpression()) {		
				// pattern a(_, x ) 
				for (int i = 1; i <= this->pkb->getNoOfStmt(); i++) {
					if (this->pkb->getRightExpr(i) == expr)
						assignResult.push_back(i);
				}
			}
			else {		
				// pattern a(_, _x_)
				for (int i = 1; i <= this->pkb->getNoOfStmt(); i++) {
					if (this->pkb->getRightExpr(i).find(expr) != NOT_FOUND) {
						assignResult.push_back(i);
					}
				}
			}
			intermediateResult = { assignResult };
		}
	}
	else if (clause.getLeftCIntValue() == IS_SYN) {
		vector<pair<int, int>> stmtLst = this->pkb->getModifies(Enum::ASSIGN, WILDCARD, Enum::TYPE::VARIABLE, WILDCARD);
		if (clause.getRightCType() == Enum::TYPE::UNDERSCORE) {
			// a(v, _)
			for (size_t i = 0; i < stmtLst.size(); i++) {
				assignResult.push_back(stmtLst[i].first);
				varResult.push_back(stmtLst[i].second);
			}
		}
		else {
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			if (!clause.getRightCIsExpression()) {
				// a(v, "x + y")
				for (size_t i = 0; i < stmtLst.size(); i++) {
					if (this->pkb->getRightExpr(stmtLst[i].first) == expr) {
						assignResult.push_back(stmtLst[i].first);
						varResult.push_back(stmtLst[i].second);
					}
				}
			}
			else {
				// a(v, "_x + y_")
				for (size_t i = 0; i < stmtLst.size(); i++) {
					cout << pkb->getRightExpr(stmtLst[i].first) << endl;
					if (this->pkb->getRightExpr(stmtLst[i].first).find(expr) != NOT_FOUND) {
						assignResult.push_back(stmtLst[i].first);
						varResult.push_back(stmtLst[i].second);
					}
					}
				}
		}
		intermediateResult = { assignResult, varResult };

	}
	else { //left child is a declared synonym
		int leftExpression = pkb->getVarIndex(clause.getLeftCStringValue());
		vector<pair<int, int>> stmtLst = this->pkb->getModifies(Enum::TYPE::ASSIGN, WILDCARD, Enum::TYPE::VARIABLE, leftExpression);
		// a("v", _)
		if (clause.getRightCType() == Enum::TYPE::UNDERSCORE) { 
			for (size_t i = 0; i < stmtLst.size(); i++) {
				assignResult.push_back(stmtLst[i].first);
			}
		}
		else {
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			// a("v", x + y)
			if (!clause.getRightCIsExpression()) { 
				for (size_t i = 0; i < stmtLst.size(); i++) {
					if (this->pkb->getRightExpr(stmtLst[i].first) == expr) {
						assignResult.push_back(stmtLst[i].first);
					}
				}
			}
			else { // a("v", _x+y_)
				for (size_t i = 0; i < stmtLst.size(); i++) {
					if (this->pkb->getRightExpr(stmtLst[i].first).find(expr) != NOT_FOUND) {
						assignResult.push_back(stmtLst[i].first);
					}
				}
			}
		}
		intermediateResult = { assignResult };
	}

	if (!intermediateResult.at(0).empty()) {
		vector<Enum::TYPE> type = { clause.getParentType() };
		vector<string> syn = { clause.getParentStringVal() };
		if (intermediateResult.size() == 2) {
			type.push_back(Enum::TYPE::VARIABLE);
			syn.push_back(clause.getLeftCStringValue());
		}
		storeResults(type, syn, intermediateResult);
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
						output.push_back(' ');
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
					output.push_back(' ');
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
		}
		stack.pop();
	}

	for (list<char>::iterator it = output.begin(); it != output.end(); ++it) {

		outputString.push_back(*it);
	}
	return outputString;
}

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
		int entityIndex = NOT_FOUND;

		if (clause.getLeftCType() == Enum::TYPE::VARIABLE || clause.getLeftCType() == Enum::TYPE::PROCEDURE) {
			entityIndex = checkValidityOfStringEntities(clause.getLeftCType(), clause.getRightCStringValue());
		}
		else if (clause.getLeftCType() == Enum::TYPE::CALLS) {
			if (clause.getLeftCIsStmt()) {
				entityIndex = checkValidityOfIntEntities(clause.getLeftCType(), clause.getRightCIntValue());
			}
			else {
				entityIndex = checkValidityOfStringEntities(clause.getLeftCType(), clause.getRightCStringValue());
			}
		}
		else {
			entityIndex = checkValidityOfIntEntities(clause.getLeftCType(), clause.getRightCIntValue());
		}

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
			return getCommonAttrValues(clause);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::ASSIGN:
		if (clause.getRightCType() == Enum::TYPE::STATEMENT || clause.getRightCType() == Enum::TYPE::CONSTANT
			|| clause.getRightCType() == Enum::TYPE::ASSIGN) {
			return getCommonAttrValues(clause);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::STATEMENT:
		return getCommonAttrValues(clause);
		break;
	case Enum::TYPE::IF:
		if (clause.getRightCType() == Enum::TYPE::STATEMENT || clause.getRightCType() == Enum::TYPE::CONSTANT
			|| clause.getRightCType() == Enum::TYPE::IF) {
			return getCommonAttrValues(clause);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::WHILE:
		if (clause.getRightCType() == Enum::TYPE::STATEMENT || clause.getRightCType() == Enum::TYPE::CONSTANT
			|| clause.getRightCType() == Enum::TYPE::WHILE) {
			return getCommonAttrValues(clause);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::CONSTANT:
		return getCommonAttrValues(clause);
		break;
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
	case Enum::TYPE::CALLS:
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::CALLS) {
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
		vector<int> resultsForLeftParam = vector<int>();
		vector<int> resultsForRightParam = vector<int>();

		for (size_t i = 0; i < mergedResults.size(); i++) {
			vector<string> name = vector<string>{ mergedResults.at(i) };
			vector<int> resultsForLeftParamName = convertNamesToIndexes(name, clause.getLeftCType());
			vector<int> resultsForRightParamName = convertNamesToIndexes(name, clause.getRightCType());
				
			if (resultsForLeftParamName.size() != resultsForRightParamName.size()) { // can only happen when call in present
				vector<int> temp = vector<int>();
				if (clause.getLeftCType() == Enum::TYPE::CALLS) {
					resultsForLeftParam.insert(resultsForLeftParam.end(), resultsForLeftParamName.begin(), resultsForLeftParamName.end());
					temp.assign(resultsForLeftParamName.size(), resultsForRightParamName.front());
					resultsForRightParam.insert(resultsForRightParam.end(), temp.begin(), temp.end());
				}
				else if (clause.getRightCType() == Enum::TYPE::CALLS){
					resultsForRightParam.insert(resultsForRightParam.end(), resultsForRightParamName.begin(), resultsForRightParamName.end());
					temp.assign(resultsForRightParamName.size(), resultsForLeftParamName.front());
					resultsForLeftParam.insert(resultsForLeftParam.end(), temp.begin(), temp.end());
				}
				else {

				}
			}
			else {
				resultsForRightParam.insert(resultsForRightParam.end(), resultsForRightParamName.begin(), resultsForRightParamName.end());
				resultsForLeftParam.insert(resultsForLeftParam.end(), resultsForLeftParamName.begin(), resultsForLeftParamName.end());
			}
		}

		vector<vector<int>> resultsToStore = { resultsForLeftParam, resultsForRightParam };
		storeResults(type, synString, resultsToStore);
		return true;
	}
}

bool QueryEvaluator::getCommonAttrValues(Clauses clause) {
	vector<int> leftSynResults;
	vector<int> rightSynResults;
	vector<int> intermediateResults;

	if (clause.getLeftCType() == Enum::TYPE::CONSTANT) { // c.value = c.value/s.stmt#/a.stmt#/if.stmt#/w.stmt#/n
		intermediateResults = getAllAttrValues(clause.getLeftCType());
		for (size_t i = 0; i < intermediateResults.size(); i++) {
			if (checkValidityOfIntEntities(clause.getRightCType(), intermediateResults.at(i)) != NOT_FOUND) {
				leftSynResults.push_back(pkb->getConstantIndex(intermediateResults.at(i)));
				rightSynResults.push_back(intermediateResults.at(i));
			}
		}
	}
	else {
		if(clause.getLeftCType() == Enum::TYPE::STATEMENT){ // s.stmt# = s.stmt#/a.stmt#/w.stmt#/if.stmt#/c.value/n
			intermediateResults = getAllAttrValues(clause.getRightCType());
			rightSynResults = intermediateResults;
			leftSynResults = intermediateResults;
		} else if (clause.getRightCType() == Enum::TYPE::CONSTANT) { // s.stmt#/a.stmt#/if.stmt#/w.stmt#/n/c.value = c.value
			intermediateResults = getAllAttrValues(clause.getRightCType());
			for (size_t i = 0; i < intermediateResults.size(); i++) {
				if (checkValidityOfIntEntities(clause.getLeftCType(), intermediateResults.at(i)) != NOT_FOUND) {
					rightSynResults.push_back(pkb->getConstantIndex(intermediateResults.at(i)));
					leftSynResults.push_back(intermediateResults.at(i));
				}
			}
		}
		else { // w.stmt# = w.stmt#/if.stmt#/s.stmt#/a.stmt#/n etc.
			intermediateResults = getAllAttrValues(clause.getLeftCType());
			rightSynResults = intermediateResults;
			leftSynResults = intermediateResults;
		}
	}
	
	vector<Enum::TYPE> type = { clause.getLeftCType(), clause.getRightCType() };
	vector<string> synString = { clause.getLeftCStringValue(), clause.getRightCStringValue() };
	if (leftSynResults.empty() || rightSynResults.empty()) {
		return false;
	}
	else {
		vector<vector<int>> resultsToStore = { leftSynResults, rightSynResults };
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
			vector<int> stmtsCalled = pkb->getStmtNumProcCalled(pkb->getProcIndex(stringResults[i]));
			for (size_t c = 0; c < stmtsCalled.size(); c++) {
				indexes.push_back(stmtsCalled.at(c));
			}
		}

		sort(indexes.begin(), indexes.end());
		indexes.erase(unique(indexes.begin(), indexes.end()), indexes.end());
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

	if (!resultsToStore.empty()) {
		storeResults(type, synString, resultsToStore);
	}
}

void QueryEvaluator::storeResults(vector<Enum::TYPE> type, vector<string> synString, vector<vector<int>> resultToStore) {
	Synonym syn = Synonym();

	syn.addResult(type, synString, resultToStore);

	this->results.push_back(syn);
}

string QueryEvaluator::convertToString(int index, Clauses syn) {
	Enum::TYPE type = syn.getParentType();

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
		if (syn.getParentIsStmt()) {
			return to_string(index);
		}
		else {
			for (int i = 0; i < pkb->getNoOfProc(); i++) {
				vector<int> callStmts = pkb->getStmtNumProcCalled(i);
				for (size_t s = 0; s < callStmts.size(); s++) {
					if (callStmts.at(s) == index) {
						return this->pkb->getProcName(i);
					} 
				}
			}

			return EMPTY_STRING;
		}
		break;
	case Enum::TYPE::CONSTANT:
		return to_string(this->pkb->getConstantValue(index));
		break;
	default:
	//	cout << "Convert to String, no TYPE matches" << endl;
		return EMPTY_STRING;
		break;
	}

}

/**************************** Merging and Sorting Results ****************************/
vector<vector<int>> QueryEvaluator::rearrangeSynonym(vector<vector<int>> syn) {
	vector<vector<int>> rearrangeResult;

	for (size_t i = 0; i < syn.size(); i++) {
		rearrangeResult.push_back({ syn.at(i).at(0) });
		syn.at(i).erase(syn.at(i).begin());
	}

	for (size_t i = 0; i < syn.at(0).size(); i++) {
	}

	for (size_t i = 0; i < syn.size(); i++) {
		for (size_t j = 0; j < syn.at(i).size(); j++) {
			for (size_t k = 0; k < rearrangeResult.at(i).size(); k++) {
				if (hasCommonSyn(this->results.at(rearrangeResult.at(i).at(k)), this->results.at(syn.at(i).at(j)))) {
					rearrangeResult.at(i).push_back(syn.at(i).at(j));
					syn.at(i).erase(syn.at(i).begin() + j);
					j = -1;
					break;
				}
			}
		}
	}

	return rearrangeResult;

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
	//			cout << "mergeSyn" << endl;
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
									result.at(k).push_back(result1[k][i]); // copy entire column
								}
						}
				}
			}
		}

		
		for (size_t k = 0; k < result1.size(); k++) {
			resultSynType.push_back(type1[k]);
			resultSynName.push_back(synName1[k]);
		}
		
	}
	else if (counter.size() == 1) {
		int row1 = counter[0].first;
		int row2 = counter[0].second;

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

		for (size_t i = 0; i < result1[row1].size(); i++) {
			for (size_t j = 0; j < result2[row2].size(); j++) {
				if (result1[row1][i] == result2[row2][j]) {

					for (size_t k = 0; k < result1.size(); k++) {
						result.at(k).push_back(result1[k][i]); // copy entire column
					}

					int offset = 0;
					for (size_t k = 0; k < result2.size(); k++) {
						if (k != row2) {
							result.at(k + numRow1 - offset).push_back(result2[k][j]);
						}
						else {
							offset = offset + 1;
						}
					}
				}
			}
		}


		for (size_t k = 0; k < result1.size(); k++) {
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

	for (size_t i = 0; i < type1.size(); i++) {
		for (size_t j = 0; j < type2.size(); j++) {
			if (type1[i] == type2[j]) {
				if (synName1[i] == synName2[j]) {
					counter.push_back(make_pair(i, j));
				}
			}
		}
	}

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

	syn.erase(syn.begin() + second);

	for (size_t i = 0; i < syn.at(first).size(); i++) {
		for (size_t j = i + 1; j < syn.at(first).size(); j++) {
			if (syn[first][i] == syn[first][j]) {
				syn.at(first).erase(syn.at(first).begin() + j);
				break;
			}
		}
	}

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
	
	if (select.front().getParentType() != Enum::TYPE::BOOLEAN) {
		nonCommonSyn = select;
	}
	else {
		for (size_t i = 0; i < groupedSyns.size(); i++) {
			if (groupedSyns[i].getSize() == 0) {
				stringedResults = { STRING_FALSE };
				return stringedResults;
			}
		}
		
		stringedResults = { STRING_TRUE };
		return stringedResults;
	}

	vector<Synonym> selectedSyns = getValuesOfSelectedSyns(groupedSyns, select);
	if (!this->nonCommonSyn.empty()) {
		for (size_t syn = 0; syn < this->nonCommonSyn.size(); syn++) {
			vector<vector<int>> resultsToStore;
			vector<int> results = getValuesOfNonCommonSyn(nonCommonSyn[syn]);
			resultsToStore.push_back(results);
			vector<Enum::TYPE> type = { nonCommonSyn[syn].getParentType() };
			vector<string> synString = { nonCommonSyn[syn].getParentStringVal() };
			Synonym nonCommonSyn = Synonym();
			nonCommonSyn.addResult(type, synString, resultsToStore);
			selectedSyns.push_back(nonCommonSyn);
		}

		/************************ Present Selected Syns **************************/
/*		cout << "Non Common Syn Present";
		cout << endl;
		cout << "Current Selected Syns";
		cout << endl;
		for (size_t i = 0; i < selectedSyns.size(); i++) {
			selectedSyns[i].printSyn();
		}
		/************************************************************************/
	}

	vector<pair<Enum::TYPE, vector<int>>> merged = mergeBetweenGroups(selectedSyns);
	return convertResultsToString(merged);
}

vector<Synonym> QueryEvaluator::getValuesOfSelectedSyns(vector<Synonym> groupedSyns, vector<Clauses> select) {
	vector<Synonym> mergedValues;
	vector<Enum::TYPE> type;
	vector<string> synString;
	vector<vector<int>> resultsToStore;

	for (size_t groupIndex = 0; groupIndex < groupedSyns.size(); groupIndex++) { // in one group
		vector<string> synsInAGroup = groupedSyns[groupIndex].getSyn();
		vector<Enum::TYPE> typesInAGroup = groupedSyns[groupIndex].getType();
		vector<vector<int>> synValuesInAGroup = groupedSyns[groupIndex].getResult();

		for (size_t groupPos = 0; groupPos < synsInAGroup.size(); groupPos++) { // a syn in a group
			for (size_t syn = 0; syn < nonCommonSyn.size(); syn++) { // a selected syn from select tree
				if (synsInAGroup[groupPos] == nonCommonSyn.at(syn).getParentStringVal() && typesInAGroup[groupPos] == nonCommonSyn.at(syn).getParentType()) {
					type.push_back(typesInAGroup[groupPos]);
					synString.push_back(synsInAGroup[groupPos]);
					resultsToStore.push_back(synValuesInAGroup[groupPos]);
					nonCommonSyn.erase(nonCommonSyn.begin() + syn);
					break;
				}
			}
		}

		if (!type.empty()) {
			Synonym syn = Synonym();
			syn.addResult(type, synString, resultsToStore);
			mergedValues.push_back(syn);
		}

		type.clear();
		synString.clear();
		resultsToStore.clear();
	}
	/**************** Printing Selected Syns *******************/
/*	cout << "Selected Syns";
	cout << endl;

	if (mergedValues.empty()) {
		cout << "<empty>";
		cout << endl;
	}
	else {
		for (size_t i = 0; i < mergedValues.size(); i++) {
			mergedValues[i].printSyn();
		}
	}
	/**********************************************************/

	return mergedValues;
}

vector<int> QueryEvaluator::getValuesOfNonCommonSyn(Clauses nonCommon) {
	Enum::TYPE typeOfSyn = nonCommon.getParentType();

	if (typeOfSyn == Enum::TYPE::VARIABLE || typeOfSyn == Enum::TYPE::PROCEDURE) {
		return getStringedAttrIndexes(typeOfSyn);
	}
	else if (typeOfSyn == Enum::TYPE::CALLS) {
		return getAllAttrValues(typeOfSyn);
	}
	else if (typeOfSyn == Enum::TYPE::CONSTANT) {
		vector<int> constantIndexes = vector<int>();

		for (int i = 0; i < pkb->getNoOfConstants(); i++) {
			constantIndexes.push_back(i);
		}
		return constantIndexes;
	}
	else {
		return getAllAttrValues(typeOfSyn);
	}
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
	default:
		break;
	}

	return stringedAttrIndexes;
}

vector<pair<Enum::TYPE, vector<int>>> QueryEvaluator::mergeBetweenGroups(vector<Synonym> mergedSyns) {
	vector<pair<Enum::TYPE, vector<int>>> merged;
	if (mergedSyns.size() == 0) {
		return merged;
	} else if (mergedSyns.size() == 1) {
		/******************** Merged Syns *********************/
	/*	cout << "Merged Syns";
		cout << endl;
		mergedSyns.front().printSyn();
		/*****************************************************/
		merged = extractTypeAndIndexes(rearrangeSynOrder(mergedSyns.front(), tree.getResultTree()));
		return merged;
	}
	else {
		int lastSetToMerge;
		int numSyn = mergedSyns.size();
		bool isOdd = false;
		vector<Synonym> mergedResults;

		if (numSyn % 2 == 1) {
			isOdd = true;
		}
		else {
			isOdd = false;
		}

		if (isOdd) {
			lastSetToMerge = mergedSyns.size() - 2; // leave the last set out.
		}
		else {
			lastSetToMerge = mergedSyns.size() - 1;
		}

		for (int i = 0; i < lastSetToMerge; i += 2) {
			mergedResults.push_back(mergeTwoGroups(mergedSyns[i], mergedSyns[i + 1]));
		}


		if (isOdd) {
			mergedResults.push_back(mergedSyns.back()); // will terminate, eventually it will be 3 sets (if more than 3) which merge to become 1.
		}

		return mergeBetweenGroups(mergedResults);
	}
}

Synonym QueryEvaluator::mergeTwoGroups(Synonym firstSet, Synonym secondSet) {
	Synonym mergedPair = Synonym();

	vector<Enum::TYPE> typeOfFirstSet = firstSet.getType();
	vector<string> synStringOfFirstSet = firstSet.getSyn();
	vector<vector<int>> synValuesOfFirstSet = firstSet.getResult();
	vector<Enum::TYPE> typeOfSecondSet = secondSet.getType();
	vector<string> synStringOfSecondSet = secondSet.getSyn();
	vector<vector<int>> synValuesOfSecondSet = secondSet.getResult();
	
	vector<Enum::TYPE> typeOfMergedSet = typeOfFirstSet;
	typeOfMergedSet.insert(typeOfMergedSet.end(), typeOfSecondSet.begin(), typeOfSecondSet.end());

	vector<string> synStringOfMergedSet = synStringOfFirstSet;
	synStringOfMergedSet.insert(synStringOfMergedSet.end(), synStringOfSecondSet.begin(), synStringOfSecondSet.end());

	vector<vector<int>> synValuesOfMergedSet;
		
	vector<int> tempForFirstSet;
	vector<int> mergedValuesForFirstSet;
	vector<int> tempForSecondSet;
	vector<int> mergedValuesForSecondSet;
		
	if (synValuesOfFirstSet.empty() || synValuesOfSecondSet.empty()) {
		mergedPair.addResult(typeOfMergedSet, synStringOfMergedSet, synValuesOfMergedSet);
		return mergedPair;
	}
	else {
		int numOfValuesForFirstSetSyn = synValuesOfFirstSet[0].size();
		int numOfValuesForSecondSetSyn = synValuesOfSecondSet[0].size();

		for (size_t i = 0; i < synStringOfFirstSet.size(); i++) {
			for (int values = 0; values < numOfValuesForFirstSetSyn; values++) {
				tempForFirstSet.assign(numOfValuesForSecondSetSyn, synValuesOfFirstSet[i][values]);
				mergedValuesForFirstSet.insert(mergedValuesForFirstSet.end(), tempForFirstSet.begin(), tempForFirstSet.end());
				tempForFirstSet.clear();
			}

			synValuesOfMergedSet.push_back(mergedValuesForFirstSet);
			mergedValuesForFirstSet.clear();
		}

		for (size_t i = 0; i < synStringOfSecondSet.size(); i++) {
			tempForSecondSet = synValuesOfSecondSet[i];

			for (int i = 0; i < numOfValuesForFirstSetSyn; i++) {
				mergedValuesForSecondSet.insert(mergedValuesForSecondSet.end(), tempForSecondSet.begin(), tempForSecondSet.end());
			}

			synValuesOfMergedSet.push_back(mergedValuesForSecondSet);
			mergedValuesForSecondSet.clear();
		}

		mergedPair.addResult(typeOfMergedSet, synStringOfMergedSet, synValuesOfMergedSet);
		return mergedPair;
	}
}

Synonym QueryEvaluator::rearrangeSynOrder(Synonym mergedSelectedSyn, vector<Clauses> select) {
	vector<Enum::TYPE> arrangedType;
	vector<string> arrangedSynString;
	vector<vector<int>> arrangedResultsToStore;

	for (size_t i = 0; i < select.size(); i++) {
		vector<Enum::TYPE> type = mergedSelectedSyn.getType();
		vector<string> synString = mergedSelectedSyn.getSyn();
		vector<vector<int>> results = mergedSelectedSyn.getResult();

		for (size_t syn = 0; syn < synString.size(); syn++) {
			if (synString[syn] == select[i].getParentStringVal() && type[syn] == select[i].getParentType()) {
				arrangedType.push_back(type[syn]);
				arrangedSynString.push_back(synString[syn]);
				arrangedResultsToStore.push_back(results[syn]);
				break;
			}
		}
	}

	Synonym syn = Synonym();
	syn.addResult(arrangedType, arrangedSynString, arrangedResultsToStore);

	/**************** Arranged Syns ******************/
/*	cout << "Arranged Syns";
	cout << endl;
	syn.printSyn();
	/************************************************/
	return syn;
}

vector<pair<Enum::TYPE, vector<int>>> QueryEvaluator::extractTypeAndIndexes(Synonym merged) {
	vector<pair<Enum::TYPE, vector<int>>> extractedSyns;

	vector<Enum::TYPE> type = merged.getType();
	vector<vector<int>> results = merged.getResult();

	for (size_t i = 0; i < type.size(); i++) {
		extractedSyns.push_back(make_pair(type[i], results[i]));
	}
	
	return extractedSyns;
}

list<string> QueryEvaluator::convertResultsToString(vector<pair<Enum::TYPE, vector<int>>> arrangedSyns) {
	list<string> stringedResults = list<string>();
	vector<Clauses> select = tree.getResultTree();
	int numOfValuesPerSyn = arrangedSyns.at(0).second.size();
	for (int values = 0; values < numOfValuesPerSyn; values++) {
		vector<int> valuesOfSyn = arrangedSyns[0].second;
		string combinedValues = convertToString(valuesOfSyn.at(values), select[0]);
		for (size_t syn = 1; syn < arrangedSyns.size(); syn++) {
			combinedValues = combinedValues + " " + convertToString(arrangedSyns[syn].second.at(values), select[syn]);
		}

		stringedResults.push_back(combinedValues);
	}

	return stringedResults;
}

void QueryEvaluator::printResults() {
	for (size_t i = 0; i < this->results.size(); i++) {
		results[i].printSyn();
	}
}