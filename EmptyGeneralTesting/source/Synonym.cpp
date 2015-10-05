#include "Synonym.h"

Synonym::Synonym()
{
}

Synonym::Synonym(Enum::TYPE type, string syn, vector<int> resultsToStore) {
	this->type = type;
	this->syn = syn;
	addResult(resultsToStore);
}

Synonym::~Synonym()
{
}

void Synonym::addResult(vector<int> resultToBeStored) {
	vector<int> intersect;

	if (this->result.empty()) {
		for (int i = 0; i < resultToBeStored.size(); i++) {
			this->result.push_back(resultToBeStored[i]);
		}
	}
	else {
		for (int i = 0; i < resultToBeStored.size(); i++) {
			for (int j = 0; j < this->result.size(); j++) {
				if (resultToBeStored[i] == this->result[j]) {
					intersect.push_back(this->result[j]);
				}
			}
		}
		this->result.clear();
		for (size_t i = 0; i < intersect.size(); i++) {
			this->result.push_back(intersect[i]);
		}
	}



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
