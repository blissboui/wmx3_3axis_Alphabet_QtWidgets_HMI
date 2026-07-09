#pragma once
#include "MotionShared.h"
#include <Windows.h>
#include <vector>
class Alphabet;

class SharedMemory 
{
public:
	bool Create();
	bool Open();

	bool SendCoord(const std::vector<Alphabet>& alpha);

	void SendMove();
	void SendStop();
	void SendNone();
	void SendInput();

	bool WaitTrigger(DWORD timeoutMs = INFINITE);
	void Trigger();

	int GetArraySize() const;
	const AlphabetShared* GetAlphabetCoord() const;
	MotionShared* GetData() const;
	int GetCommand() const;
	int GetCompletedCount() const;
	void SetCompletedCount(int i);

	void Close();
	~SharedMemory();
private:
	HANDLE m_hMap = nullptr;
	HANDLE m_hEvent = nullptr;
	MotionShared* m_data = nullptr;
};

