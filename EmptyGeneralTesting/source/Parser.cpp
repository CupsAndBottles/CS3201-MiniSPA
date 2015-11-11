#include "Parser.h"
#include "iostream"
#include <fstream>
#include <string>
#include <stack>
#include <sstream>
#include <iterator>
#include <list>
#include "PKB.h"

Parser::Parser()
{

}

Parser::~Parser()
{
}

PKB * Parser::getPkb()
{
	return pkb;
}

string Parser::openFile(string fileName) {
	ifstream myfile(fileName);
	string output;
	if (myfile.is_open())
	{
		while (getline(myfile, currLine))
		{
			size_t comments = currLine.find("\\");
			if (comments != string::npos)
			{
				currLine = currLine.erase(comments);
				allLines += " " + currLine;
			}
			else {
				allLines += " " + currLine;
			}
		}
		myfile.close();
		allLines.erase(remove(allLines.begin(), allLines.end(), '\t'), allLines.end());
		allLines.erase(remove_if(allLines.begin(), allLines.end(), isspace), allLines.end());

		lines = splitLines(allLines);
		output = linesSplitted(lines);
		Procedure();
	}

	else
	{
		output = "Unable to open file";
		cout << output;
	}
	return output;
}

string Parser::linesSplitted(list<pair<int, string>> linesToTest) {
	string output;
	while (!linesToTest.empty()) {
		pair<int, string> indexAndLines = linesToTest.front();
		int index = indexAndLines.first;
		string line = indexAndLines.second;
		output.append(to_string(index) + ": " + line);
		if (!linesToTest.empty()) {
			linesToTest.pop_front();
		}
	}
	return output;
}


list<pair<int, string>> Parser::splitLines(string lines)
{

	list<pair<int, string>> result;
	size_t position = 0;
	pair <int, string> pair;
	stmtNum = 0;
	int curly = 0;

	while ((position = lines.find_first_of("{;}\n")) != string::npos)
	{

		if (lines[position] != '\n')
		{
			string stmtToStore = lines.substr(0, position + 1);
			if (stmtToStore == "}") {
				string s = result.back().second;
				result.pop_back();
				stmtToStore = s.append("}");
				stmtNum--;
			}
			stmtNum++;
			pair.first = stmtNum;
			pair.second = stmtToStore;
			result.push_back(pair);


		}
		lines = lines.substr(position + 1);
	}

	return result;
}

void Parser::Procedure() {
	list<pair<int, string >>::iterator i;
	int endIndex=0;
	for (i = (lines).begin(); i != (lines).end(); ++i) {
		string stmt = (*i).second;
		//cout << stmt << "\n";
		if (stmt.find("procedure") != std::string::npos) {
			processProcedure((*i).first, (*i).second);
			processNextPrev((*i).first, (*i).second);
		}
		else if (stmt.find("while") != std::string::npos) {
			pkb->setType(Enum::WHILE);
			processWhile((*i).first, (*i).second);
			handleModifyAndUses((*i).first, (*i).second);
			handleFollows((*i).first, (*i).second);
			addToParent((*i).first);
			processNextPrev((*i).first, (*i).second);
			endIndex = (*i).first;
		}
		else if (stmt.find("if") != std::string::npos) {
			pkb->setType(Enum::IF);
			processIf((*i).first, (*i).second);
			processNextPrev((*i).first, (*i).second);
			endIndex = (*i).first;
		}
		else if (stmt.find("else") != std::string::npos) {
			processElse((*i).first, (*i).second);
			handleFollows((*i).first, (*i).second);
			currElse = (*i).first - numOfProc-numOfElse+1;
			processNextPrev((*i).first, (*i).second);
			endIndex = (*i).first;
		}
		else if (stmt.find("call") != std::string::npos) {
			pkb->setType(Enum::CALLS);
			processCalls((*i).first, (*i).second);
			//handleModifyAndUses((*i).first, (*i).second);
			handleFollows((*i).first, (*i).second);
			processNextPrev((*i).first, (*i).second);
			endIndex = (*i).first;
		}
		else {
			pkb->setType(Enum::ASSIGN);
			processExpressions((*i).first, (*i).second);
			handleModifyAndUses((*i).first, (*i).second);
			handleFollows((*i).first, (*i).second);
			processNextPrev((*i).first, (*i).second);
			endIndex = (*i).first;
		}

	}
	setProcEndNum(procNumInTble, endIndex);
	setRelationsInTable();
	pkb->setParentTChildrenT();
	addModifies();
	addUses();
	pkb->setByDesignExtractor();

}

