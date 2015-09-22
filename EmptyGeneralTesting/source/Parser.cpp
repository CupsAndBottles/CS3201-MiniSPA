#include "Parser.h"
#include "iostream"
#include <fstream>
#include <string>
#include <stack>
#include <regex>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>
#include <map>
#include "VarTable.h"
#include "StmtTable.h"
#include "ProcTable.h"
#include "PKB.h"

using namespace std;
vector<string> tokens;
list<pair<int, string>> listOfStatements;
list<pair<int, int>> parentLink;
list<pair<int, int>> followLink;
list<pair<int, string>> stmtNoAndExpr;
string currProcName;
int currIndex = 0;
int numOfProc = 0;
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
		cout << "Unable to open file";
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
	/*procedure{
	stmtlst
	}stmlst - if while process while*/
	list<pair<int, string >>::iterator i;

	for (i = (lines).begin(); i != (lines).end(); ++i) {
		//cout << ((*i).first) << endl << ((*i).second) << endl;
		pair <int, string> pair;
		string stmt = (*i).second;
		//remove procedure line, send to stmtlst
		if (stmt.find("procedure") != std::string::npos) {
			processProcedure((*i).first, (*i).second);
			pair.first = (*i).first;
			pair.second = (*i).second;
			containerElements.push_back(pair);
			//insert firstStmtNum
		}
		else if (stmt.find("while") != std::string::npos) {
			processWhile((*i).first, (*i).second);
			pair.first = (*i).first;
			pair.second = (*i).second;
			containerElements.push_back(pair);
			addToParent(pair.first);
			processModifyAndUses(pair.first, pair.second);
			handleFollows(pair.first, pair.second);

		}
		else {
			processExpressions((*i).first, (*i).second);
			processModifyAndUses((*i).first, (*i).second);
			handleFollows((*i).first, (*i).second);
		}
	}
	processParentChild();
	processFollow();
}

