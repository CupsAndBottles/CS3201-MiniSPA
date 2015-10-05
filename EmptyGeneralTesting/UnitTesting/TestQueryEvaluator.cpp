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

			pkb->setVarName("beads");
			pkb->setModifies(1, "beads");
			pkb->setModifiedBy("beads", 1);
			pkb->setVarName("command");
			pkb->setModifies(3, "command");
			pkb->setModifiedBy("command", 3);

			ParserForPQL parserPQL = ParserForPQL("assign a; variable v; Select a such that Modifies(a, v)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);
			
			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("1, 2");

			Assert::AreEqual(actualResults, results.front());
			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				Assert::AreEqual(actualResults, *it);
			}
		} 

//		TEST_METHOD(TestEvaluateSuchThatModifiesPatternWhile) {
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
/*			PKB *pkb = new PKB();

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

			ParserForPQL parserPQL = ParserForPQL("assign a; variable v; Select a such that Modifies(a, _)");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("1, 2, 3, 5, 7");

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
						x = x * 9;}								\\7
					x = beads + command; }}						\\8
		*/
		/**********************************************************/
/*			PKB *pkb1 = new PKB();

			pkb1->setType(Enum::TYPE::ASSIGN);
			pkb1->setType(Enum::TYPE::ASSIGN);
			pkb1->setType(Enum::TYPE::ASSIGN);
			pkb1->setType(Enum::TYPE::WHILE);
			pkb1->setType(Enum::TYPE::ASSIGN);
			pkb1->setType(Enum::TYPE::WHILE);
			pkb1->setType(Enum::TYPE::ASSIGN);
			pkb1->setType(Enum::TYPE::ASSIGN);

			vector<string> varUsed;
			pkb1->setProcNameInProcTable("dream");
			pkb1->setVarName("beads");
			pkb1->setVarName("command");
			
			varUsed.push_back("command");
			pkb1->setUsedVar(1, "command");
			pkb1->setVarName("inspiration");
			
			varUsed.push_back("inspiration");
			pkb1->setUsedVar(3, "inspiration");
			pkb1->setVarName("coffee");
			
			varUsed.push_back("coffee");
			pkb1->setUsedVar(4, "coffee");
			
			varUsed.push_back("beads");
			pkb1->setUsedVar(5, "beads");
			pkb1->setUsedVar(4, "beads");
			pkb1->setUsedVar(5, "command");
			pkb1->setUsedVar(4, "command");
			pkb1->setUsedVar(5, "coffee");
			pkb1->setUsedVar(4, "coffee");
			pkb1->setUsedVar(6, "command");
			pkb1->setUsedVar(4, "command");

			pkb1->setVarName("x");
			varUsed.push_back("x");
			pkb1->setUsedVar(7, "x");
			pkb1->setUsedVar(6, "x");
			pkb1->setUsedVar(4, "x");
			pkb1->setUsedVar(8, "beads");
			pkb1->setUsedVar(4, "beads");
			pkb1->setUsedVar(8, "command");
			pkb1->setUsedVar(4, "command");
			pkb1->setProcUses(0, varUsed);

			ParserForPQL parserPQL = ParserForPQL("procedure p; Select p");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator(*pkb1);

			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults1 = string("dyfyfiyf");

			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				Assert::AreEqual(actualResults1, *it);
			}
		}*/ 
	};
}