void Parser::addModifies() {
	vector<int> children;
	for (int i = 1;i < pkb->getNoOfStmt(); i++) {
		children = pkb->getChildrenT(i);
		while (!children.empty()) {
			vector<int> modifiesList = pkb->getModifiesForParser(children.back());
			while (!modifiesList.empty()) {
				string varName = pkb->getVarName(modifiesList.back());
				pkb->setModifies(i, varName);
				pkb->setModifiedBy(varName, i);
				modifiesList.pop_back();
			}
			children.pop_back();
		}
	}
}

void Parser::addUses() {
	vector<int> children;
	for (int i = 1;i < pkb->getNoOfStmt(); i++) {
		children = pkb->getChildrenT(i);
		while (!children.empty()) {
			vector<int> usesList = pkb->getUsesForParser(children.back());
			while (!usesList.empty()) {
				string varName = pkb->getVarName(usesList.back());

				pkb->setUsedBy(varName, i);
				pkb->setUsedVar(i, varName);
				usesList.pop_back();
			}
			children.pop_back();
		}
	}
}

void Parser::setRelationsInTable() {
	pkb->setChildren(parentLink);
    pkb->setFollows(followLink);
	string procName = pkb->setProcCalls(callsLink);
	if (!procName.empty()) {
		throw "ProcName: " + procName +" does not exist.\n";
	}
	pkb->setStmtNumProcCalled(stmtNoAndCalls);
}

void Parser::processNextPrev(int index, string stmt)
{
	if (pStmt.empty()) {
		pStmt = stmt;
		pStmtIndex = index - numOfProc - numOfElse;
	}
	else {

		if (pStmt.find("}") != std::string::npos && !ifIndex.empty() && stmt.find("else") == std::string::npos) {
			while (!ifIndex.empty()) {
				pkb->setNext(ifIndex.back(), index - numOfProc - numOfElse);
				pkb->setPrev(index - numOfProc - numOfElse,ifIndex.back());
				ifIndex.pop_back();
			}
		}

		if (pStmt.find("else") != std::string::npos) {
			pkb->setPrev(index - numOfProc - numOfElse, ifIndexStmt.back());
			pkb->setNext(ifIndexStmt.back(), index - numOfProc - numOfElse );

		}
		if (stmt.find("procedure") == std::string::npos && !whileIndex.empty()) {
			size_t n = count(pStmt.begin(), pStmt.end(), '}');
			for (size_t i = 0; i < n;i++) {
				if (!whileIndex.empty()) {
					pkb->setNext(whileIndex.back(), index - numOfProc - numOfElse);
					pkb->setPrev(index - numOfProc - numOfElse, whileIndex.back());
					whileIndex.pop_back();
				}
			}
		}
		if (stmt.find("procedure") != std::string::npos) {
			pStmt = stmt;
			pStmtIndex = index - numOfProc - numOfElse;
			whileIndex.clear();
		}
		else if (stmt.find("while") != std::string::npos) {
			indexAndType.push_back(make_pair(1, index - numOfProc - numOfElse));
			pStmt = stmt;
			pStmtIndex = index - numOfProc - numOfElse;
			pkb->setNext(index - numOfProc - numOfElse, index - numOfProc - numOfElse+1);
		}
		else if (stmt.find("if") != std::string::npos) {
			indexAndType.push_back(make_pair(2, index - numOfProc - numOfElse));
			ifIndexStmt.push_back(index - numOfProc - numOfElse);
			pkb->setPrev(index - numOfProc - numOfElse, pStmtIndex);
			pStmt = stmt;
			pStmtIndex = index - numOfProc - numOfElse;
			pkb->setNext(index - numOfProc - numOfElse, index - numOfProc - numOfElse + 1);
		
		}
		else if (stmt.find("else") != std::string::npos) {
			indexAndType.push_back(make_pair(3, index - numOfProc - numOfElse));
			pStmt = stmt;
			pStmtIndex = index - numOfProc - numOfElse;

		}
		else {
			if (stmt.find("}") != std::string::npos) {
				size_t n = count(stmt.begin(), stmt.end(), '}');
				for (size_t i = 0; i < n;i++) {
					if (!indexAndType.empty()) {
						if (indexAndType.back().first == 1) {
							pkb->setPrev(index - numOfProc - numOfElse, pStmtIndex);
							pkb->setNext(pStmtIndex, index - numOfProc - numOfElse);
							pkb->setNext(index - numOfProc - numOfElse, indexAndType.back().second);
							pkb->setPrev(indexAndType.back().second, index - numOfProc - numOfElse);
							whileIndex.push_back(indexAndType.back().second);
						}
						else if (indexAndType.back().first == 2) {
							ifIndex.push_back(index - numOfProc - numOfElse);
							pkb->setPrev(index - numOfProc - numOfElse, pStmtIndex);
							pair<int, int> currIf = indexAndType.back();
							indexAndType.pop_back();
							if (!indexAndType.empty()) {
								pkb->setNext(index-numOfProc-numOfElse,indexAndType.back().second);
								pkb->setPrev(indexAndType.back().second, index - numOfProc - numOfElse);

							}
							indexAndType.push_back(currIf);
						}
						else if (indexAndType.back().first == 3) {
							ifIndex.push_back(index - numOfProc - numOfElse);
							ifIndexStmt.pop_back();
							if (!indexAndType.empty() && indexAndType.size()>1) {
								indexAndType.pop_back();
								pkb->setNext(index - numOfProc - numOfElse, indexAndType.back().second);
								pkb->setPrev(indexAndType.back().second, index - numOfProc - numOfElse);
							}
						}
						indexAndType.pop_back();
					}
				/*	else {
					pkb->setNext(pStmtIndex, index - numOfProc - numOfElse);
					pkb->setPrev(index - numOfProc - numOfElse, pStmtIndex);
					}*/
				}
			}
			else {
				if (pStmt.find("procedure") == std::string::npos) {
					if (pStmt.find("}") == std::string::npos && pStmt.find("else") == std::string::npos) {
						pkb->setPrev(index - numOfProc - numOfElse, pStmtIndex);
					}
				}
				pkb->setNext(index - numOfProc - numOfElse, index - numOfProc - numOfElse + 1);
			}
			pStmt = stmt;
			pStmtIndex = index - numOfProc - numOfElse;
		}
	}
}

