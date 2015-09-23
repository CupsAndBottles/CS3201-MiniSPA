#pragma once
#include "PairSeparator.h"

//Note: can be extended to separate list of pairs. 
PairSeparator::PairSeparator(pair<int, int> pair)
{
		firstNum = pair.first;
		secondNum = pair.second;	
}

PairSeparator::~PairSeparator()
{
}

int PairSeparator::getFirstNum()
{
	firstNum;
	return 0;
}

int PairSeparator::getSecondNum()
{
	secondNum;
	return 0;
}
