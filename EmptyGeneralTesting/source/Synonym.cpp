#include "Synonym.h"

Synonym::Synonym()
{
}

Synonym::Synonym(TYPE type, string syn, vector<int>& resultsToStore) {
	this->type = type;
	this->syn = syn;
	for (int i = 0; i < resultsToStore.size(); i++) {
		addResult(resultsToStore[i]);
	}
}

Synonym::~Synonym()
{
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

set<int> Synonym::getResult()
{
	return this->result;
}

string Synonym::getSyn()
{
	return this->syn;
}

TYPE Synonym::getType() {
	return this->type;
}
