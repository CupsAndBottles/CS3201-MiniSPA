#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestParser)
	{
	public:
		Parser parser;
		TEST_METHOD(TestReadingFile)
		{
		//no such file exists
		string lines = parser.openFile("simple1.txt");
		Assert::AreEqual(lines, string("Unable to open file"));
		
		//file simple exists and input is read. Check if expected = actual result
		lines = parser.openFile("simple.txt");
		Assert::AreEqual(lines, string("1: proceduremain{2: y=5;3: z=7;4: whiles{5: a=x+y*u+(y+9);6: whilej{7: n=q+o*y;}}8: x=y+z;}"));
		
		}
//issues with parent method. Bug - parent is incorrect in multi nesting
		TEST_METHOD(TestParent) {
			string parentChild = parser.getParentChild();
			Assert::AreEqual(parentChild,string(" Parent: 5 Child: 7 Parent: 5 Child: 6 Parent: 3 Child: 4"));
		}

		TEST_METHOD(TestExpression) {
			string expr = parser.getExpression();
			Assert::AreEqual(expr, string("1: y5 2: z7 3: axyu*+y9++ 4: nqoy*+ 5: xyz+ "));
		}

		//follows is incorrect - bracket issues. 
		TEST_METHOD(TestFollows) {
			string followRel = parser.getFollow();
			Assert::AreEqual(followRel, string("|1->2||1->3||4->5||6->6||4->7|"));
		}
	};
}