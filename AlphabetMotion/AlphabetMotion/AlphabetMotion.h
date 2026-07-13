#pragma once
#include "SharedMemory.h"
#include "Engine.h"
#include "WMX3Api.h"
#include "CoreMotionApi.h"
#include "Alphabet.h"
#include <iostream>
#include <vector>
#include <string>
#include <thread>

class AlphabetMotion {
public:
	AlphabetMotion();
	~AlphabetMotion();
	void CreateDevice();
	void StartCommunication();
	bool SetAlphabet();
	int GetCommand() const;
	void StartMotion();
	void StopMotion();
	void StopMotionLoop();
	bool WaitTrigger();
	
private:
	WMX3Api wmx;
	Engine engineCtrl;
	SharedMemory sharedMemCtrl;
	std::vector<Alphabet> alphabet;
	CoreMotion cMotion;
	AxisSelection axis;
	Motion::LinearIntplCommand homePos;
	int err;
	char errString[256];
	std::atomic<bool> threadRun{ false };
	std::atomic<bool> stopFlag{ false };
	std::thread stopThread;
};