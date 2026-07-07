#pragma once
#include "Common.h"

class AlphabetString {
public:
	void SetString(const std::string str_);
	void PrintString() const;
	int GetStrLen() const;
	int GetStrType(int idx) const;

private:
	std::string str;
	int strLen;
	std::vector<int> strType;
};