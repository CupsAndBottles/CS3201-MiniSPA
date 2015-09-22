#include "Clauses.h"

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



Details Clauses::getLeftChild()
{
	return leftChild;
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

void Clauses::setLeftCIsExpression(bool isExpression)
{
	leftChild.setIsExpression(isExpression);
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

Details Clauses::getRightChild()
{
	return rightChild;
}

Details Clauses::getParent()
{
	return parent;
}

