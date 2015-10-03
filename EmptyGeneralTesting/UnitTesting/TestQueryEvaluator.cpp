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
			PKB *pkb = PKB::getInstanceOf();

			// Sets stmts
			pkb->setType(Enum::TYPE::ASSIGN); // stmt 1: assignment stmt
			pkb->setType(Enum::TYPE::WHILE); // stmt 2: while stmt
			pkb->setType(Enum::TYPE::ASSIGN); // stmt 3: assignment stmt

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator();
			
			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("1, 3");

			for (std::list<string>::iterator it = results.begin(); it != results.end(); it++) {
				Assert::AreEqual(actualResults, *it);
			}
		} 

		TEST_METHOD(TestEvaluateSuchThatModifies) {
			PKB *pkb = PKB::getInstanceOf();


		}
	};
}