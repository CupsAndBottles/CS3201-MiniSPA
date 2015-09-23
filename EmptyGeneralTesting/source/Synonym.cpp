#include "Synonym.h"

Synonym::Synonym()
{
}


Synonym::~Synonym()
{
}

string Synonym::getSyn()
{
	return this->syn;
}

set<int> Synonym::getResult()
{
	return this->result;
}

void Synonym::addResult(int index)
{
	result.insert(index);
}

void Synonym::setSyn(string syn) {
	this->syn = syn;
}