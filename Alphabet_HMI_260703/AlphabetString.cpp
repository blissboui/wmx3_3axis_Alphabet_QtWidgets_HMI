#include "AlphabetString.h"

void AlphabetString::SetString(const std::string str_) {
	/*std::cout << "알파벳 대문자 문자열 입력 : ";
	std::getline(std::cin, str);*/
	str = str_;
	strLen = str.size();
	strType.clear();
	for (int i = 0; i < strLen; i++) {
		if (static_cast<int>(str[i]) == 32)		// 공백 = 26
			strType.push_back(26);
		else
			strType.push_back(static_cast<int>(str[i]) - 'A');	//  ASCII 'A' = 65
	}
}
void AlphabetString::PrintString() const {
	cout << str << endl;
	for (int i = 0; i < strLen; i++) {
		cout << strType[i] << ", ";
	}
	cout << endl;
}
int AlphabetString::GetStrLen() const {
	return strLen;
}
int AlphabetString::GetStrType(int idx) const {
	return strType[idx];
}
