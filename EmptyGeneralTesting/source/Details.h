#pragma once
#include <string>
#include <map>

using namespace std;
class Details
{
public:
	Details();
	~Details();
			//  0             1         2          3           4
	//enum Type { underscore, variable, assignment, procedure, calls, equation, statement, constant, While, If };
	enum Type { ASSIGN, STATEMENT, PROCEDURE, UNDERSCORE, WHILE, IF, VARIABLE, CONSTANT, CALLS, EQUATION };
	int intValue;
	string stringValue = "";
	Type type;
	bool isExpression;
	bool getIsExpression();
	string getStringValue();
	int getIntValue();
	Type getType();

	void setIsExpression(bool isExpression);
	void setStringValue(string stringVal);
	void setIntValue(int intVal);
	void setType(string type);

	Type convertToType(string typeInString);
};

