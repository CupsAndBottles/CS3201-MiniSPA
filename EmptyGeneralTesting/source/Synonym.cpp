#include "Synonym.h"

Synonym::Synonym()
{
}

Synonym::Synonym(TYPE type, string syn) {
	this->type = type;
	this->syn = syn;
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

void Synonym::setType(TYPE type) {
	this->type = type;
}

