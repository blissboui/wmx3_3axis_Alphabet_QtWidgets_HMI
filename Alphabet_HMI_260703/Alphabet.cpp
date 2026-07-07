#include "Alphabet.h"

void Alphabet::SetType(int n) {
	type = n;
}
void Alphabet::SetCoordNum(int n) {
	coordNum = n;
}
void Alphabet::SetCoord(const double coord_[3], const int* gAxis) {
	Motion::LinearIntplCommand cmd;
	cmd.axisCount = 3;
	for (int i = 0; i < 3; i++) {
		cmd.axis[i] = gAxis[i];
		cmd.profile.acc = ACC;
		cmd.profile.dec = DEC;
		cmd.profile.velocity = VELOCITY;
		cmd.target[i] = coord_[i] * ENC;
	}

	coord.push_back(cmd);
}
int Alphabet::GetCoordNum() const {
	return coordNum;
}
void Alphabet::SetAlphabetData(vector<Alphabet>& alphabet_AZ, std::string fileName, const int* gAxis) {
	std::vector<std::vector<double>> alphabetData;
	if (!FileOpen(alphabetData, fileName)) {
		//cout << "파일 열기 실패" << endl;
		return;
	}
	int count = 0;
	for (int i = 0; i < alphabet_AZ.size(); i++) {
		alphabet_AZ[i].SetType(i);
		alphabet_AZ[i].SetCoordNum(static_cast<int>(alphabetData[i][6]));
		for (int k = 0; k < alphabet_AZ[i].GetCoordNum(); k++) {
			double coord_[3] = {	// x, y, z 좌표
				alphabetData[count][0],
				alphabetData[count][1],
				alphabetData[count][2]
			};
			alphabet_AZ[i].SetCoord(coord_, gAxis);
			count++;
		}
	}
}
bool Alphabet::FileOpen(std::vector<std::vector<double>>& data, const std::string fileName) {
	std::ifstream file(fileName);

	if (!file.is_open()) {
		return false;
	}
	std::string line;

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;
		std::vector<double> row;

		while (std::getline(ss, cell, ',')) {
			if (cell.empty()) row.push_back(0.0);

			else row.push_back(std::stod(cell));
		}

		data.push_back(row);
	}

	return true;
}
void Alphabet::SetTargetPos(const int idx, const int rowOf, const int colOf) {
	coord[idx].target[0] += (colOf * ENC);
	coord[idx].target[1] -= (rowOf * ENC);

}
Motion::LinearIntplCommand& Alphabet::GetLinearIntplCommand(int i)
{
	return coord[i];
}

void Alphabet::ShowCoord(int i) {
	/*cout << "[ " << coord[i].target[0] << ", " << coord[i].target[1] << ", " << coord[i].target[2] << " ]" << endl;*/
}