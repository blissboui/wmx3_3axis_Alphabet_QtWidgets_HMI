#pragma once
#include "SharedMemory.h"
#include "Engine.h"
#include "WMX3Api.h"
#include "CoreMotionApi.h"
#include "Alphabet.h"
#include <iostream>
#include <vector>
#include <string>


class AlphabetMotion {
public:
	AlphabetMotion();
	void CreateDevice();
	void StartCommunication();
	bool SetAlphabet();
	int GetCommand() const;
	void StartMotion();
	bool WaitTrigger();
private:
	WMX3Api wmx;
	Engine engineCtrl;
	SharedMemory sharedMemCtrl;
	std::vector<Alphabet> alphabet;
	CoreMotion cMotion;
	AxisSelection axis;
	int err;
	char errString[256];
};