void Parser::addToParent(int child) {
	pair<int, string> parentPair;
	int parent = 0;
	if (!containerElements.empty()) {
		parentPair = containerElements.back();
		//cout << "parent pair: " << parentPair.first << "\n";
		//cout << "num of else: " << numOfElse << "\n";
		//cout << "num of proc: " <<numOfProc << "\n";
		string parentStmt = parentPair.second;
		pair<int, int> pairs;
		parent = parentPair.first - numOfProc - numOfElse;

		if (parentStmt.find("while") != std::string::npos && currElse >(parentPair.first - numOfProc)) {
			parent = parentPair.first - numOfProc;
		}
		if (parentStmt.find("if") != std::string::npos && currElse >(parentPair.first - numOfProc)) {
			//cout << "else: " <<currElse << "\n";
			parent = parentPair.first - numOfProc - numOfElse+numOfElse;
		}

		int newChild = child - numOfProc - numOfElse;
		if (parent != newChild) {
			//cout << "if parent: " << parent << "\n";
			//cout << "if child: " << newChild <<"\n";
			pairs.first = parent;
			pairs.second = newChild;
			parentLink.push_back(pairs);
		}
		}
}

void Parser::processProcedure(int index, string statement) {
	//	currFollows.clear();
	containerElements.clear();
	prevStmt = "";
	size_t bracketPos = statement.find("{");
	statement.replace(bracketPos, string("{").length(), "");
	pushOpenBracket();
	string procName = statement.substr(statement.find("procedure") + 9);
	currProcName = procName;
	if (numOfProc >0) {
		//cout << "End num: " << index-1 << "\n";
		setProcEndNum(procNumInTble, index - 1);
	}
	procNumInTble = pkb->setProcNameInProcTable(procName);
	pkb->setStartNum(procNumInTble, index-numOfElse-numOfProc);

	numOfProc++;

}

