#include "AlphabetMotion.h"
#include "Common.h"
#include <chrono>

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

	// 쓰레드 생성
	threadRun.store(true);
	stopThread = std::thread(&AlphabetMotion::StopMotionLoop, this);

	const int gAxis[3] = { X_AXIS, Y_AXIS, Z_AXIS };
	axis.axisCount = 3;
	for (int i = 0; i < axis.axisCount; i++) {
		// X:0 Y:1 Z:2
		axis.axis[i] = gAxis[i];
	}

}

AlphabetMotion::~AlphabetMotion()
{
	// 쓰레드 종료
	threadRun.store(false);

	if (stopThread.joinable())
		stopThread.join();
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

	for (int i = 0; i < 3; i++)
	{
		homePos.axis[i] = alphabet[0].GetCoord()[0].axis[i];
		homePos.target[i] = 0;
	}
	homePos.axisCount = 3;
	homePos.target[2] = alphabet[0].GetCoord()[0].target[2];
	homePos.profile.velocity = alphabet[0].GetCoord()[0].profile.velocity;
	homePos.profile.acc = alphabet[0].GetCoord()[0].profile.acc;
	homePos.profile.dec = alphabet[0].GetCoord()[0].profile.dec;

	return true;
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

			if (stopFlag.exchange(false))
			{
				sharedMemCtrl.SendNone();
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
	err = cMotion.motion->StartLinearIntplPos(&homePos);	// 작성 종료 후 0위치로 이동
	if (err != ErrorCode::None)
	{
		wmx.ErrorToString(err, errString, sizeof(errString));
		cout << errString << " Error: " << err << endl;
		return;
	}
	sharedMemCtrl.SendNone();	// None Command 전송
}

void AlphabetMotion::StopMotion()
{
	err = cMotion.motion->Stop(&axis);
	if (err != ErrorCode::None)
	{
		wmx.ErrorToString(err, errString, sizeof(errString));
		cout << errString << " Error: " << err << endl;
		return;
	}
	stopFlag.store(true);
}

void AlphabetMotion::StopMotionLoop()
{
	while (threadRun.load())
	{
		if (sharedMemCtrl.GetCommand() == CMD_STOP)
		{
			StopMotion();
		}
		std::this_thread::sleep_for(std::chrono::microseconds(5));
	}
}

bool AlphabetMotion::WaitTrigger()
{
	return sharedMemCtrl.WaitTrigger();
}