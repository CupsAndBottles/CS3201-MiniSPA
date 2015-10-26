#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestDesignExtractor) {
	
	public:
		TEST_METHOD(DE_TestExtractParentT) {
			/*Source code:
			1. while i {
			2.	x = x + 2 + y;
			3.	if x then {
			4.		y = 2 + z; }
				else {
			5.		if y then {
			6.			x = 2 + z;}
					else {
			7.			x = 2 - z;}}}
			*/

			DesignExtractor designE;
			vector<int> expectedResults = { 5,3,1 };
			vector<int> actualResults;

			vector<int>parentCol = { 0,0,1,1,3,3,5,5 };
			actualResults = designE.extractParentT(parentCol, 7);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i),actualResults.at(i));
			}

		}

		TEST_METHOD(DE_TestExtractChildrenT) {
			DesignExtractor designE;
			vector<vector<int>> childrenCol = { {0}, {2,3}, {0}, {4,5}, {0}, {6,7}, {0}, {0} };
			vector<int> expectedResults = {2,3,4,5,6,7};
			vector<int> actualResults;
			
			actualResults = designE.extractChildrenT(childrenCol, 1);
			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i),actualResults.at(i));
			}
		}
		
		TEST_METHOD(DE_TestExtractFollowsT) {
			/*Source code:
			1. x = x + 7;
			2. y = y + 7;
			3. call Planets;
			4. if x then {
			5.		x = x - 7; }
			else {
			6.		y = y - 7; }
			7. y = x + y;
			*/
			DesignExtractor designE;
			vector<int> expectedResults = { 4,3,2,1};
			vector<int> actualResults;

			vector<int>followsCol = {0,0,1,2,3,0,0,4 };
			actualResults = designE.extractFollowsT(followsCol, 7);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}

		TEST_METHOD(DE_TestExtractFollowedByT) {
			DesignExtractor designE;
			vector<int> expectedResults = { 2,3,4,7};
			vector<int> actualResults;

			vector<int>followedByCol = { 0,2,3,4,7,0,0,0 };
			actualResults = designE.extractParentT(followedByCol, 1);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}

		TEST_METHOD(DE_TestExtractCallsT) {
			DesignExtractor designE;
			vector<int> expectedResults = { 1,3,7 };
			vector<int> actualResults;

			vector<vector<int>> callsCol = { {1,3}, {}, {}, {7}, {}, {}, {}, {} };
	
			actualResults = designE.extractCallsT(callsCol,0);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}

		}

		
		TEST_METHOD(DE_TestProcModifiesUses) {
			DesignExtractor designE;
			vector<vector<int>> calls = { {3,2,1}, {3}, {}, {4,2}, {} };
			vector<vector<int>> modifiesCol = { {1}, {}, {1,2}, {3}, {4} };
			vector<vector<int>> expectedResults = { {1,3,4,2}, {3,4,1,2}, {1,2}, {3,4,1,2}, {4} };
			vector<vector<int>> actualResults;

			//vector<int> expectedResults = { 0,3,4,2,1 };
			//vector<int> actualResults = designE.extractProcModifies(calls, modifiesCol);

			//test modifies
			actualResults = designE.extractProcModifiesUses(calls,modifiesCol);
			Assert::AreEqual(expectedResults.size(), actualResults.size());
			//for (int i = 0; i < actualResults.size(); i++) {
				//Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			//}
			//Assert::AreEqual(expectedResults.at(0).at(2), actualResults.at(0).at(2));
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i).size(), actualResults.at(i).size());
				for (int j = 0; j < actualResults.at(i).size(); j++) {
					Assert::AreEqual(expectedResults.at(i).at(j), actualResults.at(i).at(j));
				}
			}
				
	}
		TEST_METHOD(DE_setting) {
			vector<int> updated = { 0,0,0,0,0 };
			updated.at(1) = 1;
			Assert::AreEqual(1, updated.at(1));
		}
	
	};
}