void Parser::processCalls(int index, string stmt)
{
	pair<int, string> callsPair;
	pair<int, string> stmtNoCallPair;
	string procCalls = stmt.substr(stmt.find("call") + 4);
	size_t semiColonPos = procCalls.find(";");
	if (semiColonPos != std::string::npos) {
		procCalls.replace(semiColonPos, string(";").length(), "");
	}
	size_t bracketPos = procCalls.find("}");
	size_t n = count(procCalls.begin(), procCalls.end(), '}');
	//cout << prevStmt << "\n";
	for (size_t i = 0; i < n;i++) {
		procCalls.replace(bracketPos, string("}").length(), "");
	}

	int procExist = pkb->getProcIndex(procCalls);

	if (procExist == procNumInTble) {
		throw "Error: Procedure: " + procCalls + " calling itself!.\n";
		exit(0);
	}

	addToParent(index);
	callsPair.first = procNumInTble;
	callsPair.second = procCalls;
	callsLink.push_back(callsPair);

	stmtNoCallPair.first = index - numOfProc - numOfElse;
	stmtNoCallPair.second = procCalls;
	stmtNoAndCalls.push_back(stmtNoCallPair);
}

void Parser::processWhile(int index, string statement) {
	pushOpenBracket();
	pair <int, int> parentPair;
	pair <int, string> pair;
	pair.first = index;
	pair.second = statement;
	if (!containerElements.empty()) {
		parentPair.first = containerElements.back().first - numOfProc - numOfElse;
		parentPair.second = index - numOfProc - numOfElse;
		parentLink.push_back(parentPair);
	}
	containerElements.push_back(pair);

}

void Parser::processIf(int index, string statement)
{
	pushOpenBracket();
	pair <int, string> ifPair;
	pair <int, int> parentPair;
	pair <int, string> pair;
	pair.first = index;
	pair.second = statement;

	ifPair.first = index-numOfElse;
	ifPair.second = statement;
	
	ifStmtVec.push(ifPair);
	if (!containerElements.empty()) {
		parentPair.first = containerElements.back().first - numOfProc - numOfElse;
		parentPair.second = index - numOfProc - numOfElse;
		parentLink.push_back(parentPair);
		//containerElements.push_back(ifPair);
	}
	containerElements.push_back(pair);
	addToParent(pair.first);
	handleModifyAndUses(pair.first, pair.second);
	handleFollows(pair.first, pair.second);

}

void Parser::processElse(int index, string statement) {
	pushOpenBracket();
	numOfElse++;
	containerElements.push_back(ifStmtVec.top());
	ifStmtVec.pop();
}

bool Parser::isOperator(char o) {
	bool isOp = false;
	if (o == '+' || o == '-' || o == '*') {
		isOp = true;
	}
	return isOp;
}

void Parser::processExpressions(int index, string statement) {
	list<char> output;
	stack<char> stack;
	output.clear();
	string s;
	for (char c : statement) {
		char charac = c;
		if (c == ';') {
			addToParent(index);
			if (!isConstant(s)) {
				int index = pkb->setVarName(s);
				pkb->setProcNames(index, currProcName);
			}
			s = "";		
		}
		if (c == '}') {
			pushCloseBracket(index);
		}
		if (isOperator(charac))
		{
			char o1 = charac;
			if (!isConstant(s)) {
				int index = pkb->setVarName(s);
				pkb->setProcNames(index, currProcName);
			}
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
				Error();
			}
		}
		else
		{
			if (charac == '=') {
				output.clear();
			//	handleModifyAndUses(index, statement);
				if (!isConstant(s)) {
					int index = pkb->setVarName(s);
					pkb->setProcNames(index, currProcName);
				}
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
			Error();
		}
		if (stackTop != '}') {
			output.push_back(' ');
			output.push_back(stackTop);
		}
		stack.pop();
	}
	setExprInStmtTable(index, output);
}

