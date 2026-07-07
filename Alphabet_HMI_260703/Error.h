#pragma once
#include <string>
#include <iostream>

class Error
{
public:
	int err;
	char errString[256];
	std::string errMessage;

	std::string GetErrorMessage()
	{
		return errMessage = std::string(errString) + " (Error: " + std::to_string(err) + ")";
	}
};
