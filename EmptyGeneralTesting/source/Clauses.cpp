#include "Clauses.h"
#include "Enum.h"

Clauses::Clauses()
{
}


Clauses::~Clauses()
{
}

Clauses & Clauses::return_created_Object()
{
	return Clauses();
}

void Clauses::setLeftChild(Details leftChild)
{
	this->leftChild = leftChild;
}

void Clauses::setRightChild(Details rightChild)
{
	this->rightChild = rightChild;
}

void Clauses::setParentStringVal(string stringVal)
{
	parent.setStringValue(stringVal);
}

void Clauses::setParentType(string type)
{
	parent.setType(type);
}

string Clauses::getParentStringVal()
{
	return parent.getStringValue();
}

Enum::TYPE Clauses::getParentType()
{
	return parent.getType();
}

void Clauses::setRightCIsExpression(bool isExpression)
{
	rightChild.setIsExpression(isExpression);
}

void Clauses::setRightCStringValue(string stringVal)
{
	rightChild.setStringValue(stringVal);
}

void Clauses::setRightCIntValue(int intVal)
{
	rightChild.setIntValue(intVal);
}

void Clauses::setRightCType(string type)
{
	rightChild.setType(type);
}

void Clauses::setRightCIsStmt(string isStmt)
{
	rightChild.setIsStmt(isStmt);
}

bool Clauses::getRightCIsExpression()
{
	return rightChild.getIsExpression();
}

bool Clauses::getRightCIsStmt()
{
	return rightChild.getIsStmt();
}

string Clauses::getRightCStringValue()
{
	return rightChild.getStringValue();
}

int Clauses::getRightCIntValue()
{
	return rightChild.getIntValue();
}

Enum::TYPE Clauses::getRightCType()
{
	return rightChild.getType();
}

void Clauses::setLeftCIsExpression(bool isExpression)
{
	leftChild.setIsExpression(isExpression);
}

void Clauses::setLeftCIsStmt(string isStmt)
{
	leftChild.setIsStmt(isStmt);
}

void Clauses::setLeftCStringValue(string stringVal)
{
	leftChild.setStringValue(stringVal);
}

void Clauses::setLeftCIntValue(int intVal)
{
	leftChild.setIntValue(intVal);
}

void Clauses::setLeftCType(string type)
{
	leftChild.setType(type);
}

bool Clauses::getLeftCIsExpression()
{
	return leftChild.getIsExpression();
}

bool Clauses::getLeftCIsStmt()
{
	return leftChild.getIsStmt();
}

string Clauses::getLeftCStringValue()
{
	return leftChild.getStringValue();
}

int Clauses::getLeftCIntValue()
{
	return leftChild.getIntValue();
}

Enum::TYPE Clauses::getLeftCType()
{
	return leftChild.getType();
}