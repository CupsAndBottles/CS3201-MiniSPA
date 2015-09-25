#include "Details.h"

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

Details::Type Details::getType()
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
	Type type = convertToType(typeInString);
	this->type = type;
}

Details::Type Details::convertToType(string typeInString)
{
	if (typeInString.compare("_") == 0) {
		return Type::UNDERSCORE;
	}
	else if (typeInString.compare("variable") == 0) {
		return Type::VARIABLE;
	}
	else if (typeInString.compare("assign") == 0) {
		return Type::ASSIGN;
	}
	else if (typeInString.compare("procedure") == 0) {
		return Type::PROCEDURE;
	}
	else if (typeInString.compare("call") == 0) {
		return Type::CALLS;
	}
	else if (typeInString.compare("equation") == 0) {
		return Type::EQUATION;
	}
	else if (typeInString.compare("constant") == 0) {
		return Type::CONSTANT;
	}
	else if (typeInString.compare("while") == 0) {
		return Type::WHILE;
	}
	else if (typeInString.compare("if") == 0) {
		return Type::IF;
	}
	else if (typeInString.compare("stmt") == 0 || typeInString.compare("prog_line") == 0) {
		return Type::STATEMENT;
	}
}
