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

Details::TYPE Details::getType()
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
	TYPE type = convertToType(typeInString);
	this->type = type;
}

Details::TYPE Details::convertToType(string typeInString)
{
	if (typeInString.compare("_") == 0) {
		return TYPE::UNDERSCORE;
	}
	else if (typeInString.compare("variable") == 0) {
		return TYPE::VARIABLE;
	}
	else if (typeInString.compare("assign") == 0) {
		return TYPE::ASSIGN;
	}
	else if (typeInString.compare("procedure") == 0) {
		return TYPE::PROCEDURE;
	}
	else if (typeInString.compare("call") == 0) {
		return TYPE::CALLS;
	}
	else if (typeInString.compare("equation") == 0) {
		return TYPE::EQUATION;
	}
	else if (typeInString.compare("constant") == 0) {
		return TYPE::CONSTANT;
	}
	else if (typeInString.compare("while") == 0) {
		return TYPE::WHILE;
	}
	else if (typeInString.compare("if") == 0) {
		return TYPE::IF;
	}
	else if (typeInString.compare("stmt") == 0 || typeInString.compare("prog_line") == 0) {
		return TYPE::STATEMENT;
	}
}
