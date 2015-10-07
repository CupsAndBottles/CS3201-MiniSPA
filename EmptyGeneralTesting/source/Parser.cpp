#include "Parser.h"
#include "iostream"
#include <fstream>
#include <string>
#include <stack>
#include <sstream>
#include <iterator>
#include <list>
#include "PKB.h"

using namespace std;
vector<string> tokens;
list<pair<int, string>> listOfStatements;
vector<pair<int, int>> parentLink;
vector<pair<int, int>> followLink;
list<pair<int, string>> stmtNoAndExpr;
string currProcName;
int currIndex = 0;
int numOfProc = 0;
int procNumInTble = 0;
vector<string> varModifiedInProc;
vector<string> varUsedInProc;
vector<int> currFollows;
stack<char> closeBracket;
stack<char> openBracket;
list<pair<int, string>> containerElements;
string prevStmt;

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
			size_t comments = currLine.find("//");
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
			pair.second = toLowerCase(stmtToStore);
			result.push_back(pair);


		}
		lines = lines.substr(position + 1);
	}

	return result;
}


void Parser::Procedure() {
	list<pair<int, string >>::iterator i;

	for (i = (lines).begin(); i != (lines).end(); ++i) {
		string stmt = (*i).second;
		if (stmt.find("procedure") != std::string::npos) {
			processProcedure((*i).first, (*i).second);
		}
		else if (stmt.find("while") != std::string::npos) {
			pkb->setType(Enum::WHILE);
			processWhile((*i).first, (*i).second);
		}
		else if (stmt.find("if") != std::string::npos) {
			//processIf();
		}
		else {
			pkb->setType(Enum::ASSIGN);
			processExpressions((*i).first, (*i).second);
			handleModifyAndUses((*i).first, (*i).second);
			handleFollows((*i).first, (*i).second);
			
		}
	}
	setRelationsInTable();
}

void Parser::setRelationsInTable() {
	pkb->setChildren(parentLink);
	pkb->setFollows(followLink);
}

void Parser::addToParent(int child) {
	pair<int, string> parentPair;
	int parent = 0;
	if (!containerElements.empty()) {
		parentPair = containerElements.back();
		string parentStmt = parentPair.second;
		pair<int, int> pairs;
		int parent = parentPair.first - numOfProc;
		int newChild = child - numOfProc;
		if (parent != newChild) {
			pairs.first = parent;
			pairs.second = newChild;
			parentLink.push_back(pairs);
		}
	}

}

string Parser::toLowerCase(string s) {
	string stmtInLC;
	for (char c : s) {
		stmtInLC += tolower(c);
	}
	return stmtInLC;
}
void Parser::processProcedure(int index, string statement) {
	size_t bracketPos = statement.find("{");
	statement.replace(bracketPos, string("{").length(), "");
	pushOpenBracket();
	string procName = statement.substr(statement.find("procedure") + 9);
	currProcName = procName;
	numOfProc++;
	procNumInTble = pkb->setProcNameInProcTable(statement);
	pkb->setStartNum(procNumInTble, index);

}

void Parser::processWhile(int index, string statement) {
	pushOpenBracket();
	pair <int, int> parentPair;
	pair <int, string> pair;
	pair.first = index;
	pair.second = statement;
	if (!containerElements.empty()) {
		parentPair.first = containerElements.back().first - numOfProc;
		parentPair.second = index - numOfProc;
		parentLink.push_back(parentPair);
	}
	containerElements.push_back(pair);
	addToParent(pair.first);
	handleModifyAndUses(pair.first, pair.second);
	handleFollows(pair.first, pair.second);
}

