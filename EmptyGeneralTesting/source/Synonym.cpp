#include "Synonym.h"

Synonym::Synonym()
{
}

Synonym::~Synonym()
{
}

void Synonym::addResult(vector<Enum::TYPE> type, vector<string> syn, vector<vector<int>> resultsToStore) {
	addType(type);
	addSyn(syn);
	storeResult(resultsToStore);
}

void Synonym::addType(vector<Enum::TYPE> type) {
	this->type = type;
}

void Synonym::addSyn(vector<string> syn) {
	this->syn = syn;
}

void Synonym::storeResult(vector<vector<int>> resultsToStore) {
	this->result = resultsToStore;
}

vector<Enum::TYPE> Synonym::getType() {
	return this->type;
}

vector<string> Synonym::getSyn() {
	return this->syn;
}

vector<vector<int>> Synonym::getResult() {
	return this->result;
}

bool Synonym::operator<(const Synonym & s2) const
{
	if (this->resultSize() < s2.resultSize()) {
		return true;
	}

	return false;
}

int Synonym::resultSize() {
	return this->result.size();
}