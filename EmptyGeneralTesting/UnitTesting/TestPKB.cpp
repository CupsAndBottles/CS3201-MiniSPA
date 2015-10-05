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

		TEST_METHOD(PKB_getParent) {
			PKB *pkb = new PKB();
			vector<pair<int, int>> children;
			vector<pair<int, int>> expectedResult;
			vector<pair<int, int>> expectedDefinedResult;
			vector<pair<int, int>> actualResults;
			vector<pair<int, int>> emptyResult;

			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			children.push_back(make_pair(1, 2));
			children.push_back(make_pair(1, 3));
			pkb->setChildren(children);

			expectedResult.push_back(make_pair(1, 3));
			expectedResult.push_back(make_pair(1, 2));
			expectedDefinedResult.push_back(make_pair(1, 2));

			// Parent(s1,s2)
			actualResults = pkb->getParent(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResult[i].second, actualResults[i].second);
			}

			// Parent( _, _)
			actualResults = pkb->getParent(Enum::TYPE::UNDERSCORE, UNDEFINED, Enum::TYPE::UNDERSCORE, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResult[i].second, actualResults[i].second);
			}

			// Parent ( 1, 2)
			actualResults = pkb->getParent(Enum::TYPE::WHILE, 1, Enum::TYPE::ASSIGN, 2);
			for (size_t i = 0; i < expectedDefinedResult.size(); i++) {
				Assert::AreEqual(expectedDefinedResult[i].first, actualResults[i].first);
				Assert::AreEqual(expectedDefinedResult[i].second, actualResults[i].second);
			}

			// Parent( w, 2)
			actualResults = pkb->getParent(Enum::TYPE::WHILE, UNDEFINED, Enum::TYPE::STATEMENT, 2);
			for (size_t i = 0; i < expectedDefinedResult.size(); i++) {
				Assert::AreEqual(expectedDefinedResult[i].first, actualResults[i].first);
				Assert::AreEqual(expectedDefinedResult[i].second, actualResults[i].second);
			}

			// Parent( 1, s)
			actualResults = pkb->getParent(Enum::TYPE::WHILE, 1, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResult[i].second, actualResults[i].second);
			}

			// Empty result
			actualResults = pkb->getParent(Enum::TYPE::STATEMENT, 2, Enum::TYPE::STATEMENT, 1);
			for (size_t i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResult[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResult[i].second, actualResults[i].second);
			}
			
			// Parent( w, a) - failed
			actualResults = pkb->getParent(Enum::TYPE::WHILE, UNDEFINED, Enum::TYPE::ASSIGN, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResult[i].second, actualResults[i].second);
			}

			delete pkb;
		}

		TEST_METHOD(PKB_getFollows) {
			PKB *pkb = new PKB();
			
			vector<pair<int, int>> expectedResult;
			vector<pair<int, int>> expectedResult2;
			vector<pair<int, int>> emptyResult;
			vector<pair<int, int>> actualResult;
			
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			expectedResult.push_back(make_pair(2, 3));
			expectedResult2.push_back(make_pair(2, 3));

			pkb->setFollows(follows);
			
			// follows (s1, s2)
			actualResult = pkb->getFollows(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			// follows( _ , _ )
			actualResult = pkb->getFollows(Enum::TYPE::UNDERSCORE, UNDEFINED, Enum::TYPE::UNDERSCORE, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}
			
			// follows( s1, a)
			actualResult = pkb->getFollows(Enum::TYPE::STATEMENT, UNDEFINED,Enum::TYPE::ASSIGN, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			// follows( a, s1)
			actualResult = pkb->getFollows(Enum::TYPE::ASSIGN, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			// follows(a1,a2)
			actualResult = pkb->getFollows(Enum::TYPE::ASSIGN, UNDEFINED, Enum::TYPE::ASSIGN, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}
			
			// follows(a, 3)
			actualResult = pkb->getFollows(Enum::TYPE::ASSIGN, UNDEFINED, Enum::TYPE::STATEMENT, 3);
			for (size_t i = 0; i < expectedResult2.size(); i++) {
				Assert::AreEqual(expectedResult2[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult2[i].first, actualResult[i].first);
			}
			
			// follows(2, a)
			actualResult = pkb->getFollows(Enum::TYPE::STATEMENT, 2, Enum::TYPE::ASSIGN, -1);
			for (size_t i = 0; i < expectedResult2.size(); i++) {
				Assert::AreEqual(expectedResult2[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult2[i].first, actualResult[i].first);
			}

			//follows(2, 3)
			actualResult = pkb->getFollows(Enum::TYPE::STATEMENT, 2, Enum::TYPE::STATEMENT, 3);
			for (size_t i = 0; i < expectedResult2.size(); i++) {
				Assert::AreEqual(expectedResult2[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult2[i].first, actualResult[i].first);
			}

			//follows(5, 4) - non existent entry
			actualResult = pkb->getFollows(Enum::TYPE::STATEMENT, 5, Enum::TYPE::STATEMENT, 4);
			Assert::IsTrue(actualResult == emptyResult);

			delete pkb;
		}

		TEST_METHOD(PKB_getModifies) {
			PKB *pkb = new PKB();
			string varX = "x";
			string varY = "y";
			vector<pair<int, int>> actualResult;
			vector<pair<int, int>> expectedResult;
			vector<pair<int, int>> emptyResult;

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setVarName(varX);
			pkb->setVarName(varY);

			pkb->setModifies(1, varX);
			pkb->setModifies(2, varX);
			pkb->setModifies(2, varY);
			pkb->setModifies(3, varY);
			pkb->setModifies(4, varX);
			pkb->setModifiedBy(varX, 1);
			pkb->setModifiedBy(varX, 2);
			pkb->setModifiedBy(varY, 2);
			pkb->setModifiedBy(varY, 3);
			pkb->setModifiedBy(varX, 4);

			expectedResult.push_back(make_pair(1, 0));
			expectedResult.push_back(make_pair(2, 0));
			expectedResult.push_back(make_pair(2, 1));
			expectedResult.push_back(make_pair(3, 1));
			expectedResult.push_back(make_pair(4, 0));

			actualResult = pkb->getModifies(Enum::TYPE::UNDERSCORE, UNDEFINED, Enum::TYPE::UNDERSCORE, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			actualResult = pkb->getModifies(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			actualResult = pkb->getModifies(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(2, 0));
			expectedResult.push_back(make_pair(2, 1));
			actualResult = pkb->getModifies(Enum::TYPE::STATEMENT, 2, Enum::TYPE::VARIABLE, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(2, 1));
			//expectedResult.push_back(make_pair(3, 1));
			actualResult = pkb->getModifies(Enum::TYPE::WHILE, UNDEFINED, Enum::TYPE::VARIABLE, 1);
			for (size_t i = 0; i < actualResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			
			expectedResult.clear();
			expectedResult.push_back(make_pair(1, 0));
			actualResult = pkb->getModifies(Enum::TYPE::STATEMENT, 1, Enum::TYPE::VARIABLE, 0);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			actualResult = pkb->getModifies(Enum::TYPE::STATEMENT, 3, Enum::TYPE::VARIABLE, 0);
			for (size_t i = 0; i < actualResult.size(); i++) {
				Assert::AreEqual(emptyResult[i].second, actualResult[i].second);
				Assert::AreEqual(emptyResult[i].first, actualResult[i].first);
			}

			
		}

		TEST_METHOD(PKB_getUses) {
			PKB *pkb = new PKB();
			string varX = "x";
			string varY = "y";
			vector<pair<int, int>> actualResult;
			vector<pair<int, int>> expectedResult;
			vector<pair<int, int>> emptyResult;

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setVarName(varX);
			pkb->setVarName(varY);

			pkb->setUsedVar(1, varX);
			pkb->setUsedVar(2, varX);
			pkb->setUsedVar(2, varY);
			pkb->setUsedVar(3, varY);
			pkb->setUsedVar(4, varX);
			pkb->setUsedBy(varX, 1);
			pkb->setUsedBy(varX, 2);
			pkb->setUsedBy(varY, 2);
			pkb->setUsedBy(varY, 3);
			pkb->setModifiedBy(varX, 4);

			expectedResult.push_back(make_pair(1, 0));
			expectedResult.push_back(make_pair(2, 0));
			expectedResult.push_back(make_pair(2, 1));
			expectedResult.push_back(make_pair(3, 1));
			expectedResult.push_back(make_pair(4, 0));

			actualResult = pkb->getUses(Enum::TYPE::UNDERSCORE, UNDEFINED, Enum::TYPE::UNDERSCORE, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			actualResult = pkb->getUses(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			actualResult = pkb->getUses(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(2, 0));
			expectedResult.push_back(make_pair(2, 1));
			actualResult = pkb->getUses(Enum::TYPE::STATEMENT, 2, Enum::TYPE::VARIABLE, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(2, 1));
			expectedResult.push_back(make_pair(3, 1));
			actualResult = pkb->getUses(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::VARIABLE, 1);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}


			expectedResult.clear();
			expectedResult.push_back(make_pair(1, 0));
			actualResult = pkb->getUses(Enum::TYPE::STATEMENT, 1, Enum::TYPE::VARIABLE, 0);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			actualResult = pkb->getUses(Enum::TYPE::STATEMENT, 3, Enum::TYPE::VARIABLE, 0);
			for (size_t i = 0; i < actualResult.size(); i++) {
				Assert::AreEqual(emptyResult[i].second, actualResult[i].second);
				Assert::AreEqual(emptyResult[i].first, actualResult[i].first);
			}

		}

		TEST_METHOD(PKB_extractFollowsT) {
			PKB *pkb = new PKB();
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));

			pkb->setFollows(follows);

			//int follows1 = pkb->getFollows(1);

			int follows2 = pkb->getFollows(2);
			Assert::AreEqual(1, follows2);
			//int follows3 = pkb->getFollows(3);
			//Assert::AreEqual(2, follows3);
			//int follows4 =
 			//pkb->extractFollowsT(3);
			//vector<int> expectedResults = { 3,2,1 };
			//vector<int> actualResults = pkb->getFollowsT(4);

			//Assert::AreEqual(expectedResults.size(), actualResults.size());
			//for (int i = 0; i < actualResults.size(); i++) {
				//Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
		//	}
		}

		TEST_METHOD(PKB_getFollowsT) {
			PKB *pkb = new PKB();
			vector<pair<int, int>> expectedResult;
			vector<pair<int, int>> expectedResult2;
			vector<pair<int, int>> actualResult;
			vector<pair<int, int>> emptyResult;
			
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			
			vector<pair<int, int>> follows;
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			expectedResult.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(2, 3));
			expectedResult.push_back(make_pair(3, 4));
			expectedResult.push_back(make_pair(1 ,3));
			expectedResult.push_back(make_pair(1, 4));
			expectedResult.push_back(make_pair(2, 4));
		
			expectedResult2.push_back(make_pair(2, 3));

			pkb->setFollows(follows);

			// followsT (s1, s2)
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::IsTrue(expectedResult.size() > actualResult.size());
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			// followT( _ , _ )
			actualResult = pkb->getFollowsT(Enum::TYPE::UNDERSCORE, UNDEFINED, Enum::TYPE::UNDERSCORE, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			// followsT( a, s1)
			actualResult = pkb->getFollowsT(Enum::TYPE::ASSIGN, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(2, 3));
			expectedResult.push_back(make_pair(1, 3));

			// followsT( s1, a) - different
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::ASSIGN, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			// followsT( a1, a2)
			actualResult = pkb->getFollowsT(Enum::TYPE::ASSIGN, UNDEFINED, Enum::TYPE::ASSIGN, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(1, 3));
			expectedResult.push_back(make_pair(2, 3));
			// followsT(a, 3)
			actualResult = pkb->getFollowsT(Enum::TYPE::ASSIGN, UNDEFINED, Enum::TYPE::STATEMENT, 3);
			for (size_t i = 0; i < expectedResult2.size(); i++) {
				Assert::AreEqual(expectedResult2[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult2[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(2, 3));
			// followsT(2, a)
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, 2, Enum::TYPE::ASSIGN, -1);
			for (size_t i = 0; i < expectedResult2.size(); i++) {
				Assert::AreEqual(expectedResult2[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult2[i].first, actualResult[i].first);
			}

			//followsT(2, 3)
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, 2, Enum::TYPE::STATEMENT, 3);
			for (size_t i = 0; i < expectedResult2.size(); i++) {
				Assert::AreEqual(expectedResult2[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult2[i].first, actualResult[i].first);
			}

			//follows(5, 4) - non existent entry
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, 4, Enum::TYPE::STATEMENT, 3);
			Assert::IsTrue(actualResult == emptyResult);

			delete pkb;
		}

		TEST_METHOD(PKB_getUsedByStmtNum) {
			PKB *pkb = new PKB();
			vector<int> used;
			vector<int> expectedResult;
			string var1 = "x";
			string var2 = "y";


			pkb->setVarName(var1);
			pkb->setVarName(var2);
			pkb->setType(Enum::ASSIGN);
			pkb->setType(Enum::WHILE);
			pkb->setType(Enum::ASSIGN);
			pkb->setUsedBy(var1, 1);
			pkb->setUsedBy(var2, 2);
			pkb->setUsedBy(var2, 3);

			used = pkb -> getUsedByStmtNum(0);
			expectedResult.push_back(1);

			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i], used[i]);
			}

			delete pkb;
		}

		TEST_METHOD(PKB_getNoOfStmt) {
			PKB *pkb = new PKB();
			int NumOfStmt = 0;

			Assert::AreEqual(NumOfStmt, pkb->getNoOfStmt());

			pkb->setType(Enum::TYPE::ASSIGN);
			NumOfStmt = 1;

			Assert::AreEqual(NumOfStmt, pkb->getNoOfStmt());

			delete pkb;
		}

		TEST_METHOD(PKB_getProcNameInProcTable) {
			PKB *pkb = new PKB();
			int index;
			string procName = "TestProc";

			index = pkb->setProcNameInProcTable(procName);

			Assert::AreEqual(procName, pkb->getProcName(index));

			delete pkb;
		}

		TEST_METHOD(PKB_getVarIndex) {
			PKB *pkb = new PKB();
			string varName = "forTesting";

			pkb->setVarName(varName);

			Assert::AreEqual(0, pkb->getVarIndex(varName));

			delete pkb;

		}

		TEST_METHOD(PKB_getVarName) {
			PKB *pkb = new PKB();
			int index;
			string varName = "forTesting";

			index = pkb->setVarName(varName);

			Assert::AreEqual(varName, pkb->getVarName(index));

			delete pkb;
		}


		TEST_METHOD(PKB_getType) {
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::ASSIGN);

			Assert::IsTrue(Enum::TYPE::ASSIGN == pkb->getType(1));

			delete pkb;
		}

		TEST_METHOD(PKB_getRightExpression) {
			PKB *pkb = new PKB();

			string rightExpr = "x";

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setRightExpr(1, rightExpr);

			Assert::AreEqual(rightExpr, pkb->getRightExpr(1));

			delete pkb;
		}

	};
}