bool Parser::isOperator(char o) {
	bool isOp = false;
	if (o == '+' || o == '-' || o == '/' || o == '*') {
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
			int index = pkb->setVarName(s);
			pkb->setProcNames(index, currProcName);
			s = "";
		//	break;
		}
		if (c == '}') {

			pushCloseBracket(index);
			break;
		}
		if (isOperator(charac))
		{
			char o1 = charac;
			int index = pkb->setVarName(s);
			pkb->setProcNames(index, currProcName);
			s = "";

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
				Error();
			}
		}
		else
		{
			if (charac == '=') {
				handleModifyAndUses(index, statement);
				int index = pkb->setVarName(s);
				pkb->setProcNames(index, currProcName);
				s = "";

			}
			else {
				output.push_back(charac);
				s.push_back(charac);
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
		output.push_back(stackTop);
		stack.pop();
	}
	setExprInStmtTable(index, output);

}
//end of method

void Parser::handleModifyAndUses(int i, string stmt) {
	int bracket = 0;
	if (!closeBracket.size()) {
		bracket = closeBracket.size();
	}
	//add while var into varTable uses
	if (stmt.find("while") != std::string::npos) {
		size_t bracketPos = stmt.find("{");
		stmt.replace(bracketPos, string("{").length(), "");
		string varInWhile = stmt.substr(stmt.find("while") + 5);
		int index = pkb->setVarName(varInWhile);
		pkb->setProcNames(index, currProcName);
		pkb->setUsedBy(varInWhile, i - numOfProc);
		pkb->setUsedVar(i - numOfProc,varInWhile);
		varUsedInProc.push_back(varInWhile);
	}
	else {
		size_t equal = stmt.find("=");
		char modified = stmt.at(0);
		string s;
		s.push_back(modified);
		pkb->setModifiedBy(s, i - numOfProc);
		pkb->setModifies(i-numOfProc,s);
		varModifiedInProc.push_back(s);
		for (char c : stmt.substr(equal + 1, stmt.size())) {
			s = "";
			if (isVariable(c)) {
				s.push_back(c);
				pkb->setUsedBy(s, i - numOfProc);
				pkb->setUsedVar(i - numOfProc, s);
				varUsedInProc.push_back(s);
			}
		}
	}
}

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
	pairs.first = index - 1;
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
	cout << "Error Parsing";
}

int Parser::isPriority(const char &c)
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

void Parser::pushCloseBracket(int stmtNum) {
	closeBracket.push('}');
	if (!containerElements.empty()) {
		containerElements.pop_back();
	}

	setProcEndNum(stmtNum);
}

void Parser::setProcEndNum(int stmtNum) {

	if (containerElements.empty()) {
		pkb->setEndNum(procNumInTble, stmtNum - numOfProc);
		pkb->setProcModified(procNumInTble, varModifiedInProc);
		pkb->setProcUses(procNumInTble,varUsedInProc);

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


bool Parser::isConstant(char c) {
	bool isConstant = false;
	if (isdigit(c)) {
		isConstant = true;
	}
	return isConstant;
}

void Parser::handleFollows(int index, string stmt) {
	string currStmt = stmt;
	pair<int, int> paired;
	
	if (prevStmt.empty()) {
		prevStmt = stmt;
		currFollows.push_back(index - numOfProc);
	}
	else
	{
		if (prevStmt.find("{") != std::string::npos) {
			currFollows.push_back(index - numOfProc);
			prevStmt = currStmt;
		}
		else if (prevStmt.find("}") != std::string::npos) {
			size_t n = std::count(prevStmt.begin(), prevStmt.end(), '}');
			for (int i = 0; i < n;i++) {
				if (!currFollows.empty()) {
					currFollows.pop_back();
				}
			}
			paired.first = currFollows.back();
			paired.second = index - numOfProc;
			currFollows.pop_back();
			currFollows.push_back(index - numOfProc);
			followLink.push_back(paired);
			prevStmt = currStmt;
		}
		else {
			paired.first = currFollows.back();
			paired.second = index - numOfProc;
			currFollows.pop_back();
			currFollows.push_back(index - numOfProc);
			followLink.push_back(paired);
			prevStmt = currStmt;
		}
	}
}