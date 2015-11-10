#pragma once
#include <vector>
#include <string>
#include <regex>

using namespace std;
class ParserForSelect
{
public:
	ParserForSelect(vector<vector<string>> synonym, int index);
	~ParserForSelect();
	vector<string> selectSynonym;
	
	void parseSelect(vector<vector<string>> synonym, int pos);

	vector<string> split(const string &s, char delim);
	vector<string> &split(const string &s, char delim, vector<string> &elems);

	vector<string> removeUnwanted(vector<string> commaSubset);

	vector<string> getSelectSynonym();
};

