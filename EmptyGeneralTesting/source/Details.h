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
	bool isExpression = false;
	bool getIsExpression();
	string getStringValue();
	int getIntValue();
	Enum::TYPE getType();
	bool isStmt = false;
	bool getIsStmt();

	void setIsExpression(bool isExpression);
	void setIsStmt(string isstmt);
	void setStringValue(string stringVal);
	void setIntValue(int intVal);
	void setType(string type);

	Enum::TYPE convertToType(string typeInString);
};

