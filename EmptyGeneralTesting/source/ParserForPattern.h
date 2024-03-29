#pragma once
#include <vector>
#include <string>
using namespace std;

class ParserForPattern
{
public:
	~ParserForPattern();
	ParserForPattern(vector<vector<string>> type, vector<vector<string>> synonym, int index);

	void parsePattern(vector<vector<string>> type, vector<vector<string>> synonym, int pos);

	vector<string> removeUnwanted(vector<string> PatternSynonym);

	string getToBeSplit(vector<string> PatternSubset, int i);

	vector<string> split(const string & s, char delim);

	vector<string>& split(const string & s, char delim, vector<string>& elems);

	vector<string> getPatternSynonym();

	char toBeSplitWith[2] = { ',', ')' };
	vector<string> patternSynonym;

};

