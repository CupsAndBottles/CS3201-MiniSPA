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
			string actualResults = string("1, 3");

			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				Assert::AreEqual(actualResults, *it);
			}

			delete pkb;
		} 

		TEST_METHOD(QE_EvaluateSuchThatModifiesPatternWhile) {
		/*********************** Test Code ************************/
			/*	beads = command + 10;							\\1
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

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			pkb->setVarName("beads");
			pkb->setModifies(1, "beads");
			pkb->setVarName("command");
			pkb->setModifies(2, "beads");
			pkb->setModifies(3, "command");
			pkb->setVarName("inspiration");
			pkb->setModifies(5, "inspiration");
			pkb->setModifies(4, "inspiration");
			pkb->setVarName("x");
			pkb->setModifies(7, "x");
			pkb->setModifies(6, "x");
			pkb->setModifies(4, "x");
			pkb->setModifies(8, "x");
			pkb->setModifies(4, "x");

			ParserForPQL parserPQL = ParserForPQL("assign a; while w; variable v; Select a such that Modifies(a, _) pattern w(v, _)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("1, 2, 3, 5, 7, 8");

			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				Assert::AreEqual(actualResults, *it);
			}

			delete pkb;
		}

		TEST_METHOD(QE_EvaluateSuchThatUsesPatternAssign) {
		/*********************** Test Code ************************/
		/*	procedure dream {
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

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			vector<string> varUsed;
			pkb->setProcNameInProcTable("dream");
			pkb->setVarName("beads");
			pkb->setVarName("command");
			
			varUsed.push_back("command");
			pkb->setUsedVar(1, "command");
			pkb->setVarName("inspiration");
			
			varUsed.push_back("inspiration");
			pkb->setUsedVar(3, "inspiration");
			pkb->setVarName("coffee");
			
			varUsed.push_back("coffee");
			pkb->setUsedVar(4, "coffee");
			
			varUsed.push_back("beads");
			pkb->setUsedVar(5, "beads");
			pkb->setUsedVar(4, "beads");
			pkb->setUsedVar(5, "command");
			pkb->setUsedVar(4, "command");
			pkb->setUsedVar(5, "coffee");
			pkb->setUsedVar(4, "coffee");
			pkb->setUsedVar(6, "command");
			pkb->setUsedVar(4, "command");

			pkb->setVarName("x");
			varUsed.push_back("x");
			pkb->setUsedVar(7, "x");
			pkb->setUsedVar(6, "x");
			pkb->setUsedVar(4, "x");
			pkb->setRightExpr(7, "xx9*+");
			pkb->setUsedVar(8, "beads");
			pkb->setUsedVar(4, "beads");
			pkb->setUsedVar(8, "command");
			pkb->setUsedVar(4, "command");
			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(_, \"_x * 9_\")");
			QueryTree queryTree = parserPQL.getQueryTree();
			Assert::IsTrue(queryTree.getPatternTree().at(0).getRightCIsExpression());
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
		//	string actualResults = string("command, inspiration, coffee, beads, x");
			string actualResults = string("beads, command, inspiration, coffee, x");

			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				Assert::AreEqual(actualResults, *it);
			}
		}

		TEST_METHOD(QE_allSetMethods) {
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

			// Set ALL follows
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(5, 6));
			follows.push_back(make_pair(7, 8));

			// Set ALL Parent
			vector<pair<int, int>> parent;
			parent.push_back(make_pair(4, 5));
			parent.push_back(make_pair(4, 6));
			parent.push_back(make_pair(6, 7));
			parent.push_back(make_pair(7, 8));
			pkb->setChildren(parent);

			// Statement 1 - set constant as variables?
			pkb->setVarName("beads");		
			pkb->setVarName("command");
			pkb->setRightExpr(1 ,"command10+");
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
			pkb->setUsedBy("command",6);
			pkb->setUsedVar(4, "command");
			pkb->setUsedBy("command", 4);
			pkb->setRightExpr(8, "beadscommand+");
			

			pkb->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(_, \"_x * 9_\")");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			//	string actualResults = string("command, inspiration, coffee, beads, x");
			string actualResults = string("beads, command, inspiration, coffee, x");

			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				Assert::AreEqual(actualResults, *it);
			}
		}

		TEST_METHOD(QE_shuntingyard) {
			QueryEvaluator qe;
			string equation = "x + 9";
			string equation2 = "x + x * 9";
			string equation3 = "3 + 4 * 2 / (1 - 5)";
			string ast = "x9+";
			string ast2 = "xx9*+";
			string ast3 = "342*15-/+";
			
			Assert::AreEqual(ast, qe.convertToShuntingYard(equation));
			Assert::AreEqual(ast2, qe.convertToShuntingYard(equation2));
			Assert::AreEqual(ast3, qe.convertToShuntingYard(equation3));
		}
	};
}