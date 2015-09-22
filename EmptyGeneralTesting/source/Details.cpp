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
		return Type::underscore;
	}
	else if (typeInString.compare("variable") == 0) {
		return Type::variable;
	}
	else if (typeInString.compare("assign") == 0) {
		return Type::assignment;
	}
	else if (typeInString.compare("procedure") == 0) {
		return Type::procedure;
	}
	else if (typeInString.compare("call") == 0) {
		return Type::calls;
	}
	else if (typeInString.compare("equation") == 0) {
		return Type::equation;
	}
	else if (typeInString.compare("constant") == 0) {
		return Type::constant;
	}
	else if (typeInString.compare("while") == 0) {
		return Type::While;
	}
	else if (typeInString.compare("if") == 0) {
		return Type::If;
	}
}
