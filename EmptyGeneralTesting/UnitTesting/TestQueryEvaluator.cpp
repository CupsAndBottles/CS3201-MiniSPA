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
			//assign a; Select a;
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

			// Sets stmts
			//pkb->setType(Enum::TYPE::ASSIGN); // stmt 1: assignment stmt
			//pkb->setType(Enum::TYPE::WHILE); // stmt 2: while stmt
			//pkb->setType(Enum::TYPE::ASSIGN); // stmt 3: assignment stmt

			//ParserForPQL parserPQL = ParserForPQL("assign a; Select a");
			//QueryTree queryTree = parserPQL.getQueryTree();
			QueryTree queryTree = QueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);
			
			Clauses clause;
			clause.setParentStringVal("a");
			clause.setParentType("assign");
			queryTree.setResultTree(clause);
			
			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "1", "2", "3", "5", "7", "8", "10"};

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
			
			QueryTree queryTree;
			Clauses clause;

			clause.setParentStringVal("Follows");

			clause.setLeftCType("stmt");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(1);

			clause.setRightCType("stmt");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(2);
			queryTree.setSuchThatTree(clause);

			clause.setParentStringVal("s");
			clause.setParentType("stmt");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("stmt s; Select s such that Follows(1, 2)");
			//QueryTree queryTree = parserPQL.getQueryTree();
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

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("Parent");

			clause.setLeftCType("while");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("w");

			clause.setRightCType("_");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("_");
			queryTree.setSuchThatTree(clause);

			clause = Clauses();
			clause.setParentStringVal("w");
			clause.setParentType("while");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("while w; Select w such that Parent(w, _)");
			//QueryTree queryTree = parserPQL.getQueryTree();

			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);

			list<string> expectedResults = { "4", "4", "4", "6" };

			Assert::IsTrue(expectedResults == results);
		}

		TEST_METHOD(QE_EvaluateSuchThatUsesNoCommonSyn) {
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

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("Uses");

			clause.setLeftCType("stmt");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(2);
			clause.setLeftCStringValue("");

			clause.setRightCType("variable");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("v");
			queryTree.setSuchThatTree(clause);

			clause = Clauses();
			clause.setParentStringVal("p");
			clause.setParentType("procedure");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("procedure p; variable v; Select p such that Uses(2, v)");
			//QueryTree queryTree = parserPQL.getQueryTree();
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
						inspiration = beads + command + coffee;		\\5
						while command {								\\6
							x = x + x * 9;}							\\7
						x = beads + command; }}						\\8
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("a");
			clause.setParentType("assign");

			clause.setLeftCType("_");
			clause.setLeftCIsExpression(true);
			clause.setLeftCStringValue("_");

			clause.setRightCType("");
			clause.setRightCIsExpression(true);
	
			clause.setRightCStringValue("x * 9");
			queryTree.setPatternTree(clause);

			clause = Clauses();
			clause.setParentStringVal("a");
			clause.setParentType("assign");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(_, \"_x * 9_\")");
			//QueryTree queryTree = parserPQL.getQueryTree();
			Assert::IsTrue(queryTree.getPatternTree().at(0).getLeftCType() == Enum::TYPE::UNDERSCORE);
			Assert::IsTrue(queryTree.getPatternTree().at(0).getRightCIsExpression() == true);
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string( "7");
			string outputString = string("");

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
						inspiration = beads + command + coffee;		\\5
						while command {								\\6
							x = x + x * 9;}							\\7
						x = beads + command; }}						\\8
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("a");
			clause.setParentType("assign");

			clause.setLeftCType("_");
			clause.setLeftCIsExpression(true);
			clause.setLeftCStringValue("_");

			clause.setRightCType("");
			clause.setRightCIsExpression(false);
			clause.setRightCStringValue("inspiration + 1");
			queryTree.setPatternTree(clause);

			clause = Clauses();
			clause.setParentStringVal("a");
			clause.setParentType("assign");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(_, inspiration + 1)");
			//QueryTree queryTree = parserPQL.getQueryTree();
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
			inspiration = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }}					\\8
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("a");
			clause.setParentType("assign");

			clause.setLeftCType("");
			clause.setLeftCIsExpression(false);
			clause.setLeftCStringValue("x");

			clause.setRightCType("");
			clause.setRightCIsExpression(true);
			clause.setRightCStringValue("x * 9");
			queryTree.setPatternTree(clause);

			clause = Clauses();
			clause.setParentStringVal("a");
			clause.setParentType("assign");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(x, \"_x * 9_\")");
			//QueryTree queryTree = parserPQL.getQueryTree();
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
						inspiration = beads + command + coffee;		\\5
						while command {								\\6
							x = x + x * 9;}							\\7
						x = beads + command; }}						\\8
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("a");
			clause.setParentType("assign");

			clause.setLeftCType("");
			clause.setLeftCIsExpression(false);
			clause.setLeftCStringValue("beads");

			clause.setRightCType("");
			clause.setRightCIsExpression(false);
			clause.setRightCStringValue("command + 10");
			queryTree.setPatternTree(clause);

			clause = Clauses();
			clause.setParentStringVal("a");
			clause.setParentType("assign");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(beads, command + 10)");
			//QueryTree queryTree = parserPQL.getQueryTree();
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

		TEST_METHOD(QE_SuchThatPlusPattern) {
			/*********************** Test Code ************************/
			/*
				procedure dream {
					beads = command + 10;							\\1
					beads = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						inspiration = beads + command + coffee;		\\5
						while command {								\\6
							x = x + x * 9;}							\\7
						x = beads + command; }}						\\8
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });


			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("a");
			clause.setParentType("assign");

			clause.setLeftCType("_");
			clause.setLeftCIsExpression(true);
			clause.setLeftCStringValue("_");

			clause.setRightCType("");
			clause.setRightCIsExpression(true);
			clause.setRightCStringValue("x * 9");
			queryTree.setPatternTree(clause);

			clause = Clauses();
			clause.setParentStringVal("Modifies");

			clause.setLeftCType("assign");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("a");

			clause.setRightCType("variable");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("v");
			queryTree.setSuchThatTree(clause);

			clause = Clauses();
			clause.setParentStringVal("a");
			clause.setParentType("assign");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("assign a; variable v; Select a such that Modifies(a, v) pattern a(_, \"_x * 9_\")");
			//QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResult = { "7"};

			Assert::IsTrue(expectedResult == results);

			
		}

		TEST_METHOD(QE_shuntingyard) {
			QueryEvaluator qe;
			string equation = "x + 9";
			string equation2 = "x + x * 9";
			string ast = "x 9 +";
			string ast2 = "x x 9 * +";
			
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
			pkb->setByDesignExtractor();
			pkb->setParentTChildrenT();

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("Parent*");

			clause.setLeftCIntValue(4);
			clause.setLeftCIsExpression(false);
			clause.setLeftCStringValue("");
			clause.setLeftCType("stmt");

			clause.setRightCIntValue(-1);
			clause.setRightCIsExpression(false);
			clause.setRightCType("stmt");
			clause.setRightCStringValue("s1");
			queryTree.setSuchThatTree(clause);

			clause = Clauses();
			clause.setParentStringVal("p");
			clause.setParentType("procedure");
			queryTree.setResultTree(clause);

			clause = Clauses();
			clause.setParentStringVal("s1");
			clause.setParentType("stmt");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("procedure p; stmt s1; Select <p, s1> such that Parent*(4, s1)");
			//QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "dream 5", "dream 6", "dream 7", "dream 8" };
			
			Assert::IsTrue(expectedResults == results);
		}

		TEST_METHOD(QE_EvaluateSuchThatFollowsT) {
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
			pkb->setByDesignExtractor();

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("Follows*");

			clause.setLeftCIntValue(-1);
			clause.setLeftCIsExpression(false);
			clause.setLeftCStringValue("s");
			clause.setLeftCType("stmt");

			clause.setRightCIntValue(-1);
			clause.setRightCIsExpression(false);
			clause.setRightCType("stmt");
			clause.setRightCStringValue("s1");
			queryTree.setSuchThatTree(clause);

			clause = Clauses();
			clause.setParentStringVal("s1");
			clause.setParentType("stmt");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("stmt s, s1; Select s1 such that Follows*(s, s1)");
			//QueryTree queryTree = parserPQL.getQueryTree();
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
			inspiration = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }}					\\8
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("w");
			clause.setParentType("while");

			clause.setLeftCType("variable");
			clause.setLeftCIsExpression(false);
			clause.setLeftCStringValue("coffee");

			clause.setRightCType("_");
			clause.setRightCIsExpression(true);
			clause.setRightCStringValue("_");
			queryTree.setPatternTree(clause);

			clause = Clauses();
			clause.setParentStringVal("w");
			clause.setParentType("while");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("while w; Select w pattern w(\"coffee\", _)");
			//QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("4");
			string outputString;

			//Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}


			Assert::AreEqual(actualResults, outputString);
			delete pkb;
		}
		
		TEST_METHOD(QE_PatternIf) {
			/*********************** Test Code ************************/
			/*
			procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			inspiration = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }					\\8
			if command{								\\9
			}}
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });

			// statement 11
			pkb->setType(Enum::TYPE::IF);
			pkb->setControlVar(11, pkb->getVarIndex("command"));
			pkb->setProcUses(varUsed);

			Assert::AreEqual(pkb->getControlVar(11), 1);
			Assert::AreEqual(pkb->getVarIndex("command"), 1);
			
			
			QueryTree queryTree = QueryTree();
			Clauses clause;
			clause.setParentStringVal("ifstat");
			clause.setParentType("if");

			clause.setLeftCType("variable");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(0);
			string command = "command";
			clause.setLeftCStringValue(command);
			string fake = "0";
			clause.setLeftCIsStmt(fake);

			clause.setRightCType("_");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("_");
			
			clause.setRightCIsStmt(fake);
			queryTree.setPatternTree(clause);
//			string command = "command";
			//ParserForPQL parserPQL = ParserForPQL("if ifstat; Select ifstat pattern ifstat(\"command\", _, _)");
			//QueryTree queryTree = parserPQL.getQueryTree();
			clause = queryTree.getPatternTree().at(0);
			Assert::AreEqual(0, clause.getLeftCIntValue());
			Assert::AreEqual(false, clause.getLeftCIsExpression());
			Assert::AreEqual(false, clause.getLeftCIsStmt());
			Assert::AreEqual(command, clause.getLeftCStringValue());
			Assert::AreEqual(6, (int)clause.getLeftCType());
			string ifstat = "ifstat";
			Assert::AreEqual(ifstat, clause.getParentStringVal());
			Assert::AreEqual(5, (int)clause.getParentType());
			Assert::AreEqual(-1, clause.getRightCIntValue());
			Assert::AreEqual(false, clause.getRightCIsExpression());
			Assert::AreEqual(false, clause.getRightCIsStmt());
			string underscore = "_";
			Assert::AreEqual(underscore, clause.getRightCStringValue());
			Assert::AreEqual(3, (int)clause.getRightCType());
			
			
			clause = Clauses();
			clause.setParentStringVal("ifstat");
			clause.setParentType("if");
			queryTree.setResultTree(clause);

			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("11");
			string outputString;

//			Assert::AreEqual(results.size(), (size_t)1);
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				outputString = outputString + *it;
			}


			Assert::AreEqual(actualResults, outputString);
			delete pkb;
		}

		TEST_METHOD(QE_BooleanTrue) {
			/*********************** Test Code ************************/
			/*
			Procedure dream {
			beads = command + 10;							\\1
			beads = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			inspiration = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }					\\8
			if command{								\\9
			}}
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
			pkb->setType(Enum::TYPE::ASSIGN);	//8
			pkb->setType(Enum::TYPE::IF);

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

			// statement 9
			pkb->setControlVar(9, pkb->getVarIndex("command"));
			pkb->setProcUses(varUsed);

			QueryTree queryTree = QueryTree();
			Clauses clause;

			clause.setParentStringVal("Modifies");

			clause.setLeftCType("assign");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("a");
			clause.setLeftCIsStmt("0");

			clause.setRightCType("variable");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("x");
			clause.setRightCIsStmt("0");

			queryTree.setSuchThatTree(clause);

			clause.setParentStringVal("BOOLEAN");
			clause.setParentType("BOOLEAN");
			queryTree.setResultTree(clause);

	//		ParserForPQL parserPQL = ParserForPQL("assign a;variable x; Select BOOLEAN such that Modifies(a,x)");
	//		QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("true");
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
			inspiration = beads + command + coffee;			\\5
			while command {								\\6
			x = x + x * 9;}							\\7
			x = beads + command; }}					\\8
			if command{								\\9
			}
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
			pkb->setType(Enum::TYPE::IF);

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
			pkb->setRightExpr(8, "beads command +");
			pkb->setModifies(8, "x");
			pkb->setModifiedBy("x", 8);
			pkb->setModifies(4, "x");
			pkb->setModifiedBy("x", 4);

			// statement 9
			pkb->setControlVar(9, pkb->getVarIndex("command"));
			pkb->setProcUses(varUsed);

			QueryTree queryTree;
			Clauses clause;
			clause.setParentStringVal("a");
			clause.setParentType("assign");

			clause.setLeftCType("");
			clause.setLeftCIsExpression(false);
			clause.setLeftCStringValue("x");

			clause.setRightCType("");
			clause.setRightCIsExpression(false);
			clause.setRightCStringValue("x + 9 + 9");
			queryTree.setPatternTree(clause);

			clause = Clauses();
			clause.setParentStringVal("BOOLEAN");
			clause.setParentType("BOOLEAN");
			queryTree.setResultTree(clause);

//			ParserForPQL parserPQL = ParserForPQL("assign a; Select BOOLEAN pattern a(x, x + 9 + 9)");
//			queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("false");
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
					inspiration = beads + command + coffee;		\\5
					while command {								\\6
						x = x * 9;}								\\7
					x = beads + command; }						\\8
				call hope;}

			procedure hope {
				bye = moonlight * 7; }							\\10

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
			pkb->setType(Enum::TYPE::CALLS);

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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });

			QueryTree queryTree = QueryTree();
			Clauses clause;

			clause.setParentStringVal("Modifies");

			clause.setLeftCType("assign");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("a");
			clause.setLeftCIsStmt("0");

			clause.setRightCType("variable");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(0);
			clause.setRightCStringValue("x");
			clause.setRightCIsStmt("0");
	
			queryTree.setSuchThatTree(clause);

			clause.setLeftCType("assign");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("a");
			clause.setLeftCIsStmt("0");

			clause.setRightCType("stmt");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(10);
			clause.setRightCIsStmt("0");
			queryTree.setWithTree(clause);
			

			clause.setParentStringVal("BOOLEAN");
			clause.setParentType("BOOLEAN");
			queryTree.setResultTree(clause);

	//		ParserForPQL parserPQL = ParserForPQL("assign a; Select BOOLEAN such that Modifies(a, \"x\") with a.stmt# = 10");
	//		queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string expectedResults = "false" ;
			string outputString = string("");
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
						inspiration = beads + command + coffee;		\\5
						while command {								\\6
						x = x * 9;}								\\7
						x = beads + command; }}						\\8
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });


			pkb->setProcUses(varUsed);

			QueryTree queryTree = QueryTree();
			Clauses clause;
			clause.setLeftCType("variable");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("v");
			clause.setLeftCIsStmt("0");

			clause.setRightCType("procedure");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("p");
			clause.setRightCIsStmt("0");
			queryTree.setWithTree(clause);

			clause.setParentStringVal("v");
			clause.setParentType("variable");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("procedure p; variable v; Select v with v.varName = p.procName");
			//QueryTree queryTree = parserPQL.getQueryTree();
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
			inspiration = beads + command + coffee;			\\5
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

			QueryTree queryTree;
			Clauses clause;
			clause.setLeftCType("assign");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("a");
			clause.setLeftCIsStmt("0");

			clause.setRightCType("stmt");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("s");
			clause.setRightCIsStmt("0");
			queryTree.setWithTree(clause);

			clause = Clauses();
			clause.setParentStringVal("s");
			clause.setParentType("stmt");
			queryTree.setResultTree(clause);

			pkb->setProcUses(varUsed);
			//	ParserForPQL parserPQL = ParserForPQL("assign a; stmt s; Select s with a.stmt# = s.stmt#");
			//QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "1", "2", "3", "5", "7", "8" };

			Assert::IsTrue(expectedResults == results);
		};

		TEST_METHOD(QE_EvaluateSuchThatCallsWithCalls) {
			/*********************** Test Code ************************/
			/*	procedure dream {
					beads = command + 10;							\\1
					dream = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						inspiration = beads + command + coffee;		\\5
						while command {								\\6
							x = x * 9;}								\\7
						x = beads + command;}						\\8
					call hope; }									\\9
				
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

			QueryTree queryTree = QueryTree();

			Clauses clause;
			clause.setLeftCType("call");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("c");
			clause.setLeftCIsStmt("0");

			clause.setRightCType("procedure");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(0);
			clause.setRightCStringValue("hope");
			clause.setRightCIsStmt("0");
			queryTree.setWithTree(clause);

			clause.setParentStringVal("Calls");

			clause.setLeftCType("procedure");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("p");

			clause.setRightCType("procedure");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("q");
			queryTree.setSuchThatTree(clause);

			clause.setParentStringVal("p");
			clause.setParentType("procedure");
			queryTree.setResultTree(clause);

			clause.setParentStringVal("q");
			clause.setParentType("procedure");
			queryTree.setResultTree(clause);

			//ParserForPQL parserPQL = ParserForPQL("procedure p, q; call c; Select <p, q> such that Calls(p, q) with c.procName = \"hope\"");
			//QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "dream hope" };

			Assert::IsTrue(expectedResults == results);
		};

		TEST_METHOD(QE_EvaluateSuchThatPatternWith) {
			/*********************** Test Code ************************/
			/*	procedure dream {
			beads = command + 10;							\\1
			dream = 34;										\\2
			command = inspiration + 1;						\\3
			while coffee {									\\4
			inspiration = beads + command + coffee;			\\5
			while command {								\\6
			x = x * 9;}								\\7
			x = beads + command;}						\\8
			call hope; }								\\9

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
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight")});
			pkb->setProcModifies(0, { pkb->getVarIndex("bye")});

			QueryTree queryTree = QueryTree();

			Clauses clause;
			clause.setLeftCType("call");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("c");
			clause.setLeftCIsStmt("0");

			clause.setRightCType("procedure");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(0);
			clause.setRightCStringValue("hope");
			clause.setRightCIsStmt("0");
			queryTree.setWithTree(clause);

			clause.setParentStringVal("Calls");

			clause.setLeftCType("procedure");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("p");

			clause.setRightCType("procedure");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("q");
			queryTree.setSuchThatTree(clause);
		
			clause.setParentStringVal("a");
			clause.setParentType("assign");

			clause.setLeftCType("variable");
			clause.setLeftCIsExpression("0");
			clause.setLeftCIntValue(0);
			clause.setLeftCStringValue("command");

			clause.setRightCType("_");
			clause.setRightCIsExpression("0");
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("_");
			queryTree.setPatternTree(clause);

			
			clause.setParentStringVal("p");
			clause.setParentType("procedure");
			queryTree.setResultTree(clause);

		//	ParserForPQL parserPQL = ParserForPQL("assign a; procedure p, q; call c; Select p pattern a(\"command\", _) such that Calls(p, q) with c.procName = \"hope\"");
		
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "dream" };

			Assert::IsTrue(expectedResults == results);
		};

		TEST_METHOD(QE_EvaluateTuplesSuchThatPatternWith) {
			/*********************** Test Code ************************/
			/*	procedure dream {
					beads = command + 10;							\\1
					dream = 34;										\\2
					command = inspiration + 1;						\\3
					while coffee {									\\4
						stamps = beads + command + coffee;			\\5
						while command {								\\6
							x = x * 9;}								\\7
						x = beads + command; }						\\8
					call hope; }									\\9

			    procedure hope {
					bye = moonlight * 7; }							\\10
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
			pkb->setRightExpr(1, "command 10 +");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setUsedVar(1, "command");
			pkb->setUsedBy("command", 1);
			pkb->setConstant(10);
			pkb->setStmtUsed(pkb->getConstantIndex(10), 1);

			// Statement 2
			pkb->setVarName("dream");
			pkb->setModifies(2, "dream");
			pkb->setModifiedBy("dream", 2);
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
			pkb->setRightExpr(7, "x x 9 * +");
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
			pkb->setRightExpr(10, "moonlight 7 *");
			pkb->setModifies(10, "bye");
			pkb->setModifiedBy("bye", 10);
			pkb->setUsedVar(10, "moonlight");
			pkb->setUsedBy("moonlight", 10);
			pkb->setConstant(7);
			pkb->setStmtUsed(pkb->getConstantIndex(7), 10);
			pkb->setProcUses(0, { pkb->getVarIndex("moonlight") });
			pkb->setProcModifies(0, { pkb->getVarIndex("bye") });
			
			QueryTree queryTree = QueryTree();

			Clauses clause;
			clause.setLeftCType("assign");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("a");
			clause.setLeftCIsStmt("0");

			clause.setRightCType("stmt");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("s");
			clause.setRightCIsStmt("0");
			queryTree.setWithTree(clause);

			clause.setParentStringVal("Modifies");

			clause.setLeftCType("stmt");
			clause.setLeftCIsExpression(false);
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("s");

			clause.setRightCType("variable");
			clause.setRightCIsExpression(false);
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("v");
			queryTree.setSuchThatTree(clause);

			clause.setParentStringVal("w");
			clause.setParentType("while");

			clause.setLeftCType("_");
			clause.setLeftCIsExpression("0");
			clause.setLeftCIntValue(-1);
			clause.setLeftCStringValue("_");

			clause.setRightCType("_");
			clause.setRightCIsExpression("0");
			clause.setRightCIntValue(-1);
			clause.setRightCStringValue("_");
			queryTree.setPatternTree(clause);


			clause.setParentStringVal("w");
			clause.setParentType("while");
			queryTree.setResultTree(clause);

			clause.setParentStringVal("s");
			clause.setParentType("stmt");
			queryTree.setResultTree(clause);

			clause.setParentStringVal("v");
			clause.setParentType("variable");
			queryTree.setResultTree(clause);
			
	//		ParserForPQL parserPQL = ParserForPQL("while w; assign a; variable v; stmt s; Select <w, s, v> such that Modifies(s, v) with a.stmt# = s.stmt# pattern w(_, _)");
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			list<string> expectedResults = { "4 1 beads", "4 2 dream", "4 3 command", "4 5 stamps", "4 7 x", "4 8 x", "4 10 bye", "6 1 beads", "6 2 dream", "6 3 command", "6 5 stamps", "6 7 x", "6 8 x", "6 10 bye" };

			Assert::IsTrue(expectedResults == results);
		};
	};
}
