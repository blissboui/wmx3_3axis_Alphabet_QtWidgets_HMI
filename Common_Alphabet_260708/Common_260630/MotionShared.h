#pragma once
#include "CoreMotionApi.h"

enum Command
{
	CMD_NONE = 0,
	CMD_MOVE = 1,
	CMD_STOP = 2,
	CMD_INPUT = 3
};

class AlphabetShared
{
public:
	
	int coordCount;	// ÁÂÇ¥ °³¼ö 
	wmx3Api::Motion::LinearIntplCommand coord[100];
};

struct MotionShared
{
	int command;
	AlphabetShared alphabet[100];
	int alphabetCount = 0;
	int completedCount = 0;
};