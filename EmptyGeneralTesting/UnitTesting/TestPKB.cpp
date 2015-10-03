#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Enum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestPKB)
	{
	public:
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
			
			pkb->setType(0);

			Assert::AreEqual(0, pkb->getType(1));
		}
	};
}