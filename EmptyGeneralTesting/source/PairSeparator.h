#pragma once
#include <list>
using namespace std;

class PairSeparator{
public:
	PairSeparator(pair<int, int> pair);
	~PairSeparator();
	int getFirstNum();
	int getSecondNum();

	int firstNum;
	int secondNum;
};