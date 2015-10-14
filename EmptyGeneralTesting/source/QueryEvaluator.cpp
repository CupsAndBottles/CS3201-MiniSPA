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
const string STRING_BOOLEAN = "BOOLEAN";
const string STRING_FALSE = "FALSE";
const string STRING_TRUE = "TRUE";

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
	vector<vector<string>> intermediateResult;
	list<string> result;
	bool isTrueClause;

	if (!tree.getIsValid()) { // variables not found in program
		list<string> emptyResult{};
		return emptyResult;
	}

	suchThat = tree.getSuchThatTree();
	pattern = tree.getPatternTree();
	with = tree.getWithTree();
	select = tree.getResultTree();

	for (size_t i = 0; i < suchThat.size(); i++) {
		isTrueClause = evaluateSuchThat(suchThat[i]);
		if (!isTrueClause) {
			list<string> emptyResult;
			if (select.at(0).getParentStringVal() == STRING_BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			else {
				emptyResult = {};
			}
			return emptyResult;
		}
	}

	for (size_t i = 0; i < pattern.size(); i++) {
		isTrueClause = evaluatePattern(pattern[i]);
		if (!isTrueClause) {
			list<string> emptyResult;
			if (select.at(0).getParentStringVal() == STRING_BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			else {
				emptyResult = {};
			}
			return emptyResult;
		}
	}

	for (size_t i = 0; i < with.size(); i++) {
		isTrueClause = evaluateWith(with[i]);
		if (!isTrueClause) {
			list<string> emptyResult;
			if (select.at(0).getParentStringVal() == STRING_BOOLEAN) {
				emptyResult = { STRING_FALSE };
			}
			else {
				emptyResult = {};
			}
			return emptyResult;
		}
	}

	if (select.at(0).getParentStringVal() == STRING_BOOLEAN) {
		list<string> trueResult = { STRING_TRUE };
		return trueResult;
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
	Enum::TYPE type = select.getParentType();

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
				resultForSyn.push_back(convertToString(i, type));
			}
			break;
		case Enum::TYPE::ASSIGN:
			for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
				if (pkb->getType(i) == Enum::TYPE::ASSIGN) {
					resultForSyn.push_back(convertToString(i, type));
				}
			}
			break;
		case Enum::TYPE::WHILE:
			for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
				if (pkb->getType(i) == Enum::TYPE::WHILE) {
					resultForSyn.push_back(convertToString(i, type));
				}
			}
			break;
		case Enum::TYPE::IF:
			for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
				if (pkb->getType(i) == Enum::TYPE::IF) {
					resultForSyn.push_back(convertToString(i, type));
				}
			}
			break;
		case Enum::TYPE::PROCEDURE:
			for (int i = 0; i < pkb->getNoOfProc(); i++) {
				resultForSyn.push_back(convertToString(i, type));
			}
			break;
		case Enum::TYPE::VARIABLE:
			for (int i = 0; i < pkb->getNoOfVar(); i++) {
				resultForSyn.push_back(convertToString(i, type));
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

// without consideration to constant
bool QueryEvaluator::evaluateWith(Clauses clause) {
	vector<int> results;

	if (clause.getRightCIntValue() != NOT_FOUND) {
		if (clause.getLeftCType() != Enum::TYPE::VARIABLE && clause.getLeftCType() != Enum::TYPE::PROCEDURE 
			&& clause.getLeftCType() != Enum::TYPE::CONSTANT) {
			if (clause.getLeftCType() == Enum::TYPE::CALLS) {
				if (clause.getLeftCIsStmt()) {
					return evaluateValidStmtRefs(clause.getLeftCType(), clause.getRightCIntValue());
				}
			}
			else {
				return evaluateValidStmtRefs(clause.getLeftCType(), clause.getRightCIntValue());
			}
		}

		results.push_back(clause.getRightCIntValue());
		storeResults(results, clause.getLeftCStringValue(), clause.getLeftCType());
		return true;
	}
	else { // n = c.value / s/a/w/if.stmt# = c.value / v.varName/p.procName/call.procName = p.procName/v.varName/call.procName
		return evaluateNonGivenAttr(clause);
	}
}

bool QueryEvaluator::evaluateValidStmtRefs(Enum::TYPE type, int index) {
	switch (type) {
	case Enum::TYPE::STATEMENT:
		return (0 < index && index <= pkb->getNoOfStmt());
		break;
	case Enum::TYPE::ASSIGN:
		if (0 < index && index <= pkb->getNoOfStmt()) {
			return (pkb->getType(index) == Enum::TYPE::ASSIGN);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::WHILE:
		if (0 < index && index <= pkb->getNoOfStmt()) {
			return (pkb->getType(index) == Enum::TYPE::WHILE);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::IF:
		if (0 < index && index <= pkb->getNoOfStmt()) {
			return (pkb->getType(index) == Enum::TYPE::IF);
		}
		else {
			return false;
		}
		break;
	case Enum::TYPE::CALLS:
		if (0 < index && index <= pkb->getNoOfStmt()) {
			return (pkb->getType(index) == Enum::TYPE::CALLS);
		}
		else {
			return false;
		}
	default:
		return false;
		break;
	}
}

bool QueryEvaluator::evaluateNonGivenAttr(Clauses clause) {
	switch (clause.getLeftCType()) {
	case Enum::TYPE::PROCEDURE:
		return hasSameAttrNames(clause); 
		break;
	case Enum::TYPE::VARIABLE:
		return hasSameAttrNames(clause);
		break;
	case Enum::TYPE::CALLS:
		return hasSameAttrNames(clause);
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
		break;
	case Enum::TYPE::CONSTANT:
		return hasSameAttrValues(clause);
		break;
/*	case Enum::TYPE::PROG_LINE:
		return hasSameAttrNum(clause);
		break; */
	default:
		return false;
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
/*		for(int i = 0; i <= pkb->getNoOfConstants(); i++) {
			allValues.push_back(pkb->getConstantIndex());
		}		
*/
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

	storeResults(convertNamesToIndexes(mergedResults, clause.getLeftCType()), clause.getLeftCStringValue(), clause.getLeftCType());
	storeResults(convertNamesToIndexes(mergedResults, clause.getRightCType()), clause.getRightCStringValue(), clause.getRightCType());
	return !mergedResults.empty();
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

	storeResults(mergedResults, clause.getLeftCStringValue(), clause.getLeftCType());
	storeResults(mergedResults, clause.getRightCStringValue(), clause.getRightCType());
	return !mergedResults.empty();
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

bool QueryEvaluator::evaluateSuchThat(Clauses clause) {
	bool hasValidFirstIndex = false;
	bool hasValidSecondIndex = false;
	vector<pair<int, int>> results;
	string relationship = clause.getParentStringVal();

	Enum::TYPE firstParamType = clause.getLeftCType();
	Enum::TYPE secondParamType = clause.getRightCType();
	int firstParamIndex = clause.getLeftCIntValue();
	int secondParamIndex = clause.getRightCIntValue();

	if (firstParamType != Enum::TYPE::CALLS && firstParamType != Enum::TYPE::PROCEDURE
		&& firstParamType != Enum::TYPE::VARIABLE && firstParamType != Enum::TYPE::CONSTANT) {
		if (firstParamIndex != NOT_FOUND) {
			hasValidFirstIndex = evaluateValidStmtRefs(firstParamType, firstParamIndex);
		}
		else {
			hasValidFirstIndex = true;
		}
	}
	else {
		hasValidFirstIndex = true;
	}

	if (secondParamType != Enum::TYPE::CALLS && secondParamType != Enum::TYPE::PROCEDURE
		&& secondParamType != Enum::TYPE::VARIABLE && secondParamType != Enum::TYPE::CONSTANT) {
		if (secondParamIndex != NOT_FOUND) {
			hasValidSecondIndex = evaluateValidStmtRefs(secondParamType, secondParamIndex);
		}
		else {
			hasValidSecondIndex = true;
		}
	}
	else {
		hasValidSecondIndex = true;
	}

	if (hasValidFirstIndex && hasValidSecondIndex) {
		if (relationship == RELATIONSHIP_CALLS) {
			results = this->pkb->getCalls(firstParamIndex, secondParamIndex);
		} /*else if (relationship == RELATIONSHIP_CALLST {
			results = this->pkb->getCalls*(firstParamIndex, secondParamIndex);
		}*/
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
		return evaluateIf(clause);
		break;
	case Enum::TYPE::WHILE :
		return evaluateWhile(clause);
		break;
	default:
		break;
	}
}

bool QueryEvaluator::evaluateWhile(Clauses clause) {
	vector<int> intermediateResult;

	if (clause.getLeftCType() == Enum::TYPE::UNDERSCORE) {
		//pattern if(_, _, _)
		for (size_t i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::WHILE) {
				intermediateResult.push_back(i);
			}
		}
	}
	else if (clause.getLeftCIntValue() != NOT_FOUND) {
		//pattern if (x, _, _)
		for (int i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::WHILE) {
				if (pkb->getControlVar(i) == clause.getLeftCIntValue()) {
					intermediateResult.push_back(i);
				}
			}
		}
	}

	if (intermediateResult.size() != 0) {
		storeResults(intermediateResult, clause.getParentStringVal(), Enum::TYPE::WHILE);
		return true;
	}
	else {
		return false;
	}
}

bool QueryEvaluator::evaluateIf(Clauses clause) {
	vector<int> intermediateResult;

	if (clause.getLeftCType() == Enum::TYPE::UNDERSCORE) {
		//pattern if(_, _, _)
		for (size_t i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::IF) {
				intermediateResult.push_back(i);
			}
		}
	}
	else if (clause.getLeftCIntValue() != NOT_FOUND) {
		//pattern if (x, _, _)
		for (size_t i = 1; i <= pkb->getNoOfStmt(); i++) {
			if (pkb->getType(i) == Enum::TYPE::IF) {
				if (pkb->getControlVar(i) == clause.getLeftCIntValue()) {
					intermediateResult.push_back(i);
				}
			}
		}
	}

	if (intermediateResult.size() != 0) {
		storeResults(intermediateResult, clause.getParentStringVal(), Enum::TYPE::IF);
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
		else{ 			
			string expr = convertToShuntingYard(clause.getRightCStringValue());
			cout << expr << endl;
			if (!clause.getRightCIsExpression()) {		// pattern a(_, x ) 
				for (int i = 1; i <= this->pkb->getNoOfStmt(); i++) {
					//cout << i << "." << endl;
					//cout << pkb->getRightExpr(i) << endl;
					if (this->pkb->getRightExpr(i) == expr)
						intermediateResult.push_back(i);
				}
			}
			else {		// pattern a(_, _x_)
				for (int i = 1; i <= this->pkb->getNoOfStmt(); i++) {
					if (this->pkb->getRightExpr(i).find(expr) != NOT_FOUND) {
						intermediateResult.push_back(i);
						//cout << i << endl;
						//cout << "RightExpr:" << pkb->getRightExpr(i) << endl;
					}
				}
			}
		}
	}
	else { //left child is a variable
		vector<pair<int,int>> stmtLst = this->pkb->getModifies(Enum::TYPE::ASSIGN, WILDCARD , Enum::TYPE::VARIABLE, clause.getLeftCIntValue());
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

	if (clause.getLeftCIntValue() == WILDCARD) {
		for (size_t i = 0; i < results.size(); i++) {
			firstSynResults.push_back(results[i].first);
		}

		sort(firstSynResults.begin(), firstSynResults.end());
		firstSynResults.erase(unique(firstSynResults.begin(), firstSynResults.end()), firstSynResults.end());
		storeResults(firstSynResults, clause.getLeftCStringValue(), clause.getLeftCType());
	}

	if (clause.getRightCIntValue() == WILDCARD) {
		for (size_t i = 0; i < results.size(); i++) {
			secondSynResults.push_back(results[i].second);
		}
		sort(secondSynResults.begin(), secondSynResults.end());
		secondSynResults.erase(unique(secondSynResults.begin(), secondSynResults.end()), secondSynResults.end());
		storeResults(secondSynResults, clause.getRightCStringValue(), clause.getRightCType());
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

	cout << "storing results\n";

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
