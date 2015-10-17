#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "ParserForPQL.h"
#include "PKB.h"
#include "Enum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator)
	{
	public:
		TEST_METHOD(QE_EvaluateSelectClause) {
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

		TEST_METHOD(QE_EvaluateSuchThatFollowsGiven) {
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

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("stmt s; Select s such that Follows(1, 2)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "1", "2", "3", "4", "5", "6","7", "8" };
			Assert::IsTrue(expectedResults == results);
		}

		TEST_METHOD(QE_EvaluateSuchThatParentUnderscore) {
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

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("while w; Select w such that Parent(w, _)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);

			list<string> expectedResults = { "4", "6" };

			Assert::IsTrue(expectedResults == results);
		}

		TEST_METHOD(QE_EvaluateSuchThatUsesNoCommonSyn) {
		/*********************** Test Code ************************/
		/*	procedure dream {
				beads = command + 10;							\\1
				beads = 34;										\\2
				command = inspiration + 1;						\\3
				while coffee {									\\4
					stamps = beads + command + coffee;			\\5
					while command {								\\6
						x = x * 9;}								\\7
					x = beads + command; }}						\\8
		*/
		/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("procedure p; variable v; Select p such that Uses(2, v)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { };
			
		
			Assert::IsTrue(expectedResults == results);
		}

		TEST_METHOD(QE_PatternSubExpression) {
			/*********************** Test Code ************************/
			/*
				procedure dream {
					beads = command + 10;							\\1
					beads = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						stamps = beads + command + coffee;			\\5
						while command {								\\6
							x = x + x * 9;}							\\7
						x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(_, \"_x * 9_\")");
			QueryTree queryTree = parserPQL.getQueryTree();
			Assert::IsTrue(queryTree.getPatternTree().at(0).getLeftCType() == Enum::TYPE::UNDERSCORE);
			Assert::IsTrue(queryTree.getPatternTree().at(0).getRightCIsExpression() == true);
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string( "7");
			string outputString;

			Assert::AreEqual(results.size(), (size_t) 1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}

			Assert::AreEqual(outputString, actualResults);
		}


		TEST_METHOD(QE_PatternRightDefined) {
			/*********************** Test Code ************************/
			/*
				procedure dream {
					beads = command + 10;							\\1
					beads = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						stamps = beads + command + coffee;			\\5
						while command {								\\6
							x = x + x * 9;}							\\7
						x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(_, inspiration + 1)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("3");
			string outputString;

			Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}

			Assert::AreEqual(outputString, actualResults);
		}

		TEST_METHOD(QE_PatternBothDefined) {
			/*********************** Test Code ************************/
			/*
			Procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			stamps = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }}					\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(7, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(x, \"_x * 9_\")");
			QueryTree queryTree = parserPQL.getQueryTree();
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIntValue(), 5 );
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("7");
			string outputString;

			Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}
			

			Assert::AreEqual(actualResults, outputString);
		}

		TEST_METHOD(QE_PatternNonSubExpression) {
			/*********************** Test Code ************************/
			/*
				procedure dream {
					beads = command + 10;							\\1
					beads = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						stamps = beads + command + coffee;			\\5
						while command {								\\6
							x = x + x * 9;}							\\7
						x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(beads, command + 10)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("1");
			string outputString;

			Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}

			Assert::AreEqual(outputString, actualResults);
		}

//		TEST_METHOD(QE_SuchThatPlusPattern) {
			/*********************** Test Code ************************/
			/*
				procedure dream {
					beads = command + 10;							\\1
					beads = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						stamps = beads + command + coffee;			\\5
						while command {								\\6
							x = x + x * 9;}							\\7
						x = beads + command; }}						\\8
			*/
			/**********************************************************/
/*			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; variable x; Select a such that Modifies(a, x) pattern a(_, \"_x * 9_\")");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResult = { "7" };

			Assert::IsTrue(expectedResult == results);
		}
	*/	
		TEST_METHOD(QE_shuntingyard) {
			QueryEvaluator qe;
			string equation = "x + 9";
			string equation2 = "x + x * 9";
			string ast = "x 9 + ";
			string ast2 = "xx9*+";
			string ast3 = "342*15-/+";
			
			Assert::AreEqual(ast, qe.convertToShuntingYard(equation));
			Assert::AreEqual(ast2, qe.convertToShuntingYard(equation2));
		}

		TEST_METHOD(QE_EvaluateSuchThatParentTTuple) {
			/*********************** Test Code ************************/
			/*	procedure dream {
					beads = command + 10;							\\1
					beads = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						stamps = beads + command + coffee;			\\5
						while command {								\\6
							x = x * 9;}								\\7
						x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("procedure p; stmt s1; Select <p, s1> such that Parent*(4, s1)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "dream, 5", "dream, 6", "dream, 7", "dream, 8" };

			Assert::IsTrue(expectedResults == results);
		}

		TEST_METHOD(QE_EvaluateSuchThatFollowsT) {
			/*********************** Test Code ************************/
			/*	procedure dream {
					beads = command + 10;							\\1
					beads = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						stamps = beads + command + coffee;			\\5
						while command {								\\6
							x = x * 9;}								\\7
						x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("stmt s, s1; Select s1 such that Follows*(s, s1)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "2", "3", "4", "3", "4", "4", "6", "8", "8" };

			Assert::IsTrue(expectedResults == results);

		}

		TEST_METHOD(QE_PatternWhile) {
			/*********************** Test Code ************************/
			/*
			Procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			stamps = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }}					\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(7, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setControlVar(4, pkb->getVarIndex("coffee"));

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

			// statement 6
			pkb->setControlVar(6, pkb->getVarIndex("command"));
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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("while w; Select w pattern w(\"coffee\", _)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("4");
			string outputString;

			//Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}


			Assert::AreEqual(actualResults, outputString);
		}
		
		TEST_METHOD(QE_PatternIf) {
			/*********************** Test Code ************************/
			/*
			Procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			stamps = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }}					\\8
			if command{								\\9
			}
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8
			pkb->setType(Enum::TYPE::IF);

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(7, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setControlVar(4, pkb->getVarIndex("coffee"));

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

			// statement 6
			pkb->setControlVar(6, pkb->getVarIndex("command"));
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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");

			// statement 9
			pkb->setControlVar(9, pkb->getVarIndex("command"));
			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("if ifstat; Select ifstat pattern ifstat(command, _, _)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("9");
			string outputString;

			Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}


			Assert::AreEqual(actualResults, outputString);
		}

		TEST_METHOD(QE_BooleanTrue) {
			/*********************** Test Code ************************/
			/*
			Procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			stamps = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }}					\\8
			if command{								\\9
			}
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8
			pkb->setType(Enum::TYPE::IF);

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(7, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
			parent.push_back(make_pair(6, 7));
			parent.push_back(make_pair(4, 8));
			pkb->setChildren(parent);

			// Statement 1 - set constant as variables?
			pkb->setVarName("beads");
			pkb->setVarName("command");
			pkb->setRightExpr(1, "command10+");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setControlVar(4, pkb->getVarIndex("coffee"));

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

			// statement 6
			pkb->setControlVar(6, pkb->getVarIndex("command"));
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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");

			// statement 9
			pkb->setControlVar(9, pkb->getVarIndex("command"));
			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a;variable x; Select BOOLEAN such that Modifies(a,x)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("TRUE");
			string outputString;

			Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}


			Assert::AreEqual(actualResults, outputString);
		}

		TEST_METHOD(QE_BooleanFalse) {
			/*********************** Test Code ************************/
			/*
			Procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			stamps = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }}					\\8
			if command{								\\9
			}
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8
			pkb->setType(Enum::TYPE::IF);

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(7, 8));
			pkb->setFollows(follows);

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
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

			// Statement 2
			pkb->setModifies(2, "beads");
			pkb->setModifiedBy("beads", 2);

			// Statement 3 - set constant as variable?
			pkb->setVarName("inspiration");
			pkb->setRightExpr(3, "inspiration1+");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedBy("inspiration", 3);

			//Statement 4
			pkb->setVarName("coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setControlVar(4, pkb->getVarIndex("coffee"));

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

			// statement 6
			pkb->setControlVar(6, pkb->getVarIndex("command"));
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

			// statement 8
			pkb->setUsedVar(8, "beads");
			pkb->setUsedBy("beads", 8);
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");

			// statement 9
			pkb->setControlVar(9, pkb->getVarIndex("command"));
			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select BOOLEAN pattern a(x, x + 9 + 9)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("FALSE");
			string outputString;

			Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}


			Assert::AreEqual(actualResults, outputString);
		}

		TEST_METHOD(QE_EvaluateWithClauseGiven) {
		/*********************** Test Code ************************/
		/*	procedure dream {
				beads = command + 10;							\\1
				beads = 34;										\\2
				command = inspiration + 1;						\\3
				while coffee {									\\4
					stamps = beads + command + coffee;			\\5
					while command {								\\6
						x = x * 9;}								\\7
					x = beads + command; }}						\\8
		*/
		/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select BOOLEAN such that Modifies(a, \"x\") with a.stmt# = 10");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string expectedResults = "FALSE" ;
			string outputString;
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}


			Assert::AreEqual(expectedResults, outputString);
		};

		TEST_METHOD(QE_EvaluateWithClauseNotGivenString) {
			/*********************** Test Code ************************/
			/*	procedure dream {
					beads = command + 10;							\\1
					dream = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						stamps = beads + command + coffee;			\\5
						while command {								\\6
						x = x * 9;}								\\7
						x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);

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
		};

		TEST_METHOD(QE_EvaluateWithClauseNotGivenInt) {
			/*********************** Test Code ************************/
			/*	procedure dream {
			beads = command + 10;							\\1
			dream = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			stamps = beads + command + coffee;			\\5
			while command {								\\6
			x = x * 9;}								\\7
			x = beads + command; }}						\\8
			*/
			/**********************************************************/
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);	//1
			pkb->setType(Enum::TYPE::ASSIGN);	//2
			pkb->setType(Enum::TYPE::ASSIGN);	//3
			pkb->setType(Enum::TYPE::WHILE);	//4
			pkb->setType(Enum::TYPE::ASSIGN);	//5
			pkb->setType(Enum::TYPE::WHILE);	//6
			pkb->setType(Enum::TYPE::ASSIGN);	//7
			pkb->setType(Enum::TYPE::ASSIGN);	//8

			vector<string> varUsed;
			varUsed.push_back("command");
			varUsed.push_back("inspiration");
			varUsed.push_back("coffee");
			varUsed.push_back("beads");
			varUsed.push_back("x");

			pkb->setProcNameInProcTable("dream");	//0
			pkb->setStartNum(pkb->getProcIndex("dream"), 1);
			pkb->setEndNum(pkb->getProcIndex("dream"), 8);

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(5, 8));
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

			//Statement 5
			pkb->setVarName("stamps");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedBy("beads", 5);
			pkb->setUsedVar(5, "command");
			pkb->setUsedBy("command", 5);
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedBy("coffee", 5);
			pkb->setModifies(5, "stamps");
			pkb->setModifiedBy("stamps", 5);
			// set statement 4
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedBy("coffee", 4);
			pkb->setModifies(4, "stamps");
			pkb->setModifiedBy("stamps", 4);

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
			pkb->setUsedVar(6, "beads");
			pkb->setUsedBy("beads", 6);
			pkb->setUsedVar(4, "beads");
			pkb->setUsedBy("beads", 4);
			pkb->setUsedVar(8, "command");
			pkb->setUsedBy("command", 8);
			pkb->setUsedVar(6, "command");
			pkb->setUsedBy("command", 6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");


			pkb->setProcUses(0, varUsed);
			ParserForPQL parserPQL = ParserForPQL("assign a; stmt s; Select s with a.stmt# = s.stmt#");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "1", "2", "3", "5", "7", "8" };

			Assert::IsTrue(expectedResults == results);
		};
	};
}
