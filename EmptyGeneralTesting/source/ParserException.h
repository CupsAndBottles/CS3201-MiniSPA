#pragma once
#include <exception>
#include <string>
using namespace std;

class ParserException : public exception
{
	string msg = "invalid input \n";
	virtual const char* what() const throw()
	{
		return msg.c_str();
	}
public:
	ParserException(string message) {
		msg = message + "\n";
	}
};


