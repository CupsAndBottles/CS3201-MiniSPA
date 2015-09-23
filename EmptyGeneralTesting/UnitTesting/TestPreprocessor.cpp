#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParserForPQL.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestPQLParser)
	{
	public:
		TEST_METHOD(TestResultTree)
		{
		//	enum Type { underscore, variable, assignment, procedure, calls, equation, stmt, constant, BOOLEAN, While, If };
			string input = "while w1, w2, w3; stmt s, n; Select <w1,w2, w3> with n = 10 pattern w1(\"x\", _) such that Follows(n, s)";
			//"while w1;prog_line n; stmt s; Select w1 such that Follows(n, s) pattern(\"x\", _)"
			ParserForPQL parser(input);
			QueryTree queryTree = parser.getQueryTree();

			Assert::AreEqual(queryTree.getResultTree().at(0).getParent().getStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getResultTree().at(0).getParent().getType()), 8);
			Assert::AreEqual(queryTree.getResultTree().at(1).getParent().getStringValue(), string("w2"));
			Assert::AreEqual(int(queryTree.getResultTree().at(1).getParent().getType()), 8);
			Assert::AreEqual(queryTree.getResultTree().at(2).getParent().getStringValue(), string("w3"));
			Assert::AreEqual(int(queryTree.getResultTree().at(2).getParent().getType()), 8);

			input = "while w1; prog_line n; stmt s; Select w1 such that Follows(n, s) pattern w1(\"x\", _)";
			ParserForPQL parser1(input);
			queryTree = parser1.getQueryTree();

			Assert::AreEqual(queryTree.getResultTree().at(0).getParent().getStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getResultTree().at(0).getParent().getType()), 8);
		}

		TEST_METHOD(testSuchThatTree)
		{
			string input = "while w1, w2, w3; assign a, n, s; Select <w1,w2, w3> with n = 10 pattern a(\"x\", _) such that Follows(n, s)";
			ParserForPQL parser2(input);
			QueryTree queryTree = parser2.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Follows"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("n"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 2);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("s"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 2);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);


			input = "assign a; stmt s; prog_line n; Select a such that Follows*(s, n) pattern a(\"x\", _)";
			ParserForPQL parser3(input);
			queryTree = parser3.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Follows*"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("s"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("n"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 6);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
		
			input = "assign a; Select a such that Uses(a, \"x\") pattern a(\"y\", _)";
			ParserForPQL parser4(input);
			queryTree = parser4.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Uses"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("a"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 2);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("\"x\""));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 1);
			//attention:::::::need to change from pkb
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);


			input = "while w1; assign a; Select w1 such that Modifies(w1, \"x\") pattern a(\"x\", _)";
			ParserForPQL parser5(input);
			queryTree = parser5.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Modifies"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 8);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("\"x\""));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);

			input = "while w1; stmt s; prog_line n; assign a; Select <w1, a> such that Parent(w1, _) pattern a(\"x\", _)";
			ParserForPQL parser6(input);
			queryTree = parser6.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Parent"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 8);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 0);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -2);

			input = "while w1; assign a; prog_line n; Select w1 such that Parent*(w1, \"2\") pattern a(\"x\", _)";
			ParserForPQL parser7(input);
			queryTree = parser7.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Parent*"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 8);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 6);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), 2);
			//Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), true);
		}

		TEST_METHOD(testPatternTree) {
			string input = "while w1, w2, w3; assign a, n; Select <w1,w2, w3> with n = 10 pattern a(\"x\", _) such that Follows(n, a)";
			ParserForPQL parser8(input);
			QueryTree queryTree = parser8.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("\"x\""));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 0);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = "while w1, w2, w3; assign a, n; Select <w1,w2, w3> with n = 10 pattern a(\"x\", 2) such that Follows(n, a)";
			ParserForPQL parser9(input);
			queryTree = parser9.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("\"x\""));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("2"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 7);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = "while w1, w2, w3; assign a, n; variable v; Select <w1,w2, w3> with n = 10 pattern a(v, 2) such that Follows(n, a)";
			ParserForPQL parser10(input);
			queryTree = parser10.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("2"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 7);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = "while w1, w2, w3; assign a, n; variable v; Select <w1,w2, w3> with n = 10 pattern a(v, _x+2_) such that Follows(n, a)";
			ParserForPQL parser11(input);
			queryTree = parser11.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("x+2"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 5);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), true);

		
		}
	};
}