#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestDesignExtractor) {
	
	public:
		TEST_METHOD(TestExtractParentT) {
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

			vector<int>parentCol = { -1,-1,1,1,3,3,5,5 };
			actualResults = designE.extractParentT(parentCol, 7);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i),actualResults.at(i));
			}

		}

		TEST_METHOD(TestExtractChildrenT) {
			DesignExtractor designE;
			vector<vector<int>> childrenCol = { {-1}, {2,3}, {-1}, {4,5}, {-1}, {6,7}, {-1}, {-1} };
			vector<int> expectedResults = {2,3,4,5,6,7};
			vector<int> actualResults;
			
			actualResults = designE.extractChildrenT(childrenCol, 1);
			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i),actualResults.at(i));
			}
		}
		
		TEST_METHOD(TestExtractFollowsT) {
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

			vector<int>followsCol = {-1,-1,1,2,3,-1,-1,4 };
			actualResults = designE.extractFollowsT(followsCol, 7);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}

		TEST_METHOD(TestExtractFollowedByT) {
			DesignExtractor designE;
			vector<int> expectedResults = { 2,3,4,7};
			vector<int> actualResults;

			vector<int>followedByCol = { -1,2,3,4,7,-1,-1,-1 };
			actualResults = designE.extractParentT(followedByCol, 1);

			Assert::AreEqual(expectedResults.size(), actualResults.size());
			for (int i = 0; i < actualResults.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), actualResults.at(i));
			}
		}
	
	};
}