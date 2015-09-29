#include "Synonym.h"

Synonym::Synonym()
{
}

Synonym::Synonym(Enum::TYPE type, string syn, vector<int>& resultsToStore) {
	this->type = type;
	this->syn = syn;
	addResult(resultsToStore);
}

Synonym::~Synonym()
{
}

void Synonym::addResult(vector<int> &resultToBeStored) {
	vector<int> intersect;

	if (result.empty()) {
		for (int i = 0; i < resultToBeStored.size(); i++) {
			result.push_back(resultToBeStored[i]);
		}
	}
	else {
		for (int i = 0; i < resultToBeStored.size(); i++) {
			for (int j = 0; i < result.size(); j++) {
				if (resultToBeStored[i] == result[j]) {
					intersect.push_back(result[j]);
				}
			}
		}
	}

	// Might cause problems
	this->result = intersect;

}

void Synonym::setSyn(string syn) {
	this->syn = syn;
}

void Synonym::setType(Enum::TYPE type) {
	this->type = type;
}

vector<int> Synonym::getResult()
{
	return this->result;
}

string Synonym::getSyn()
{
	return this->syn;
}

Enum::TYPE Synonym::getType() {
	return this->type;
}
