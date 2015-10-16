#include "Synonym.h"

using namespace std;

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
	if (this->getSize() < s2.getSize()) {
		return true;
	}

	return false;
}

int Synonym::resultSize() {
	return this->result.size();
}

void Synonym::printSyn() {
	for (size_t i = 0; i < syn.size(); i++) {
		cout << syn[i] << " ";
	}
	cout << endl; 

	for (size_t i = 0; i < type.size(); i++) {
		cout << type[i] << " ";
	}
	cout << endl;

	for (size_t i = 0; i < result.size(); i++) {
		for (size_t j = 0; j < result[i].size(); j++) {
			cout << result[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}