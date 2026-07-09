#include "AlphabetString.h"

void AlphabetString::SetString(const std::string str_) {
	str = str_;
	strType.clear();
	for (int i = 0; i < str.size(); i++) {
		if (static_cast<int>(str[i]) == 32)		// 공백 = 26
			strType.push_back(26);
		else
			strType.push_back(static_cast<int>(str[i]) - 'A');	//  ASCII 'A' = 65
	}
}
void AlphabetString::PrintString() const {
	cout << str << endl;
	for (int i = 0; i < str.size(); i++) {
		cout << strType[i] << ", ";
	}
	cout << endl;
}
int AlphabetString::GetStrLen() const {
	return str.size();
}
int AlphabetString::GetStrType(int idx) const {
	if (idx < 0 || idx >= static_cast<int>(strType.size()))
		return 26; // 공백 처리

	return strType[idx];
}
