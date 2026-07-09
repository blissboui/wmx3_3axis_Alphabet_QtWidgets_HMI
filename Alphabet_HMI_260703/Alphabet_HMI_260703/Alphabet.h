#pragma once
#include "Common.h"
#include <fstream>
#include <sstream>
#include <array>

class AlphabetShared;

class Alphabet {
public:
	void SetType(int n);
	void SetCoordNum(int n);
	void SetCoord(const double coord_[3], const int* gAxis);
	void SetTargetPos(const int idx, const int rowOf, const int colOf);
	Motion::LinearIntplCommand& GetLinearIntplCommand(int i);
	int GetCoordNum() const;
	void ShowCoord(int i);
	const std::vector<Motion::LinearIntplCommand>& GetCoord() const;
	Alphabet& operator=(const AlphabetShared& rhs);
	static void SetAlphabetData(vector<Alphabet>& alphabet_AZ, std::string fileName, const int* gAxis);
	static bool FileOpen(std::vector<std::vector<double>>& data, const std::string fileName);
private:
	int type;		// 알파벳 종류
	int coordNum;	// xyz 좌표 개수
	std::vector<Motion::LinearIntplCommand> coord;		// 알파벳 좌표 배열 (직선보간)
};
