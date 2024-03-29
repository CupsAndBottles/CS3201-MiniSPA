#include "stdafx.h"
#include "CppUnitTest.h"

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

		TEST_METHOD(PKB_extractParentT) {
			DesignExtractor designE;
			PKB *pkb = new PKB();
			vector<int> actualResults;
			vector<int> expectedResults = { 2,1 };

			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);

			pkb->setParent(1, 0);
			pkb->setParent(2, 1);
			pkb->setParent(3, 2);

			pkb->extractParentT(3);
			actualResults = pkb->getParentT(3);

			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}

		TEST_METHOD(PKB_setByDesignExtractor) {
			PKB *pkb = new PKB();
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);

			vector<pair<int, int>> children;
			vector<int> expectedResults;
			vector<int> actualResults;

			children.push_back(make_pair(1, 2));
			children.push_back(make_pair(2, 3));
			pkb->setChildren(3, 0);
			pkb->setChildren(children);

			vector<pair<int, string>> procCalls;

			pkb->setProcNameInProcTable("Main");
			pkb->setProcNameInProcTable("Planet");
			pkb->setProcNameInProcTable("La");
			pkb->setProcNameInProcTable("Sun");

			procCalls.push_back(make_pair(0, "Planet"));
			procCalls.push_back(make_pair(1, "Sun"));
			pkb->setProcCalls(procCalls);

			pkb->setByDesignExtractor();
			pkb->setParentTChildrenT();

			//test if parentT
			expectedResults = { 2,1 };
			actualResults = pkb->getParentT(3);
			Assert::AreEqual(expectedResults.size(), actualResults.size());

			for (int i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}

			//test for childrenT
			expectedResults.clear();
			expectedResults = { 2,3 };
			actualResults = pkb->getChildrenT(1);
			Assert::AreEqual(expectedResults.size(), actualResults.size());

			for (int i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}

			//test for callsT
			expectedResults.clear();
			expectedResults = { 1,3 };

			actualResults = pkb->getCallsT(0);
			Assert::AreEqual(expectedResults.size(), actualResults.size());

			for (int i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}

		TEST_METHOD(PKB_getParentT) {
			/*Source eg.
			1. while x {
			2.	while y {
			3.		x = x-7; } }
			*/

			PKB *pkb = new PKB();
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);

			vector<pair<int, int>> children;
			vector<pair<int, int>> expectedResults;
			vector<pair<int, int>> actualResults;

			children.push_back(make_pair(1, 2));
			children.push_back(make_pair(2, 3));
			pkb->setChildren(3, 0);
			pkb->setChildren(children);

			pkb->setParentTChildrenT();

			expectedResults.push_back(make_pair(1, 2));
			expectedResults.push_back(make_pair(1, 3));
			expectedResults.push_back(make_pair(2, 3));

			//Assert::AreEqual(0, pkb->getChildren(3).at(0));

			// ParentT(s1, s2)
			actualResults = pkb->getParentT(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (size_t i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResults[i].second, actualResults[i].second);
			}


			//Parent( _, _)
			actualResults = pkb->getParentT(Enum::TYPE::UNDERSCORE, UNDEFINED, Enum::TYPE::UNDERSCORE, UNDEFINED);
			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (size_t i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResults[i].second, actualResults[i].second);
			}


			// ParentT ( 1, 2)
			expectedResults.clear();
			actualResults.clear();
			expectedResults.push_back(make_pair(1, 2));

			actualResults = pkb->getParentT(Enum::TYPE::WHILE, 1, Enum::TYPE::WHILE, 2);
			for (size_t i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResults[i].second, actualResults[i].second);
			}

			// ParentT( w, 3)
			expectedResults.clear();
			actualResults.clear();
			expectedResults.push_back(make_pair(2, 3));
			expectedResults.push_back(make_pair(1, 3));

			actualResults = pkb->getParentT(Enum::TYPE::WHILE, UNDEFINED, Enum::TYPE::STATEMENT, 3);
			for (size_t i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResults[i].second, actualResults[i].second);
			}


			expectedResults.clear();
			expectedResults.push_back(make_pair(1, 2));
			expectedResults.push_back(make_pair(1, 3));


			// Parent( 1, s)
			actualResults = pkb->getParentT(Enum::TYPE::WHILE, 1, Enum::TYPE::STATEMENT, UNDEFINED);
			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (size_t i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResults[i].second, actualResults[i].second);
			}


			vector<pair<int, int>> empty;
			// Empty result
			actualResults = pkb->getParentT(Enum::TYPE::STATEMENT, 2, Enum::TYPE::STATEMENT, 1);
			for (size_t i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(empty[i].first, actualResults[i].first);
				Assert::AreEqual(empty[i].second, actualResults[i].second);
			}

			// Parent( w, a)
			expectedResults.clear();
			expectedResults.push_back(make_pair(1, 3));
			expectedResults.push_back(make_pair(2, 3));
			actualResults = pkb->getParentT(Enum::TYPE::WHILE, UNDEFINED, Enum::TYPE::ASSIGN, UNDEFINED);

			for (size_t i = 0; i < expectedResults.size(); i++) {
				Assert::AreEqual(expectedResults[i].first, actualResults[i].first);
				Assert::AreEqual(expectedResults[i].second, actualResults[i].second);
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
			actualResult = pkb->getFollows(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::ASSIGN, UNDEFINED);
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
		/*
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
		*/

		TEST_METHOD(PKB_getFollowsT) {
			PKB *pkb = new PKB();
			vector<pair<int, int>> expectedResult;
			vector<pair<int, int>> actualResult;
			vector<pair<int, int>> emptyResult;

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);

			vector<pair<int, int>> follows;
			follows.push_back(make_pair(0, 1));
			follows.push_back(make_pair(1, 2));
			follows.push_back(make_pair(2, 3));
			follows.push_back(make_pair(3, 4));
			follows.push_back(make_pair(4, 0));

			expectedResult.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(1, 3));
			expectedResult.push_back(make_pair(1, 4));
			expectedResult.push_back(make_pair(2, 3));
			expectedResult.push_back(make_pair(2, 4));
			expectedResult.push_back(make_pair(3, 4));

			pkb->setFollows(follows);
			pkb->setByDesignExtractor();

			// followsT (s1, s2)
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);

			Assert::AreEqual(expectedResult.size(), actualResult.size());
			for (size_t i = 0; i < expectedResult.size(); i++) {

				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			actualResult.clear();
			// followT( _ , _ )
			actualResult = pkb->getFollowsT(Enum::TYPE::UNDERSCORE, UNDEFINED, Enum::TYPE::UNDERSCORE, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(1, 3));
			expectedResult.push_back(make_pair(1, 4));
			expectedResult.push_back(make_pair(2, 3));
			expectedResult.push_back(make_pair(2, 4));
			// followsT( a, s1)
			actualResult = pkb->getFollowsT(Enum::TYPE::ASSIGN, UNDEFINED, Enum::TYPE::STATEMENT, UNDEFINED);
			Assert::AreEqual(expectedResult.size(), actualResult.size());
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(3, 4));
			expectedResult.push_back(make_pair(2, 4));
			expectedResult.push_back(make_pair(1, 4));

			// followsT( s1, a) - different
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, UNDEFINED, Enum::TYPE::ASSIGN, UNDEFINED);
			Assert::AreEqual(expectedResult.size(), actualResult.size());
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(1, 4));
			expectedResult.push_back(make_pair(2, 4));
			// followsT( a1, a2)
			actualResult = pkb->getFollowsT(Enum::TYPE::ASSIGN, UNDEFINED, Enum::TYPE::ASSIGN, UNDEFINED);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			//expectedResult.push_back(make_pair(2, 3));
			//expectedResult.push_back(make_pair(1, 3));

			// followsT(w, 3)
			actualResult = pkb->getFollowsT(Enum::TYPE::WHILE, UNDEFINED, Enum::TYPE::STATEMENT, 3);
			Assert::AreEqual(expectedResult.size(), actualResult.size());
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.clear();
			expectedResult.push_back(make_pair(2, 4));
			// followsT(2, a)
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, 2, Enum::TYPE::ASSIGN, -1);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			expectedResult.pop_back();
			//followsT(2, 3)
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, 2, Enum::TYPE::STATEMENT, 3);
			for (size_t i = 0; i < expectedResult.size(); i++) {
				Assert::AreEqual(expectedResult[i].second, actualResult[i].second);
				Assert::AreEqual(expectedResult[i].first, actualResult[i].first);
			}

			//follows(5, 4) - non existent entry
			actualResult = pkb->getFollowsT(Enum::TYPE::STATEMENT, 4, Enum::TYPE::STATEMENT, 3);
			Assert::IsTrue(actualResult == emptyResult);

			delete pkb;
		}

		TEST_METHOD(PKB_extractFollowedByT) {
			PKB *pkb = new PKB();
			vector<int> actualResults;
			vector<int> expectedResults = { 2,3,4 };
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			vector<pair<int, int>>followedBy;
			followedBy.push_back(make_pair(0, 1));
			followedBy.push_back(make_pair(1, 2));
			followedBy.push_back(make_pair(2, 3));
			followedBy.push_back(make_pair(3, 4));
			followedBy.push_back(make_pair(4, 0));
			pkb->setFollows(followedBy);

			//Assert::AreEqual(2,pkb->getFollowedBy(1));
			//Assert::AreEqual(3,pkb->getFollowedBy(2));
			//Assert::AreEqual(4, pkb->getFollowedBy(3));
			//Assert::AreEqual(0,pkb->getFollowedBy(4));

			pkb->extractFollowedByT(1);
			actualResults = pkb->getFollowedByT(1);

			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}

		TEST_METHOD(PKB_getCallsT) {
			PKB *pkb = new PKB();
			vector<pair<int,int>> expectedResults;
			vector<pair<int,int>> actualResults;
		}
		
		TEST_METHOD(PKB_extractNextT) {

			PKB *pkb = new PKB();

			vector<int> expectedResults = { 2,3 };

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			pkb->setNext(1, 2);
			pkb->setNext(2, 3);
		
			vector<int> actualResults = pkb->extractNextT(1);
			Assert::AreEqual(expectedResults.size(), actualResults.size());

			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}

		TEST_METHOD(PKB_getNextT) {
			PKB *pkb = new PKB();

			vector<pair<int,int>> expectedResults;
			expectedResults.push_back(make_pair(1, 3));
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::ASSIGN);

			pkb->setNext(1, 2);
			pkb->setNext(2, 3);

			vector<pair<int,int>> actualResults = pkb->getNextT(Enum::TYPE::STATEMENT, 1, Enum::TYPE::STATEMENT, 3);
			Assert::AreEqual(expectedResults.size(), actualResults.size());

			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i).first, actualResults.at(i).first);
				Assert::AreEqual(expectedResults.at(i).second, actualResults.at(i).second);
			}

			actualResults = pkb->getNextT(Enum::TYPE::STATEMENT, -1, Enum::TYPE::WHILE, -1);
			expectedResults.clear();
			expectedResults.push_back(make_pair(1, 2));
			//expectedResults.push_back(make_pair(1, 3));
			//expectedResults.push_back(make_pair(2, 3));

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i).first, actualResults.at(i).first);
				Assert::AreEqual(expectedResults.at(i).second, actualResults.at(i).second);
			}
		}

		TEST_METHOD(PKB_setCallsStmtModifiesUses) {
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::CALLS);
			pkb->setType(Enum::TYPE::CALLS);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::CALLS);
			
			pkb->setProcNameInProcTable("Main");
			pkb->setProcNameInProcTable("La");
			pkb->setProcNameInProcTable("Sun");
			pkb->setProcNameInProcTable("Moon");

			pkb->setVarName("x");
			pkb->setVarName("y");
			pkb->setVarName("z");
			pkb->setVarName("r");

			vector<pair<int, string>> procModified;
			procModified.push_back(make_pair(0, "x"));
			procModified.push_back(make_pair(0, "y"));
			procModified.push_back(make_pair(1, "x"));
			procModified.push_back(make_pair(2, "z"));
			procModified.push_back(make_pair(2, "r"));
			procModified.push_back(make_pair(3, "x"));
			procModified.push_back(make_pair(3, "y"));
			procModified.push_back(make_pair(3, "z"));
			pkb->setProcModified(procModified);
			vector<pair<int, string>> procCalls;
			procCalls.push_back(make_pair(0, "Sun"));
			procCalls.push_back(make_pair(1, "Sun"));
			procCalls.push_back(make_pair(1, "Moon"));

			pkb->setProcCalls(procCalls);
			pkb->extractProcExtraModifiesUses();

			vector<pair<int, string>> stmtCalled;
			stmtCalled.push_back(make_pair(1, "Sun"));
			stmtCalled.push_back(make_pair(2, "Sun"));
			stmtCalled.push_back(make_pair(4, "Moon"));
			pkb->setStmtNumProcCalled(stmtCalled);
			vector<int> procs = pkb->getStmtNumProcCalled(2);
			
			Assert::AreEqual(2, procs.at(0));
			Assert::AreEqual(1, procs.at(1));
			pkb->setCallsStmtModifiesUses();

			vector<int> test = pkb->getProcModified(2);
			Assert::AreEqual(3, test.at(0));
			Assert::AreEqual(2,test.at(1));

			vector<int> expectedResults = { 3,2 };
			vector<int> actualResults = pkb->getModifiesForParser(1);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}

			//test set modifies Parent of Call statement
			vector<int> parentT = { 3 };
			pkb->setParentT(4, parentT);
			pkb->setModifies(3, "x");

			pkb->setCallStmtsParentTModifiesUses();
			expectedResults = { 0,2,1 };
			actualResults = pkb->getModifiesForParser(3);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}
		
		TEST_METHOD(PKB_ExtractProcExtraModifiesUses) {
			PKB *pkb = new PKB();

			pkb->setProcNameInProcTable("Main");
			pkb->setProcNameInProcTable("La");
			pkb->setProcNameInProcTable("Sun");
			pkb->setProcNameInProcTable("Planet");

			pkb->setVarName("w");
			pkb->setVarName("x");
			pkb->setVarName("y");
			pkb->setVarName("z");
			pkb->setVarName("v");
			pkb->setVarName("m");
			pkb->setVarName("n");
			pkb->setVarName("r");

			vector<pair<int, string>> procModified;
			procModified.push_back(make_pair(0, "y"));
			procModified.push_back(make_pair(0, "x"));
			

			procModified.push_back(make_pair(1, "x"));
			procModified.push_back(make_pair(1, "y"));
			procModified.push_back(make_pair(1, "z"));

			procModified.push_back(make_pair(3, "x"));
			procModified.push_back(make_pair(3, "y"));
			procModified.push_back(make_pair(3, "r"));
			procModified.push_back(make_pair(3, "v"));
			procModified.push_back(make_pair(3, "m"));
			pkb->setProcModified(procModified);

			vector<pair<int, string>> calls;
			calls.push_back(make_pair(0, "La"));
			calls.push_back(make_pair(1, "Planet"));
			pkb->setProcCalls(calls);
			pkb->extractProcExtraModifiesUses();
			
			vector<int> actualResults = pkb->getProcModified(0);
			vector<int> expectedResults = { 1,2,3,5,4,7 };

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}

		}
		/*
		TEST_METHOD(PKB_setCallsStmtModifiesUses) {
			PKB *pkb = new PKB();

			pkb->setType(Enum::TYPE::CALLS);
			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::ASSIGN);

			pkb->setProcNameInProcTable("Main");
			pkb->setProcNameInProcTable("La");

			pkb->setVarName("x");
			pkb->setVarName("y");
			
			vector<pair<int, string>> procModified;
			procModified.push_back(make_pair(0, "x"));
			procModified.push_back(make_pair(0, "y"));
			pkb->setProcModified(procModified);

			procModified.clear();
			procModified.push_back(make_pair(1, "x"));
			pkb->setProcModified(procModified);
			vector<pair<int, string>> pair;
			pair.push_back(make_pair(1, "La"));
			pkb->setStmtNumProcCalled(pair);

			pkb->setCallsStmtModifiesUses();

			vector<int> expectedResults = { 0 };
			vector<int> actualResults = pkb->getModifiesForParser(1);
			Assert::AreEqual(expectedResults.size(), actualResults.size());
			Assert::AreEqual(expectedResults.at(0), actualResults.at(0));
			
		}
		*/

		TEST_METHOD(PKB_getCalls) {
			PKB *pkb = new PKB();
			pkb->setProcNameInProcTable("morning");
			pkb->setProcNameInProcTable("afternoon");
			pkb->setProcNameInProcTable("evening");
			pkb->setProcNameInProcTable("night");
			pkb->setProcNameInProcTable("midnight");

			vector<pair<int, string>> calls;
			calls.push_back(make_pair(0, "afternoon"));
			calls.push_back(make_pair(1, "evening"));
			calls.push_back(make_pair(2, "night"));
			calls.push_back(make_pair(3, "midnight"));

			pkb->setProcCalls(calls);

			// Calls(p, p1)
			vector<pair<int, int>> actualResult = pkb->getCalls(UNDEFINED, UNDEFINED);
			vector<pair<int, int>> expectedResult;
			
			expectedResult.push_back(make_pair(0,1));
			expectedResult.push_back(make_pair(1, 2));
			expectedResult.push_back(make_pair(2, 3));
			expectedResult.push_back(make_pair(3, 4));
			
			Assert::IsTrue(actualResult == expectedResult);
			
			// Calls(1, p)
			actualResult.clear();
			expectedResult.clear();

			actualResult = pkb->getCalls(1, UNDEFINED);
			expectedResult.push_back(make_pair(1, 2));
			
			Assert::IsTrue(actualResult == expectedResult);
			
			// Calls(p, 1)
			actualResult.clear();
			expectedResult.clear();

			actualResult = pkb->getCalls(UNDEFINED, 3);
			expectedResult.push_back(make_pair(2, 3));

			Assert::IsTrue(actualResult == expectedResult);

			// Calls(1,2)
			actualResult.clear();
			expectedResult.clear();
			actualResult = pkb->getCalls(4, 3);
			
			Assert::IsTrue(actualResult == expectedResult);

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

		TEST_METHOD(PKB_getControlVar) {
			PKB *pkb = new PKB();
			string varX = "x";
			string varY = "y";

			pkb->setType(Enum::TYPE::ASSIGN);
			pkb->setType(Enum::TYPE::WHILE);
			pkb->setType(Enum::TYPE::IF);
			pkb->setVarName(varX);
			pkb->setVarName(varY);

			pkb->setControlVar(2, 0);
			pkb->setControlVar(3, 1);

			Assert::AreEqual(0, pkb->getControlVar(2));
			Assert::AreEqual(1, pkb->getControlVar(3));
		}

	};
}