void Parser::handleModifyAndUses(int i, string stmt) {
	int bracket = 0;
	list<pair<int, string>> tempList;
	if (!closeBracket.size()) {
		bracket = closeBracket.size();
	}
	replace(stmt.begin(), stmt.end(), '(', ' ');
	replace(stmt.begin(), stmt.end(), ')', ' ');
	remove_if(stmt.begin(), stmt.end(), isspace);

	//add while var into varTable uses
	if (stmt.find("while") != std::string::npos) {
		size_t bracketPos = stmt.find("{");
		stmt.replace(bracketPos, string("{").length(), "");
		string varInWhile = stmt.substr(stmt.find("while") + 5);
		int index = pkb->setVarName(varInWhile);
		pkb->setControlVar(i - numOfProc - numOfElse, index);
		if (!containerElements.empty()) {
			pair<int, string> pairedParent = containerElements.back();
			int parentUse = pairedParent.first - numOfProc - numOfElse;
			if (!isConstant(varInWhile)) {
				pkb->setUsedBy(varInWhile, parentUse);
				pkb->setUsedVar(parentUse, varInWhile);
			}
		}
		pkb->setProcNames(index, currProcName);
		if (!isConstant(varInWhile)) {
			
			pkb->setUsedBy(varInWhile, i - numOfProc - numOfElse);
			pkb->setUsedVar(i - numOfProc - numOfElse, varInWhile);
			varUsedInProc.push_back(make_pair(procNumInTble,varInWhile));
		}
	}
	else if (stmt.find("if") != std::string::npos) {
		size_t bracketPos = stmt.find("{");
		stmt.replace(bracketPos, string("{").length(), "");
		size_t ifstmt = stmt.find("if") + 2;
		size_t thenstmt = stmt.find("then") - 2;
		string varInIf = stmt.substr(ifstmt, thenstmt);
		int index = pkb->setVarName(varInIf);
		pkb->setControlVar(i - numOfProc - numOfElse, index);
		
		if (!containerElements.empty()) {
			pair<int, string> pairedParent = containerElements.back();
			int parentUse = pairedParent.first - numOfProc - numOfElse;
			if (!isConstant(varInIf)) {
				pkb->setUsedBy(varInIf, parentUse);
				pkb->setUsedVar(parentUse, varInIf);
			}
		}
		pkb->setProcNames(index, currProcName);
		if (!isConstant(varInIf)) {
			pkb->setUsedBy(varInIf, i - numOfProc - numOfElse);
			pkb->setUsedVar(i - numOfProc - numOfElse, varInIf);
			varUsedInProc.push_back(make_pair(procNumInTble, varInIf));
		}
	}

	else {
		size_t equal = stmt.find("=");
		string modified = stmt.substr(0, equal);
		string s;
		if (!containerElements.empty()) {
			pair<int, string> pairedParent = containerElements.back();
			int parentMod = pairedParent.first - numOfProc - numOfElse;

			string parentStmt = pairedParent.second;
			if (parentStmt.find("while") != std::string::npos && currElse >(pairedParent.first - numOfProc)) {
				parentMod = pairedParent.first - numOfProc;
			}
			if (parentStmt.find("if") != std::string::npos && currElse >(pairedParent.first - numOfProc)) {
				parentMod = pairedParent.first - numOfProc - numOfElse + 1;
			//	cout << "added\n";
			}

			if (!isConstant(modified)) {
				pkb->setModifiedBy(modified, parentMod);
				pkb->setModifies(parentMod, modified);
			}
		}
		if (!isConstant(modified)) {
			pair<int, string> varAndProc;
			varAndProc.first = procNumInTble;
			varAndProc.second = modified;
			pkb->setModifiedBy(modified, i - numOfProc - numOfElse);
			pkb->setModifies(i - numOfProc - numOfElse, modified);
			varModifiedInProc.push_back(varAndProc);
		}

		for (char c : stmt.substr(equal + 1, stmt.size())) {
			if (isOperator(c) || c == '}' || c == ';') {
				if (!s.empty()) {
					if (!containerElements.empty()) {
						pair<int, string> pairedParent = containerElements.back();
						int parentUse = pairedParent.first - numOfProc - numOfElse;

						string parentStmt = pairedParent.second;
						if (parentStmt.find("while") != std::string::npos && currElse >(pairedParent.first - numOfProc)) {
							parentUse = pairedParent.first - numOfProc;
						}
						if (parentStmt.find("if") != std::string::npos && currElse >(pairedParent.first - numOfProc)) {
							parentUse = pairedParent.first - numOfProc - numOfElse + 1;
						}

						if (!isConstant(s)) {

							pkb->setUsedBy(s, parentUse);
							pkb->setUsedVar(parentUse, s);
						}
					}
					if (isConstant(s)) {
						int constant = atoi(s.c_str());
						int constantInd = pkb->setConstant(constant);
						pkb->setStmtUsed(constantInd, i - numOfProc - numOfElse);
					}
					if (!isConstant(s)) {
						pkb->setUsedBy(s, i - numOfProc - numOfElse);
						pkb->setUsedVar(i - numOfProc - numOfElse, s);
						varUsedInProc.push_back(make_pair(procNumInTble, s));
					}
				}
				s = "";
			}
			else {
				s.push_back(c);

			}
		}
	}
}

