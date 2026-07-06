#pragma once
#include "Common.h"
#include <fstream>
#include <sstream>
#include <array>


class Alphabet {
public:
	void SetType(int n);
	void SetCoordNum(int n);
	void SetCoord(const double coord_[3]);
	void SetTargetPos(const int idx, const int rowOf, const int colOf);
	Motion::LinearIntplCommand& GetLinearIntplCommand(int i);
	int GetCoordNum() const;
	static void SetAlphabetData(vector<Alphabet>& alphabet_AZ, std::string fileName);
	static bool FileOpen(std::vector<std::vector<double>>& data, const std::string fileName);
	void ShowCoord(int i);
	int GetAxisNum(int i) const;
private:
	int type;		// 알파벳 종류
	int coordNum;	// xyz 좌표 개수
	const int gAxis[3] = { X_AXIS, Y_AXIS, Z_AXIS };
	std::vector<Motion::LinearIntplCommand> coord;		// 알파벳 좌표 배열 (직선보간)
};
