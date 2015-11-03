#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(PQL_Select)
		{
			PKB *pkb = new PKB();

			// Sets stmts
			pkb->setType(Enum::TYPE::ASSIGN); // stmt 1: assignment stmt
			pkb->setType(Enum::TYPE::WHILE); // stmt 2: while stmt
			pkb->setType(Enum::TYPE::ASSIGN); // stmt 3: assignment stmt

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);


			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "1", "3" };

			Assert::IsTrue(expectedResults == results);
		}

		TEST_METHOD(PQL_Follows) {
			/*********************** Test Code ************************/
			/*
			procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			inspiration = beads + command + coffee;		\\5
			while command {								\\6
			x = x + 9;}								\\7
			x = beads + command; }						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<pair<int, string>> varUsed;
			varUsed.push_back(make_pair(0, "command"));
			varUsed.push_back(make_pair(0, "inspiration"));
			varUsed.push_back(make_pair(0, "coffee"));
			varUsed.push_back(make_pair(0, "beads"));
			varUsed.push_back(make_pair(0, "x"));

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(6, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
			parent.push_back(make_pair(4, 8));
			parent.push_back(make_pair(6, 7));
			pkb->setChildren(parent);

			// Statement 1 - set constant as variables?
			pkb->setVarName("beads");
			pkb->setVarName("command");
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);
			pkb->setConstant(34);
			pkb->setStmtUsed(pkb->getConstantIndex(34), 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration 1 +");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);
			pkb->setConstant(1);
			pkb->setStmtUsed(pkb->getConstantIndex(1), 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setControlVar(4, pkb->getVarIndex("coffee"));

			// statement 5
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "inspiration");
			pkb->setModifiedBy("inspiration", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "inspiration");
			pkb->setModifiedBy("inspiration", 4);

			// statement 6
			pkb->setControlVar(6, pkb->getVarIndex("command"));
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);

			// statement 7
			pkb->setVarName("x");
			pkb->setUsedVar(7, "x");
			pkb->setUsedBy("x", 7);
			pkb->setUsedVar(6, "x");
			pkb->setUsedBy("x", 6);
			pkb->setUsedVar(4, "x");
			pkb->setUsedBy("x", 4);
			pkb->setRightExpr(7, "xx9*+");
			pkb->setModifies(7, "x");
			pkb->setModifiedBy("x", 7);
			pkb->setModifies(6, "x");
			pkb->setModifiedBy("x", 6);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);
			pkb->setConstant(9);
			pkb->setStmtUsed(pkb->getConstantIndex(9), 7);


			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beads command +");
			pkb->setModifies(8, "x");
			pkb->setModifiedBy("x", 8);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);


			pkb->setProcUses(varUsed);

			ParserForPQL parserPQL = ParserForPQL("stmt s; Select s such that Follows(1, 2)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "1", "2", "3", "4", "5", "6","7", "8" };
			Assert::IsTrue(expectedResults == results);
		}

		TEST_METHOD(PQL_With) {
			/*********************** Test Code ************************/
			/*	procedure dream {
			beads = command + 10;							\\1
			dream = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			inspiration = beads + command + coffee;		\\5
			while command {								\\6
			x = x * 9;}								\\7
			x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<pair<int, string>> varUsed;
			varUsed.push_back(make_pair(0, "command"));
			varUsed.push_back(make_pair(0, "inspiration"));
			varUsed.push_back(make_pair(0, "coffee"));
			varUsed.push_back(make_pair(0, "beads"));
			varUsed.push_back(make_pair(0, "x"));

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(6, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
			parent.push_back(make_pair(4, 8));
			parent.push_back(make_pair(6, 7));
			pkb->setChildren(parent);

			// Statement 1 - set constant as variables?
			pkb->setVarName("beads");
			pkb->setVarName("command");
			pkb->setRightExpr(1, "command10+");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
			pkb->setConstant(34);
			pkb->setStmtUsed(pkb->getConstantIndex(34), 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);
			pkb->setConstant(1);
			pkb->setStmtUsed(pkb->getConstantIndex(1), 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setControlVar(4, pkb->getVarIndex("coffee"));

			//Statement 5
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "inspiration");
			pkb->setModifiedBy("inspiration", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "inspiration");
			pkb->setModifiedBy("inspiration", 4);

			// statement 6
			pkb->setControlVar(6, pkb->getVarIndex("command"));
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);

			// statement 7
			pkb->setVarName("x");
			pkb->setUsedVar(7, "x");
			pkb->setUsedBy("x", 7);
			pkb->setUsedVar(6, "x");
			pkb->setUsedBy("x", 6);
			pkb->setUsedVar(4, "x");
			pkb->setUsedBy("x", 4);
			pkb->setRightExpr(7, "xx9*+");
			pkb->setModifies(7, "x");
			pkb->setModifiedBy("x", 7);
			pkb->setModifies(6, "x");
			pkb->setModifiedBy("x", 6);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);
			pkb->setConstant(9);
			pkb->setStmtUsed(pkb->getConstantIndex(9), 7);

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");
			pkb->setModifies(8, "x");
			pkb->setModifiedBy("x", 8);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);


			pkb->setProcUses(varUsed);

			ParserForPQL parserPQL = ParserForPQL("procedure p; variable v; Select v with v.varName = p.procName");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string expectedResults = "dream";
			string outputString;
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}

			Assert::AreEqual(expectedResults, outputString);
		}

		TEST_METHOD(PQL_Boolean) {
			/*********************** Test Code ************************/
			/*	procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			inspiration = beads + command + coffee;		\\5
			while command {								\\6
			x = x * 9;}								\\7
			x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<pair<int, string>> varUsed;
			varUsed.push_back(make_pair(0, "command"));
			varUsed.push_back(make_pair(0, "inspiration"));
			varUsed.push_back(make_pair(0, "coffee"));
			varUsed.push_back(make_pair(0, "beads"));
			varUsed.push_back(make_pair(0, "x"));

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(6, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
			parent.push_back(make_pair(4, 8));
			parent.push_back(make_pair(6, 7));
			pkb->setChildren(parent);

			// Statement 1 - set constant as variables?
			pkb->setVarName("beads");
			pkb->setVarName("command");
			pkb->setRightExpr(1, "command10+");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);
			pkb->setConstant(34);
			pkb->setStmtUsed(pkb->getConstantIndex(34), 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);
			pkb->setConstant(1);
			pkb->setStmtUsed(pkb->getConstantIndex(1), 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setControlVar(4, pkb->getVarIndex("coffee"));

			//Statement 5
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "inspiration");
			pkb->setModifiedBy("inspiration", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "inspiration");
			pkb->setModifiedBy("inspiration", 4);

			// statement 6
			pkb->setControlVar(6, pkb->getVarIndex("command"));
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);

			// statement 7
			pkb->setVarName("x");
			pkb->setUsedVar(7, "x");
			pkb->setUsedBy("x", 7);
			pkb->setUsedVar(6, "x");
			pkb->setUsedBy("x", 6);
			pkb->setUsedVar(4, "x");
			pkb->setUsedBy("x", 4);
			pkb->setRightExpr(7, "xx9*+");
			pkb->setModifies(7, "x");
			pkb->setModifiedBy("x", 7);
			pkb->setModifies(6, "x");
			pkb->setModifiedBy("x", 6);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);
			pkb->setConstant(9);
			pkb->setStmtUsed(pkb->getConstantIndex(9), 7);

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");
			pkb->setModifies(8, "x");
			pkb->setModifiedBy("x", 8);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);


			pkb->setProcUses(varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select BOOLEAN such that Modifies(a, \"x\") with a.stmt# = 10");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string expectedResults = "false";
			string outputString;
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}

			Assert::AreEqual(expectedResults, outputString);
		}

		TEST_METHOD(PQL_3Clauses) {
			/*********************** Test Code ************************/
			/*	procedure dream {
			beads = command + 10;							\\1
			dream = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			inspiration = beads + command + coffee;			\\5
			while command {								\\6
			x = x * 9;}								\\7
			x = beads + command;						\\8
			call hope; }}								\\9

			procedure hope {
			bye = moonlight * 7;							\\10
			}
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 9);

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);   //8
			pkb->setType(Enum::TYPE::CALLS);	//9

			vector<pair<int, string>> varUsed;
			varUsed.push_back(make_pair(0, "command"));
			varUsed.push_back(make_pair(0, "inspiration"));
			varUsed.push_back(make_pair(0, "coffee"));
			varUsed.push_back(make_pair(0, "beads"));
			varUsed.push_back(make_pair(0, "x"));

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(6, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
			parent.push_back(make_pair(4, 8));
			parent.push_back(make_pair(6, 7));
			pkb->setChildren(parent);

			// Statement 1 - set constant as variables?
			pkb->setVarName("beads");
			pkb->setVarName("command");
			pkb->setRightExpr(1, "command10+");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
			pkb->setConstant(34);
			pkb->setStmtUsed(pkb->getConstantIndex(34), 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);
			pkb->setConstant(1);
			pkb->setStmtUsed(pkb->getConstantIndex(1), 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setControlVar(4, pkb->getVarIndex("coffee"));

			//Statement 5
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "inspiration");
			pkb->setModifiedBy("inspiration", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "inspiration");
			pkb->setModifiedBy("inspiration", 4);

			// statement 6
			pkb->setControlVar(6, pkb->getVarIndex("command"));
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);

			// statement 7
			pkb->setVarName("x");
			pkb->setUsedVar(7, "x");
			pkb->setUsedBy("x", 7);
			pkb->setUsedVar(6, "x");
			pkb->setUsedBy("x", 6);
			pkb->setUsedVar(4, "x");
			pkb->setUsedBy("x", 4);
			pkb->setRightExpr(7, "xx9*+");
			pkb->setModifies(7, "x");
			pkb->setModifiedBy("x", 7);
			pkb->setModifies(6, "x");
			pkb->setModifiedBy("x", 6);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);
			pkb->setConstant(9);
			pkb->setStmtUsed(pkb->getConstantIndex(9), 7);

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");
			pkb->setModifies(8, "x");
			pkb->setModifiedBy("x", 8);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);

			// statement 9
			pkb->setProcNameInProcTable("hope");
			vector<pair<int, string>> calledProc;
			calledProc.push_back(make_pair(0, "hope"));
			pkb->setProcCalls(calledProc);
			pkb->setProcCalledBy(1, 0);

			pkb->setProcUses(varUsed);

			pkb->setType(Enum::TYPE::ASSIGN);	//10
			pkb->setStartNum(1, 10);
			pkb->setEndNum(1, 10);

			pkb->setProcCallsT(0, { 1 });
			pkb->setProcCalledByT(1, { 0 });

			// statement 10
			varUsed = { make_pair(1, "moonlight") };
			pkb->setProcUses(varUsed);
			pkb->setVarName("bye");
			pkb->setVarName("moonlight");
			pkb->setRightExpr(10, "moonlight7*");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });

			ParserForPQL parserPQL = ParserForPQL("assign a; procedure p, q; call c; Select p pattern a(\"command\", _) such that Calls(p, q) with c.procName = \"hope\"");

			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(parserPQL.getQueryTree());
			list<string> expectedResults = { "dream" };

			Assert::IsTrue(expectedResults == results);
		}

	};
}