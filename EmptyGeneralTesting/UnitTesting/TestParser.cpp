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
			Assert::AreEqual(lines, string("1: procedureABC{2: i=1;3: b=200;4: c=a;5: whilea{6: whilebeta{7: oSCar=1+beta+tmp;8: whiletmp{9: oSCar=I+k+j1k+chArlie;}10: whilex{11: x=x+1;12: whileleft{13: whileright{14: Romeo=Romeo+1;15: b=0;16: c=delta+l+width+Romeo;}17: whilec{18: c=c+1;}19: x=x+1;}}20: a=2;}21: w=w+1;}}"));

		}

		TEST_METHOD(TestParent) {
			string parentChild = parser.getParentChild();
			Assert::AreEqual(parentChild, string("Parent: 5 Child: 20| Parent: 9 Child: 19| Parent: 11 Child: 18| Parent: 16 Child: 17| Parent: 11 Child: 16| Parent: 12 Child: 15| Parent: 12 Child: 14| Parent: 12 Child: 13| Parent: 11 Child: 12| Parent: 9 Child: 11| Parent: 9 Child: 10| Parent: 5 Child: 9| Parent: 7 Child: 8| Parent: 5 Child: 7| Parent: 5 Child: 6| Parent: 4 Child: 5| "));
		}

		TEST_METHOD(TestExpression) {
			string expr = parser.getExpression();
			Assert::AreEqual(expr, string("1: 1 2: 200 3: a 4: 1 beta + tmp +  5: I k + j1k + chArlie +  6: x 1 +  7: Romeo 1 +  8: 0 9: delta l + width + Romeo +  10: c 1 +  11: x 1 +  12: 2 13: w 1 +  "));
		}

		//follows is incorrect - bracket issues. 
		TEST_METHOD(TestFollows) {
			string followRel = parser.getFollow();
			Assert::AreEqual(followRel, string("5->20|9->19|16->18|12->16|14->15|13->14|10->11|7->9|6->7|3->4|2->3|1->2|"));
		}
	};
}