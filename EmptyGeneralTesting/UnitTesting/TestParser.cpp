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
			Assert::AreEqual(lines, string("1: proceduremain{2: y=5;3: whiles{4: j=i+p;5: z=k+l;6: x=s+l;7: l=o+k;}}"));

		}

		TEST_METHOD(TestParent) {
			string parentChild = parser.getParentChild();
			Assert::AreEqual(parentChild, string("Parent: 2 Child: 6| Parent: 2 Child: 5| Parent: 2 Child: 4| Parent: 2 Child: 3| "));
		}

		TEST_METHOD(TestExpression) {
			string expr = parser.getExpression();
			Assert::AreEqual(expr, string("1: y5 2: jip+ 3: zkl+ 4: xsl+ 5: lok+ "));
		}

		//follows is incorrect - bracket issues. 
		TEST_METHOD(TestFollows) {
			string followRel = parser.getFollow();
			Assert::AreEqual(followRel, string("5->6|4->5|3->4|1->2|"));
		}
	};
}