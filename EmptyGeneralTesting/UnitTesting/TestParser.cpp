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
		Assert::AreEqual(lines, string("1: proceduremain{2: y=5;3: z=7;4: whiles{5: whilek{6: k=o+p;7: j=o+p;}8: p=o+i;}9: j=i+p;10: z=k+l;}"));
		
		}

		TEST_METHOD(TestParent) {
			string parentChild = parser.getParentChild();
			Assert::AreEqual(parentChild,string("Parent: 3 Child: 4| Parent: 4 Child: 5| Parent: 4 Child: 6| Parent: 3 Child: 7| "));
		}

		TEST_METHOD(TestExpression) {
			string expr = parser.getExpression();
			Assert::AreEqual(expr, string("1: y5; 2: z7; 3: kop;+ 4: jop;+ 5: poi;+ 6: jip;+ 7: zkl;+ "));
		}

		//follows is incorrect - bracket issues. 
		TEST_METHOD(TestFollows) {
			string followRel = parser.getFollow();
			Assert::AreEqual(followRel, string("1->2|2->3|5->6|4->7|3->8|8->9|"));
		}
	};
}