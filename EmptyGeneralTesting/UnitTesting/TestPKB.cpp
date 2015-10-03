#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Enum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const int UNDEFINED = -1;

namespace UnitTesting
{
	TEST_CLASS(TestPKB)
	{
	public:
		/**
		TEST_METHOD_CLEANUP(cleanEachMethod) {
			PKB::destroyInstance();
		}
		**/

		TEST_METHOD(PKB_getNoOfStmt) {
			PKB *pkb = PKB::getInstanceOf();
			int NumOfStmt = 0;

			Assert::AreEqual(NumOfStmt, pkb->getNoOfStmt());

			pkb->setType(Enum::TYPE::ASSIGN);
			NumOfStmt = 1;

			Assert::AreEqual(NumOfStmt, pkb->getNoOfStmt());
		}

		TEST_METHOD(PKB_getProcNameInProcTable) {
			PKB *pkb = PKB::getInstanceOf();
			int index; 
			string procName = "TestProc";
			
			index = pkb->setProcNameInProcTable(procName);

			Assert::AreEqual(procName, pkb->getProcName(index));
		}

		TEST_METHOD(PKB_getVarName) {
			PKB *pkb = PKB::getInstanceOf();
			int index;
			string varName = "TestVarName";

			index = pkb->setVarName(varName);

			Assert::AreEqual(varName, pkb->getVarName(index));
		}


		TEST_METHOD(PKB_getType){
			PKB *pkb = PKB::getInstanceOf();
			
			pkb->setType(Enum::TYPE::ASSIGN);

			Assert::IsTrue(Enum::TYPE::ASSIGN == pkb->getType(1));
		}

		TEST_METHOD(PKB_getRightExpression) {
			PKB *pkb = PKB::getInstanceOf();
			string rightExpr = "x";

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setRightExpr(1, rightExpr);

			Assert::AreEqual(rightExpr, pkb->getRightExpr(1));

		}

		TEST_METHOD(PKB_getParent) {
			PKB *pkb = PKB::getInstanceOf();

			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			
			vector<pair<int, int>> children;

			children.push_back(make_pair(1, 2));
			pkb->setChildren(children);

			vector<pair<int, int>> expectedResults;
			vector<pair<int, int>> actualResults;
			expectedResults.push_back(make_pair(1,2));

			actualResults = pkb->getParent(Enum::TYPE::WHILE, 1, Enum::TYPE::ASSIGN, 2);
			
			for (size_t i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResults[i].second, actualResults[i].second);

			}

//			Assert::IsTrue(expectedResults == pkb->getParent(Enum::TYPE::STATEMENT, -1, Enum::TYPE::STATEMENT, -1));
//		Assert::IsTrue(expectedResults == pkb->getParent(Enum::TYPE::STATEMENT, 1, Enum::TYPE::STATEMENT, 2));
		}

		TEST_METHOD(PKB_getFollows) {
			PKB *pkb = PKB::getInstanceOf();
			vector<pair<int, int>> expectedResult;
			vector<pair<int, int>> actualResult;
			
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			expectedResult.push_back(make_pair(2, 3));
			
			pkb->setFollows(follows);
			actualResult = pkb->getFollows(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);

			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
			}
			
		}



	};
}