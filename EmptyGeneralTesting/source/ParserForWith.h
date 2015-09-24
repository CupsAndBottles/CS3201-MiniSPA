#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;
class ParserForWith
{
public:
	ParserForWith(vector<vector<string> > type, vector<vector<string> > synonym, int index);
	~ParserForWith();
	vector<string> withSynonym;

	void parseWith(vector<vector<string> > type, vector<vector<string> > synonym, int pos);
	vector<string> split(const string &s, char delim);
	vector<string> &split(const string &s, char delim, vector<string> &elems);

	vector<string> getWithSynonym();
	vector<string> removeUnwanted(vector<string> withSynonym);
};

