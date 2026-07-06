#include "InputString.h"

void InputString::SetString() {
	/*std::cout << "알파벳 대문자 문자열 입력 : ";
	std::getline(std::cin, str);*/
	strLen = str.size();
	strType.clear();
	for (int i = 0; i < strLen; i++) {
		if (static_cast<int>(str[i]) == 32)		// 공백 = 26
			strType.push_back(26);
		else
			strType.push_back(static_cast<int>(str[i]) - 'A');	//  ASCII 'A' = 65
	}
}
void InputString::PrintString() const {
	cout << str << endl;
	for (int i = 0; i < strLen; i++) {
		cout << strType[i] << ", ";
	}
	cout << endl;
}
int InputString::GetStrLen() const {
	return strLen;
}
int InputString::GetStrType(int idx) const {
	return strType[idx];
}