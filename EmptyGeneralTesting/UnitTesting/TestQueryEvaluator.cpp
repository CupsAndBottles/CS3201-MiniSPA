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
		TEST_METHOD(TestEvaluateSelectClause) {
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
		} 

		TEST_METHOD(TestEvaluateSuchThatModifiesPatternWhile) {
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
		}

		TEST_METHOD(TestEvaluateSuchThatUsesPatternAssign) {
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

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a pattern a(_, \"x + x * 9\")");
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
			string ast = "x9+";
			string ast2 = "xx9*+";
			Assert::AreEqual(ast, qe.convertToShuntingYard(equation));
			Assert::AreEqual(ast2, qe.convertToShuntingYard(equation2));
		}
	};
}