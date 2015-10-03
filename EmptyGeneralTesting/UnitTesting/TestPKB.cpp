#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"

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

		TEST_METHOD(PKB_) {
		}
	};
}