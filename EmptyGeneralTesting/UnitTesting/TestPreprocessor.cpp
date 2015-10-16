#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParserForPQL.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestPQLParser)
	{
	public:
		TEST_METHOD(TestResultTree)
		{
			PKB *pkb = new PKB();
			pkb->setVarName("x");
			pkb->setVarName("y");

			string input = "while w1, w2, w3; stmt s, n; Select <w1,w2, w3> pattern w1(\"x\", _) such that Follows(n, s)";
			//"while w1;prog_line n; stmt s; Select w1 such that Follows(n, s) pattern(\"x\", _)"
			ParserForPQL parser(input);
			QueryTree queryTree = parser.getQueryTree();

			Assert::AreEqual(queryTree.getResultTree().at(0).getParentStringVal(), string("w1"));
			Assert::AreEqual(int(queryTree.getResultTree().at(0).getParentType()), 4);
			Assert::AreEqual(queryTree.getResultTree().at(1).getParentStringVal(), string("w2"));
			Assert::AreEqual(int(queryTree.getResultTree().at(1).getParentType()), 4);
			Assert::AreEqual(queryTree.getResultTree().at(2).getParentStringVal(), string("w3"));
			Assert::AreEqual(int(queryTree.getResultTree().at(2).getParentType()), 4);

			input = "while w1; prog_line n; stmt s; Select w1 such that Follows(n, s) pattern w1(\"x\", _)";
			ParserForPQL parser1(input);
			queryTree = parser1.getQueryTree();

			Assert::AreEqual(queryTree.getResultTree().at(0).getParentStringVal(), string("w1"));
			Assert::AreEqual(int(queryTree.getResultTree().at(0).getParentType()), 4);
		}

		TEST_METHOD(TestSuchThatTree)
		{
			PKB *pkb = new PKB();
			pkb->setVarName("x");
			pkb->setVarName("y");
			pkb->setProcNameInProcTable("Main");
			pkb->setProcNameInProcTable("y");
			pkb->setProcNameInProcTable("x");

			string input = "while w1, w2, w3; assign a, s; prog_line n; Select <w1,w2, w3> with n = 10 pattern a(\"x\", _) such that Follows(n, s) ";
			ParserForPQL parser2(input);
			QueryTree queryTree = parser2.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Follows"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("n"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("s"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 0);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);


			input = "assign a; stmt s; prog_line n; Select a such that Follows*(s, n) pattern a(\"x\", _)";
			ParserForPQL parser3(input);
			queryTree = parser3.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Follows*"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("s"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("n"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);

			input = "assign a; Select a such that Uses(a, \"x\") pattern a(\"y\", _)";
			ParserForPQL parser4(input);
			queryTree = parser4.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Uses"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("a"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 0);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 6);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);


			input = "while w1; assign a; Select w1 such that Modifies(w1, \"x\") pattern a(\"x\", _)";
			ParserForPQL parser5(input);
			queryTree = parser5.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Modifies"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 4);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 6);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);

			input = "while w1; stmt s; prog_line n; assign a; Select <w1, a> such that Parent(w1, _) pattern a(\"x\", _)";
			ParserForPQL parser6(input);
			queryTree = parser6.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Parent"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 4);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 3);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);

			input = "while w1; assign a; prog_line n; Select w1 such that Parent*(w1, \"2\") pattern a(\"x\", _)";
			ParserForPQL parser7(input);
			queryTree = parser7.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Parent*"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 4);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), 2);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("2"));

			input = "while w1; assign a; prog_line n; Select w1 such that Uses(\"Main\", \"y\")";
			ParserForPQL parser8(input);
			queryTree = parser8.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Uses"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("Main"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 2);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 6);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("y"));

			input = "while w1; assign a; prog_line n; Select w1 such that Next*(20, n) and Next*(40, 620)";
			ParserForPQL parser10(input);
			queryTree = parser10.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Next*"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("20"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), 20);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("n"));

			input = "while w; Select w such that Parent(w, 7)";
			ParserForPQL parser9(input);
			queryTree = parser9.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getParentStringVal(), string("Parent"));
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 4);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIntValue(), -1);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIntValue(), 7);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("7"));
		}
	
 		TEST_METHOD(TestPatternTree) {
			PKB *pkb = new PKB();
			pkb->setVarName("x");
			pkb->setVarName("y");
			pkb->setConstant(2);

			string input = "while w1, w2, w3; assign a; prog_line n; Select <w1,w2, w3> pattern a(\"x\", _) such that Follows(n, a)";
			ParserForPQL parser4(input);
			QueryTree queryTree = parser4.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 0);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = "while w1, w2, w3; assign a, n; Select <w1,w2, w3> pattern a(\"x\", 2) such that Follows(n, a)";
			ParserForPQL parser9(input);
			queryTree = parser9.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 0);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("2"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 7);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = "while w1, w2, w3; assign a, n; variable v; Select <w1,w2, w3> pattern a(v, 2) such that Follows(n, a)";
			ParserForPQL parser10(input);
			queryTree = parser10.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 0);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("2"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 7);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = "while w1, w2, w3; assign a, n; variable v; Select <w1,w2, w3> pattern a(v, _x+2_) such that Follows(n, a)";
			ParserForPQL parser11(input);
			queryTree = parser11.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 0);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("x+2"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 9);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), true);


			input = "while w1, w2, w3; assign a, n; variable v; Select <w1,w2, w3> pattern w1(v, _) such that Follows(n, a)";
			ParserForPQL parser12(input);
			queryTree = parser12.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("w1"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 4);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = "while w1, w2, w3; assign a, n; variable v; Select <w1,w2, w3> pattern w2(\"x\", _) such that Follows(n, a)";
			ParserForPQL parser13(input);
			queryTree = parser13.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("w2"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 4);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = "while w1, w2, w3; assign a, n; if if; Select <w1,w2, w3> pattern if(\"x\", _, _) such that Follows(n, a)";
			ParserForPQL parser14(input);
			queryTree = parser14.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("if"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 5);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			input = " assign a, n; if if; Select a pattern a(_, \"_x*9_\")";
			ParserForPQL parser15(input);
			queryTree = parser15.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 0);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("x*9"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 9);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), true);


		}
		TEST_METHOD(TestAnd) {
			PKB *pkb = new PKB();
			string input = "while w1, w2, w3; assign a, n, a1, n1; variable v; Select w1 such that Parent*(w1, w2) and Follows(n, a) and Parent(w2, a) pattern a1(_, _) and n1(v, _)";
			ParserForPQL parser4(input);
			QueryTree queryTree = parser4.getQueryTree();
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 4);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("w2"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 4);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getLeftCStringValue(), string("n"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getLeftCType()), 0);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getRightCStringValue(), string("a"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getRightCType()), 0);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getSuchThatTree().at(2).getLeftCStringValue(), string("w2"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(2).getLeftCType()), 4);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(2).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(2).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getSuchThatTree().at(2).getRightCStringValue(), string("a"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(2).getRightCType()), 0);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(2).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(2).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("a1"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 0);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getPatternTree().at(1).getParentStringVal(), string("n1"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getParentType()), 0);
			Assert::AreEqual(queryTree.getPatternTree().at(1).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(1).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getRightCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(1).getRightCIsExpression(), false);
		}
		TEST_METHOD(TestMultipleClauses) {
			PKB *pkb = new PKB();
			pkb->setVarName("x");
			pkb->setVarName("y");

			//test multiple such that
			string input = "while w1, w2, w3; assign a, n, a1; Select w1 such that Parent*(w1, w2) such that Follows(n, a) such that Uses(a1, \"x\")";
			ParserForPQL parser4(input);
			QueryTree queryTree = parser4.getQueryTree();

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 4);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("w2"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 4);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getLeftCStringValue(), string("n"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getLeftCType()), 0);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getRightCStringValue(), string("a"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getRightCType()), 0);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getSuchThatTree().at(2).getLeftCStringValue(), string("a1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(2).getLeftCType()), 0);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(2).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(2).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getSuchThatTree().at(2).getRightCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(2).getRightCType()), 6);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(2).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(2).getRightCIsExpression(), false);

			//test multiple pattern
			input = "while w1, w2, w3; assign a; variable v; if if; Select w1 pattern if(\"x\", _, _) pattern a(v, \"2\")";
			ParserForPQL parser5(input);
			queryTree = parser5.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("if"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 5);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getPatternTree().at(1).getParentStringVal(), string("a"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getParentType()), 0);
			Assert::AreEqual(queryTree.getPatternTree().at(1).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(1).getRightCStringValue(), string("2"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getRightCType()), 7);
			Assert::AreEqual(int(queryTree.getPatternTree().at(1).getRightCIntValue()), 2);
			Assert::AreEqual(queryTree.getPatternTree().at(1).getRightCIsExpression(), false);

			//test multiple pattern and suchThat
			input = "while w1, w2, w3; assign a, n; variable v; if if; Select w1 pattern if(\"x\", _, _)  pattern a(v, \"2\") such that Parent*(w1, w2) and Follows(n, a)" ;
			ParserForPQL parser6(input);
			queryTree = parser6.getQueryTree();

			Assert::AreEqual(queryTree.getPatternTree().at(0).getParentStringVal(), string("if"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getParentType()), 5);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCStringValue(), string("_"));
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCType()), 3);
			Assert::AreEqual(int(queryTree.getPatternTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getPatternTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCStringValue(), string("w1"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCType()), 4);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCStringValue(), string("w2"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCType()), 4);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getLeftCStringValue(), string("n"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getLeftCType()), 0);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getRightCStringValue(), string("a"));
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getRightCType()), 0);
			Assert::AreEqual(int(queryTree.getSuchThatTree().at(1).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getSuchThatTree().at(1).getRightCIsExpression(), false);
		}

		TEST_METHOD(TestGetVarName) {
			PKB *pkb = new PKB();
			pkb->setVarName("x");
			pkb->setVarName("y");

			Assert::AreEqual(pkb->getVarIndex("x"), 0);
		}

		TEST_METHOD(TestWithTree) {
			PKB *pkb = new PKB();
			pkb->setVarName("x");
			pkb->setVarName("y");
			pkb->setProcNameInProcTable("Second");
			pkb->setProcNameInProcTable("Main");

			string input = "stmt s; constant c; while w1; Select w1 with s.stmt# = c.value";
			ParserForPQL parser1(input);
			QueryTree queryTree = parser1.getQueryTree();
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCStringValue(), string("s"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCStringValue(), string("c"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCType()), 7);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsExpression(), false);

			input = "procedure p; variable v; while w1; prog_line n; Select w1 with v.varName = \"x\" with p.procName= \"Second\"";
			ParserForPQL parser3(input);
			queryTree = parser3.getQueryTree();
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCStringValue(), string("x"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCType()), 6);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCStringValue(), string("p"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCType()), 2);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCStringValue(), string("Second"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCType()), 2);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCIsExpression(), false);

			input = "stmt s; constant c; while w1; prog_line n; Select w1 with s.stmt# = c.value and n = 10";
			ParserForPQL parser2(input);
			queryTree = parser2.getQueryTree();
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCStringValue(), string("s"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCStringValue(), string("c"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCType()), 7);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCStringValue(), string("n"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCStringValue(), string("10"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCIntValue()), 10);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCIsExpression(), false);

			input = "assign a; stmt s; prog_line n; procedure p; variable v; Select a such that Follows*(s, n) pattern a(\"x\", _) with n = 10 and p.procName = \"Main\"";

			ParserForPQL parser4(input);
			queryTree = parser4.getQueryTree();
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCStringValue(), string("n"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCStringValue(), string("10"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCIntValue()), 10);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCStringValue(), string("p"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCType()), 2);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCStringValue(), string("Main"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCType()), 2);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCIsExpression(), false);

			input = "assign a; stmt s; prog_line n; procedure p; variable v, v1; Select a with n = 10 and v.varName = v1.varName";

			ParserForPQL parser5(input);
			queryTree = parser5.getQueryTree();
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCStringValue(), string("n"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCStringValue(), string("10"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCIntValue()), 10);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsExpression(), false);

			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCStringValue(), string("v"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCType()), 6);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCStringValue(), string("v1"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCType()), 6);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCIsExpression(), false);

			input = "if ifs; assign a1, a2; variable v1, v2; stmt s; Select ifs with a2.stmt# = a1.stmt# and a1.stmt# = s.stmt#";
			ParserForPQL parser6(input);
			queryTree = parser6.getQueryTree();
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCStringValue(), string("a2"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCType()), 0);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsStmt(), true);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCStringValue(), string("a1"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCType()), 0);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsStmt(), true);

			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCStringValue(), string("a1"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCType()), 0);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(1).getLeftCIsStmt(), true);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCStringValue(), string("s"));
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCType()), 1);
			Assert::AreEqual(int(queryTree.getWithTree().at(1).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(1).getRightCIsStmt(), true);

			input = "if ifs; call c1, c2; variable v1, v2; stmt s; Select ifs with c1.stmt# = c2.stmt#";
			ParserForPQL parser7(input);
			queryTree = parser7.getQueryTree();
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCStringValue(), string("c1"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCType()), 8);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getLeftCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getLeftCIsStmt(), true);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCStringValue(), string("c2"));
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCType()), 8);
			Assert::AreEqual(int(queryTree.getWithTree().at(0).getRightCIntValue()), -1);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsExpression(), false);
			Assert::AreEqual(queryTree.getWithTree().at(0).getRightCIsStmt(), true);
		}
		};
}
