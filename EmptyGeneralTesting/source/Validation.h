#pragma once
#include<vector>
#include<string>
#include<regex>

using namespace std;

class Validation
{
public:
	Validation();
	~Validation();

	void grammarValidation(vector<vector<string>> suchThatSynAndType);
	void patternValidation(vector<vector<string>> patternSynAndType);

	void withValidation(string declaredType, string userType);

	//In arrangement of parent/* 1st argument, parent/* 2nd arg, follows/*, modifies 1st arg, uses 1st arg, modifies/uses 2nd arg
	vector<regex> synonym = { std::regex("\\bprog_line\\b|\\bwhile\\b|\\bif\\b|\\bstmt\\b|\\b_\\b"), //1 parent/* (0)
		std::regex("\\bprog_line\\b|\\bassign\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bstmt\\b|\\b_\\b"), // 2 parent /* (1)
		std::regex("\\bprog_line\\b|\\bassign\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bstmt\\b|\\b_\\b"), // follows / */next/next* (2)
		std::regex("\\bprocedure\\b|\\bprog_line\\b|\\bassign\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bstmt\\b"), //1 modifies/ use (3)
		std::regex("\\bvariable\\b|\\b_\\b"), // modifies/use 2nd arg and pattern assign1/pattern 1st agrument for while and if and with varName (4)
		std::regex("\\bvariable\\b"), // (5) 
		std::regex("\\b_\\b"), // _(6)
		std::regex("\\bvariable\\b|\\b_\\b|\\bequation\\b|\\bconstant\\b"), //7 for assign arg2 (7)
		std::regex("\\bprocedure\\b|\\b_\\b"), //for calls (8)
		std::regex("\\bassign\\b|\\b_\\b|\\bprog_line\\b|\\bstmt\\b"),//for affect (9)
		std::regex("\\bassign\\b|\\bstmt\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b"), //for stmt# with(10)
		std::regex("\\bprocedure\\b|\\bcall\\b"), //for procName with (11)
		std::regex("\\bconstant\\b"), //for value with (12)
		std::regex("\\bconstant\\b|\\bstmt\\b|\\bassign\\b|\\bstmt\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b"), //for size 4 of with cases of constant (13)
		std::regex("\\bprocedure\\b|\\bvariable\\b|\\bcall\\b"), //for size 4 of with cases of proc and var (14)
		std::regex("\\bassign\\b|\\bstmt\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bconstant\\b"), //for size 4 of with cases of assign (15)
		std::regex("\\bassign\\b|\\bstmt\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bconstant\\b"), // for size 4 of stmt (16)
		std::regex("\\bassign\\b|\\bstmt\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bconstant\\b"), // for size 4 of while (17)
		std::regex("\\bassign\\b|\\bstmt\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bconstant\\b"), // for size 4 of if (18)
		std::regex("\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bconstant\\b|\\bstmt\\b|\\bprocedure\\b|\\bvariable\\b") // for size 4 of call (19)
	};
};

