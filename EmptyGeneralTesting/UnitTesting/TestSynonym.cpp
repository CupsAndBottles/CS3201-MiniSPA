#include "stdafx.h"
#include "CppUnitTest.h"
#include "Synonym.h"
#include "Enum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestSynonym)
	{
	public:
		TEST_METHOD(Synonym_Constructor) {
			string synName = "a";
			vector<int> expectedResult;
			expectedResult.push_back(1);
			expectedResult.push_back(2);
			expectedResult.push_back(3);
			Synonym syn(Enum::TYPE::ASSIGN, synName, expectedResult);
			vector<int> actualResult;
			
			Assert::IsTrue(syn.getType() == Enum::TYPE::ASSIGN);
			Assert::AreEqual<string>(syn.getSyn(), synName);
			Assert::IsTrue(syn.getSyn() == synName);

			actualResult = syn.getResult();

			Assert::AreEqual(syn.getResult().size() , (size_t)3);
			for (size_t i = 0; i < actualResult.size();i++) {
				Assert::AreEqual(actualResult[i], expectedResult[i]);
			}
		}

		TEST_METHOD(Synonym_addResult) {
			string synName = "a";
			vector<int> intialResult;
			vector<int> resultToBeAdded;
			intialResult.push_back(1);
			intialResult.push_back(2);
			intialResult.push_back(3);
			resultToBeAdded.push_back(1);

			Synonym syn(Enum::TYPE::ASSIGN, synName, intialResult);
			syn.addResult(resultToBeAdded);
			
			vector<int> actualResult = syn.getResult();

			//Assert::AreEqual(resultToBeAdded.size(), actualResult.size());

			for (size_t i = 0; i < resultToBeAdded.size(); i++) {
				Assert::AreEqual(actualResult[i], resultToBeAdded[i]);
			}


		}



	};
}