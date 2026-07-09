#include "AlphabetMotion.h"

AlphabetMotion::AlphabetMotion() : engineCtrl(&wmx)
{
	// 디바이스 생성
	CreateDevice();
	// 통신 시작
	StartCommunication();
	// 공유 메모리 연결
	if (!sharedMemCtrl.Open())
	{
		std::cout << "공유 메모리 연결 실패" << std::endl;
		exit(1);
	}
	std::cout << "HMI 연결 성공" << std::endl;
	axis.axisCount = 3;
	for (int i = 0; i < axis.axisCount; i++) {
		// X:0 Y:1 Z:2
		axis.axis[i] = i;
	}
}

void AlphabetMotion::CreateDevice()
{
	std::string errMes = engineCtrl.CreateDevice();
	if (!errMes.empty())
	{
		std::cout << errMes << std::endl;
		exit(1);
	}
}

void AlphabetMotion::StartCommunication()
{
	std::string errMes = engineCtrl.StartCommunication();
	if (!errMes.empty())
	{
		std::cout << errMes << std::endl;
		exit(1);
	}
}

bool AlphabetMotion::SetAlphabet()
{
	const AlphabetShared* src = sharedMemCtrl.GetAlphabetCoord();
	int count = sharedMemCtrl.GetArraySize();

	if (src == nullptr || count <= 0)
		return false;

	alphabet.clear();
	alphabet.resize(count);   // Alphabet 객체 count개 생성

	for (int i = 0; i < count; i++)
	{
		alphabet[i] = src[i]; // AlphabetShared -> Alphabet 대입
	}

	sharedMemCtrl.SendNone();

	return !alphabet.empty();
}

int AlphabetMotion::GetCommand() const
{
	return sharedMemCtrl.GetCommand();
}

void AlphabetMotion::StartMotion()
{
	for (int idx = 0; idx < alphabet.size(); idx++)
	{
		for (int i = 0; i < alphabet[idx].GetCoordNum(); i++)
		{
			err = cMotion.motion->StartLinearIntplPos(&alphabet[idx].GetLinearIntplCommand(i));
			if (err != ErrorCode::None)
			{
				wmx.ErrorToString(err, errString, sizeof(errString));
				cout << errString << " Error: " << err << endl;
				return;
			}

			err = cMotion.motion->Wait(&axis);
			if (err != ErrorCode::None)
			{
				wmx.ErrorToString(err, errString, sizeof(errString));
				cout << errString << " Error: " << err << endl;
				return;
			}
		}
		sharedMemCtrl.SetCompletedCount(idx+1);
	}
	sharedMemCtrl.SendNone();	// None Command 전송
}

bool AlphabetMotion::WaitTrigger()
{
	return sharedMemCtrl.WaitTrigger();
}