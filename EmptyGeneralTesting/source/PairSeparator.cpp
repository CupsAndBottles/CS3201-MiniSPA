#pragma once
#include "PairSeparator.h"

//Note: can be extended to separate list of pairs. 

PairSeparator::PairSeparator()
{
}

void PairSeparator::addPair(pair<int, int> pair)
{
	firstNum = pair.first;
	secondNum = pair.second;
}

PairSeparator::~PairSeparator()
{
}

int PairSeparator::getFirstNum()
{
	return firstNum;
	
}

int PairSeparator::getSecondNum()
{
	return secondNum;
}