void Parser::addToParent(int child) {
	pair<int, string> parentPair;
	int parent = 0;
	if (!containerElements.empty()) {
		parentPair = containerElements.back();
		string parentStmt = parentPair.second;
		size_t procedureStmt = parentStmt.find("procedure");
		if (procedureStmt == std::string::npos) {
			pair<int, int> pairs;
			int parent = parentPair.first - numOfProc;
			//insert into stmt table
			int newChild = child - numOfProc;
			if (parent != newChild) {
				//cout << "\nParent : " << parent << "\n";
				//cout << "\nChild : " << newChild << "\n";
				pairs.first = parent;
				pairs.second = newChild;
				parentLink.push_back(pairs);
			}
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

}

void Parser::processWhile(int index, string statement) {
	//size_t bracketPos = statement.find("{");
	pushOpenBracket();
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

	for (char c : statement) {
		char charac = c;
		if (c == ';') {
			addToParent(index);
			break;
		}
		if (c == '}') {
			pushCloseBracket(index);
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
				Error();
			}
		}
		else
		{
			if (charac == '=') {
				processModifyAndUses(index, statement);
				//output.pop_back();

			}
			else {
				output.push_back(charac);
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

void Parser::processModifyAndUses(int i, string stmt) {
	/*	cout << "\n" <<"Index: "<< i-numOfProc-closeBracket.size() << "\n";
	cout << "Var: " << var << "\n";
	cout << "Stmt: " << stmt << "\n";*/
	int bracket = 0;
	if (!closeBracket.size()) {
		bracket = closeBracket.size();
	}
	//add while var into varTable uses
	if (stmt.find("while") != std::string::npos) {
		size_t bracketPos = stmt.find("{");
		stmt.replace(bracketPos, string("{").length(), "");
		string varInWhile = stmt.substr(stmt.find("while") + 5);
	
	}
	else {
		size_t equal = stmt.find("=");
		char modified = stmt.at(0);
		string s;
		s.push_back(modified);

		for (char c : stmt.substr(equal + 1, stmt.size())) {
			if (isVariable(c)) {
		
			}
		}
	}
}

//need to insert into tables, but interferes with testing
void Parser::processParentChild() {
	/*while (!parentLink.empty()) {
		pair<int, int> parentChild = parentLink.back();
		int parent = parentChild.first;
		int child = parentChild.second;

	if (!parentLink.empty()) {
			parentLink.pop_back();
		}
	}*/

}

string Parser::getParentChild() {
	string output;
	while (!parentLink.empty()) {
		pair<int, int> parentChild = parentLink.back();
		int parent = parentChild.first;
		int child = parentChild.second;
		output.append(" Parent: " + to_string(parent) + " Child: " + to_string(child));
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
		output.append(to_string(counter)+": "+ (*i).second+" ");
	}
	cout << output;
	return output;
}
//interferes with follows testing
void Parser::processFollow() {
	/*string output;
	while (!followLink.empty()) {
		pair<int, int> followPair = followLink.front();
		int firstNum = followPair.first;
		int secondNum = followPair.second;
		output.append("|" + to_string(firstNum) + " " + to_string(secondNum) + "|");
		if (!followLink.empty()) {
			followLink.pop_front();
		}
	}*/
//	return output;
}

string Parser::getFollow() {
	string output;
	while (!followLink.empty()) {
		pair<int, int> followPair = followLink.front();
		int firstNum = followPair.first;
		int secondNum = followPair.second;
		output.append("|" + to_string(firstNum) + "->" + to_string(secondNum) + "|");
		if (!followLink.empty()) {
			followLink.pop_front();
		}
	}
		return output;
}

void Parser::insertIntoStmtTable(int typeOfStat, int i, int stmtNum, int var) {
	int parent;
	int child;

	int follows;
	int followedBy;
	vector<int> parentT;
	vector<int> childrenT;
	vector<int> followsT;
	vector<int> followedByT;
	int uses;
	int modify;
	string rExpr;

	for (list<pair<int, string>>::iterator it = stmtNoAndExpr.begin(); it != stmtNoAndExpr.end(); ++it) {
		if ((*it).first = stmtNum) {
			rExpr = (*it).second;
		}
		else {
			rExpr = "";
		}
	}

	if (i = 0) {//modify
		modify = var;
	//	stmtTable.insertStmt(typeOfStat, 0, 0, 0, 0, parentT, childrenT, followsT, followedByT, 0, modify, rExpr);
	}
	else if (i = 1) {
		uses = var;
		//stmtTable.insertStmt(typeOfStat, 0, 0, 0, 0, parentT, childrenT, followsT, followedByT, uses, 0, rExpr);
	}
}
void Parser::setExprInStmtTable(int index, list<char> exprOutput) {
	pair<int, string> pairs;
	pairs.first = index - 1;
	string s;
	for (list<char>::iterator it = exprOutput.begin(); it != exprOutput.end(); ++it) {

		s.push_back(*it);
	}
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
		//	cBrack++;
	}
	setProcEndNum(stmtNum);
}

void Parser::setProcEndNum(int stmtNum) {

	if (containerElements.empty()) {
		//procTable.setEndStmtNo(stmtNum - numOfProc, currProcName);
		//cBrack = 0;
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
		currFollows.push_back(index);
	}
	else {
		if (currStmt.find("{") != std::string::npos) {
			paired.first = currFollows.back() - numOfProc;
			paired.second = index - numOfProc;
			//currFollows.pop_back();
			currFollows.push_back(index + 1);

			followLink.push_back(paired);

			prevStmt = currStmt;
		}
		else if (currStmt.find("}") != std::string::npos) {
			if (currStmt != "}") {
				paired.first = currFollows.back() - numOfProc;
				paired.second = index - numOfProc;
				if (!currFollows.empty()) {
					currFollows.pop_back();
				}
				prevStmt = currStmt;
				followLink.push_back(paired);
			}
			else {
				if (!currFollows.empty()) {
					currFollows.pop_back();
				}
			}

		}
		else {
			paired.first = currFollows.back() - numOfProc;
			paired.second = index - closeBracket.size() - numOfProc;
			prevStmt = currStmt;
			if (paired.first != paired.second) {
				followLink.push_back(paired);
			}
		}
	}
}