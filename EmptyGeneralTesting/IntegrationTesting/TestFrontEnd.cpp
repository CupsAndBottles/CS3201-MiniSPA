#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		Parser parser;
		PKB *pkb;
		TEST_METHOD(Frontend_ReadFile)
		{
			/* Test case used in file:
			procedure Dream {
			beads = command + 10;
			while coffee {	
				if dream then{			
				stamps = beads+ coffee;						
				}else{					
				x = x * y + d + y * a; }
				h=s+i*u;}				
			beads = command + 10;}	

			procedure Hope {
			call Dream;
			if a then {			
				x = x * a; }
			else {
				x = x + a; }}

			procedure School{
			dreams=work+2;				
			while t {							
			x = d + 5 * y + z;
			t = t - 1; }				
			d = x + d;}	
			*/

			//file simpleTestFrontend.txt exists and input is read. Check if expected = actual result
			string lines = parser.openFile("simpleTestFrontend.txt");
			Assert::AreEqual(lines, string("1: procedureDream{2: beads=command+10;3: whilecoffee{4: ifdreamthen{5: stamps=beads+coffee;}6: else{7: x=x*y+d+y*a;}8: h=s+i*u;}9: beads=command+10;}10: procedureHope{11: callDream;12: ifathen{13: x=x*a;}14: else{15: x=x+a;}}16: procedureSchool{17: dreams=work+2;18: whilet{19: x=d+5*y+z;20: t=t-1;}21: d=x+d;}"));
		}

		TEST_METHOD(Frontend_getChildren) {
			parser.openFile("simpleTestFrontend.txt");
			pkb = parser.getPkb();
			vector<int> expectedResults = { 6,3 };
			vector<int> children = pkb->getChildren(2);

			Assert::AreEqual(expectedResults.size(), children.size());
			for (int i = 0; i < children.size(); i++) {
				Assert::AreEqual(expectedResults.at(i), children.at(i));
			}
			delete pkb;
		}

		TEST_METHOD(Frontend_getFollowedBy) {
			parser.openFile("simpleTestFrontend.txt");
			pkb = parser.getPkb();

			int expected = 7;
			int actual = pkb->getFollowedBy(2);
			Assert::AreEqual(expected,actual);
			
			expected = 6;
			actual = pkb->getFollowedBy(3);
			Assert::AreEqual(expected, actual);

			expected = 0;
			actual = pkb->getFollowedBy(9);
			Assert::AreEqual(expected, actual);

			expected = 13;
			actual = pkb->getFollowedBy(12);
			Assert::AreEqual(expected, actual);

			expected = 15;
			actual = pkb->getFollowedBy(14);
			Assert::AreEqual(expected, actual);


			delete pkb;
		}

		TEST_METHOD(Frontend_getFollows) {
			parser.openFile("simpleTestFrontend.txt");
			pkb = parser.getPkb();

			int expected = 1;
			int actual = pkb->getFollows(2);
			Assert::AreEqual(expected, actual);

			expected = 3;
			actual = pkb->getFollows(6);
			Assert::AreEqual(expected, actual);

			expected = 0;
			actual = pkb->getFollows(10);
			Assert::AreEqual(expected, actual);

			expected = 13;
			actual = pkb->getFollowedBy(12);
			Assert::AreEqual(expected, actual);

			expected = 14;
			actual = pkb->getFollows(15);
			Assert::AreEqual(expected, actual);


			delete pkb;
		}

		TEST_METHOD(Frontend_getExpression) {
			parser.openFile("simpleTestFrontend.txt");
			pkb = parser.getPkb();

			string expected = "x y * d + y a * +";
			string actual = pkb->getRightExpr(5);
			Assert::AreEqual(expected, actual);

			expected = "d 5 y * + z +";
			actual = pkb->getRightExpr(14);
			Assert::AreEqual(expected, actual);


			delete pkb;
		}

		TEST_METHOD(Frontend_getProcModified) {
			parser.openFile("simpleTestFrontend.txt");
			pkb = parser.getPkb();

			string expected = " stamps h beads x";
			string actual = "";
			vector<int> modifiedVar = pkb->getProcModified(1);
	
			while (!modifiedVar.empty()) {
				string var = pkb->getVarName(modifiedVar.back());
				actual += " " + var;
				modifiedVar.pop_back();
			}
			Assert::AreEqual(expected, actual);

			delete pkb;
		}

		TEST_METHOD(Frontend_getProcUsed) {
			parser.openFile("simpleTestFrontend.txt");
			pkb = parser.getPkb();

			string expected = " dream beads coffee d y s i u command a x a x a";
			string actual = "";
			vector<int> usedVar = pkb->getProcUsed(1);

			while (!usedVar.empty()) {
				string var = pkb->getVarName(usedVar.back());
				actual += " " + var;
				usedVar.pop_back();
			}
			Assert::AreEqual(expected, actual);

			delete pkb;
		}
	};


}