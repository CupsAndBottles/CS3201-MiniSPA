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
			pkb->setType(Enum::TYPE::ASSIGN); // beads = command + 10
			pkb->setType(Enum::TYPE::WHILE); // while coffee
			pkb->setType(Enum::TYPE::ASSIGN); // command = inspiration + beads

			// Sets variables
			pkb->setVarName("beads");
			pkb->setVarName("command");
			pkb->setVarName("inspiration");

			// Sets parent and children
/*			pkb->setParent(3, 2);
			pkb->setChildren(vector<pair<int, int>> {(2, 3)});
*/
			// Sets modifies and uses
			pkb->setModifies(1, "beads");
			pkb->setUsedVar(1, "command");
			pkb->setUsedVar(2, "coffee");
			pkb->setModifies(3, "command");
			pkb->setModifies(2, "command");
			pkb->setUsedVar(3, "inspiration");
			pkb->setUsedVar(2, "inspiration");
			pkb->setUsedVar(3, "beads");
			pkb->setUsedVar(2, "beads");

			ParserForPQL parserPQL = ParserForPQL("assign a; Select a");
			QueryTree queryTree = parserPQL.getQueryTree();
			QueryEvaluator queryEvaluator = QueryEvaluator();
			
			list<string> results = queryEvaluator.evaluateQuery(queryTree);
			string actualResults = string("1, 3");

			Assert::AreEqual(actualResults, results.front());
		}
	};
}