//method for testing
string Parser::getParentChild() {
	string output;
	while (!parentLink.empty()) {
		pair<int, int> parentChild = parentLink.back();
		int parent = parentChild.first;
		int child = parentChild.second;
		output.append("Parent: " + to_string(parent) + " Child: " + to_string(child) + "| ");
		if (!parentLink.empty()) {
			parentLink.pop_back();
		}
	}
	return output;
}

string Parser::getExpression() {
	list<pair<int, string >>::iterator i;
	string output;
	int counter = 0;
	for (i = (stmtNoAndExpr).begin(); i != (stmtNoAndExpr).end(); ++i) {
		counter++;
		if ((*i).second != ";") {
			output.append(to_string(counter) + ": " + (*i).second + " ");
		}
	}
	return output;
}

string Parser::getFollow() {
	string output;
	while (!followLink.empty()) {
		pair<int, int> followPair = followLink.back();
		int firstNum = followPair.first;
		int secondNum = followPair.second;
		output.append(to_string(firstNum) + "->" + to_string(secondNum) + "|");
		if (!followLink.empty()) {
			followLink.pop_back();
		}
	}
	return output;
}

void Parser::setExprInStmtTable(int index, list<char> exprOutput) {
	pair<int, string> pairs;
	pairs.first = index - numOfProc - numOfElse;
	string s;
	for (list<char>::iterator it = exprOutput.begin(); it != exprOutput.end(); ++it) {
		if (*it != ';') {
			s.push_back(*it);
		}
	}

	pkb->setRightExpr(pairs.first, s);
	if (!s.empty()) {
		pairs.second = s;
	}
	stmtNoAndExpr.push_back(pairs);
}

void Parser::Error() {
	throw "ProcName: " + procName + " does not exist.\n";
}

int Parser::isPriority(const char &c)
{
	if (c == '^')
	{
		return 3;
	}
	if (c == '*')
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

void Parser::pushCloseBracket(int stmtNum) {
	closeBracket.push('}');
	if (!containerElements.empty()) {
		//cout << "Container index: " << containerElements.back().first << "\n\n";
		//cout << "Container: " << containerElements.back().second<< "\n\n";
		containerElements.pop_back();
	}
}

void Parser::setProcEndNum(int procNum,int stmtNum) {
	//cout << "size: " << containerElements.size();
	if (containerElements.empty()) {
		pkb->setProcModified(varModifiedInProc);
		pkb->setProcUses(varUsedInProc);
		pkb->setEndNum(procNum, stmtNum - numOfProc - numOfElse);
		varUsedInProc.clear();
		varModifiedInProc.clear();
	}
}

void Parser::pushOpenBracket() {
	openBracket.push('{');
}

bool Parser::isVariable(char c) {
	bool isAlphabet = false;
	if (isalpha(c)) {
		isAlphabet = true;
	}
	return isAlphabet;
}

bool Parser::isConstant(string s) {
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it))
	{
		++it;
	}
	return !s.empty() && it == s.end();
}

void Parser::handleFollows(int index, string stmt) {
	string currStmt = stmt;
	pair<int, int> paired;
	if (prevStmt.empty()) {
		prevStmt = stmt;
		currFollows.push_back(index - numOfProc - numOfElse);
	}
	else
	{
		if (prevStmt.find("{") != std::string::npos) {
			currFollows.push_back(index - numOfProc - numOfElse);
			prevStmt = currStmt;
		}
		else if (prevStmt.find("}") != std::string::npos) {
			if (currStmt.find("else") != std::string::npos) {
				prevStmt = currStmt;
				currFollows.pop_back();
			}

			else {
				size_t n = count(prevStmt.begin(), prevStmt.end(), '}');
				//cout << prevStmt << "\n";
				for (size_t i = 0; i < n;i++) {
					if (!currFollows.empty()) {
						currFollows.pop_back();
					}
				}

				paired.first = currFollows.back();
				paired.second = index - numOfProc - numOfElse;

				if (!currFollows.empty()) {
					currFollows.pop_back();
				}
				currFollows.push_back(index - numOfProc - numOfElse);
				followLink.push_back(paired);
				prevStmt = currStmt;
			}
		}
		else {

			paired.first = currFollows.back();
			paired.second = index - numOfProc - numOfElse;

			if (!currFollows.empty()) {
				currFollows.pop_back();
			}
			currFollows.push_back(index - numOfProc - numOfElse);
			followLink.push_back(paired);
			prevStmt = currStmt;
		}
	}
}