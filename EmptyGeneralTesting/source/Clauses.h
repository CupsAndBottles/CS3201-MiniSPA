#pragma once
#include "Details.h"
#include <string>

class Clauses
{
public:
	Clauses();
	~Clauses();
	Clauses & return_created_Object();
	Details leftChild;
	Details rightChild;
	Details parent;
	//string stringVal;

	Details getLeftChild();
	Details getRightChild();
	Details getParent();
	//string getStringVal();

	void setLeftChild(Details leftChild);
	void setRightChild(Details rightChild);

	void setParentStringVal(string stringVal);
	void setParentType(string type);
	string getParentStringVal();
	Details::Type getParentType();

	void setRightCIsExpression(bool isExpression);
	void setRightCStringValue(string stringVal);
	void setRightCIntValue(int intVal);
	void setRightCType(string type);

	bool getRightCIsExpression();
	string getRightCStringValue();
	int getRightCIntValue();
	Details::Type getRightCType();

	void setLeftCIsExpression(bool isExpression);
	void setLeftCStringValue(string stringVal);
	void setLeftCIntValue(int intVal);
	void setLeftCType(string type);

	bool getLeftCIsExpression();
	string getLeftCStringValue();
	int getLeftCIntValue();
	Details::Type getLeftCType();
	
};

