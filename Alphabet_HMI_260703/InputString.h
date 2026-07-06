#pragma once
#include "Common.h"

class InputString {
public:
	void SetString();
	void PrintString() const;
	int GetStrLen() const;
	int GetStrType(int idx) const;

private:
	std::string str;
	int strLen;
	std::vector<int> strType;
};