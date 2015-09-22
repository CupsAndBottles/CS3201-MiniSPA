#pragma once

#include <vector>
#include <string>

using namespace std;
class ParserForSuchThat
{
public:
	ParserForSuchThat(vector<vector<string> > type, vector<vector<string> > synonym, int index);
	~ParserForSuchThat();
	vector<string> suchThatSynonym;
	char toBeSplitWith[3] = { '(', ',', ')' };

	void parseSuchThat(vector<vector<string> > type, vector<vector<string> > synonym, int pos);
	string getToBeSplit(vector<string> suchThatSubset, int i);
	vector<string> split(const string &s, char delim);
	vector<string> &split(const string &s, char delim, vector<string> &elems);

	vector<string> getSuchThatSynonym();

	vector<string> removeUnwanted(vector<string> suchThatSyn);
};

