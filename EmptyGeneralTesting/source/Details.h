#pragma once
#include <string>
#include <map>
#include "Enum.h"

using namespace std;
class Details
{
public:
	Details();
	~Details();
			//  0             1         2          3           4
//	enum TYPE { ASSIGN, STATEMENT, PROCEDURE, UNDERSCORE, WHILE, IF, VARIABLE, CONSTANT, CALLS, EQUATION };

	int intValue;
	string stringValue = "";
	Enum::TYPE type;
	bool isExpression;
	bool getIsExpression();
	string getStringValue();
	int getIntValue();
	Enum::TYPE getType();

	void setIsExpression(bool isExpression);
	void setStringValue(string stringVal);
	void setIntValue(int intVal);
	void setType(string type);

	Enum::TYPE convertToType(string typeInString);
};

