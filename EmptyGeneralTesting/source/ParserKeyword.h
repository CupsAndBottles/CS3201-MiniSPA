#pragma once

#include <vector>
#include <string>
#include <regex>

using namespace std;
class ParserKeyword
{
public:
	ParserKeyword(vector<vector<string> > type, vector<vector<string> > synonym);
	~ParserKeyword();
	vector<string> keywordSynonym;
	vector<string> keywordType;

	void parseKeyword(vector<vector<string> > type, vector<vector<string> > synonym);

	vector<string> split(const string &s, char delim);
	vector<string> &split(const string &s, char delim, vector<string> &elems);

	vector<string> removeUnwanted(vector<string> commaSubset);
	void addType(vector<string> temp, vector<vector<string>> type, int i);

	vector<string> getKeywordSynonym();
	vector<string> getKeywordType();
};


