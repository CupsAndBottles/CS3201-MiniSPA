#pragma once
#include<vector>
#include<string>
#include<regex>

using namespace std;

class Validation
{
public:
	Validation(vector<vector<string>> suchThatSynAndType);
	~Validation();

	void grammarValidation(vector<vector<string>> suchThatSynAndType);

	//In arrangement of parent/* 1st argument, parent/* 2nd arg, follows/*, modifies 1st arg, uses 1st arg, modifies/uses 2nd arg
	vector<regex> synonym = { std::regex("\\bprog_line\\b|\\bwhile\\b|\\bif\\b|\\bstmt\\b|\\b_\\b"), //1 parent/* (0)
		std::regex("\\bprog_line\\b|\\bassign\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bstmt\\b|\\b_\\b"), // 2 parent /* (1)
		std::regex("\\bprog_line\\b|\\bassign\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bstmt\\b|\\b_\\b"), // follows / * (2)
		std::regex("\\bprocedure\\b|\\bprog_line\\b|\\bassign\\b|\\bwhile\\b|\\bif\\b|\\bcall\\b|\\bstmt\\b"), //1 modifies/ use (3)
		std::regex("\\bvariable\\b|\\b_\\b") }; // modifies/use 2nd arg (4)
};

