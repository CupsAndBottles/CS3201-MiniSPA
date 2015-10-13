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

	void setLeftChild(Details leftChild);
	void setRightChild(Details rightChild);

	void setParentStringVal(string stringVal);
	void setParentType(string type);
	string getParentStringVal();
	Enum::TYPE getParentType();

	void setRightCIsExpression(bool isExpression);
	void setRightCStringValue(string stringVal);
	void setRightCIntValue(int intVal);
	void setRightCType(string type);

	void setRightCIsStmt(string isStmt);

	bool getRightCIsExpression();
	bool getRightCIsStmt();
	string getRightCStringValue();
	int getRightCIntValue();
	Enum::TYPE getRightCType();

	void setLeftCIsExpression(bool isExpression);
	void setLeftCIsStmt(string isStmt);
	void setLeftCStringValue(string stringVal);
	void setLeftCIntValue(int intVal);
	void setLeftCType(string type);

	bool getLeftCIsExpression();
	bool getLeftCIsStmt();
	string getLeftCStringValue();
	int getLeftCIntValue();
	Enum::TYPE getLeftCType();
	
};

