#include "Details.h"
#include "Enum.h"

Details::Details()
{
}


Details::~Details()
{
}

bool Details::getIsExpression()
{
	return isExpression;
	return false;
}

string Details::getStringValue()
{
	return stringValue;
}

int Details::getIntValue()
{
	return intValue;
}

Enum::TYPE Details::getType()
{
	return type;
}

void Details::setIsExpression(bool isExpression)
{
	this->isExpression = isExpression;
}

void Details::setStringValue(string stringVal)
{
	stringValue = stringVal;
}

void Details::setIntValue(int intVal)
{
	intValue = intVal;
}

void Details::setType(string typeInString)
{
	Enum::TYPE type = convertToType(typeInString);
	this->type = type;
}

Enum::TYPE Details::convertToType(string typeInString)
{
	if (typeInString.compare("_") == 0) {
		return Enum::TYPE::UNDERSCORE;
	}
	else if (typeInString.compare("variable") == 0) {
		return Enum::TYPE::VARIABLE;
	}
	else if (typeInString.compare("assign") == 0) {
		return Enum::TYPE::ASSIGN;
	}
	else if (typeInString.compare("procedure") == 0) {
		return Enum::TYPE::PROCEDURE;
	}
	else if (typeInString.compare("call") == 0) {
		return Enum::TYPE::CALLS;
	}
	else if (typeInString.compare("equation") == 0) {
		return Enum::TYPE::EQUATION;
	}
	else if (typeInString.compare("constant") == 0) {
		return Enum::TYPE::CONSTANT;
	}
	else if (typeInString.compare("while") == 0) {
		return Enum::TYPE::WHILE;
	}
	else if (typeInString.compare("if") == 0) {
		return Enum::TYPE::IF;
	}
	else if (typeInString.compare("stmt") == 0 || typeInString.compare("prog_line") == 0) {
		return Enum::TYPE::STATEMENT;
	}
}
