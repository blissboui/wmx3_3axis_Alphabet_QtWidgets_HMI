#pragma once

#include "EventApi.h"
#include "WMX3Api.h"
#include "CoreMotionApi.h"
#include <iostream>
#include <vector>
#include <string>

#define MAX_COL 5
#define MAX_ROW 5
#define ROW_OFFSET_LEN 50
#define COL_OFFSET_LEN 50

enum AXIS
{
	X_AXIS = 1,
	Y_AXIS = 0,
	Z_AXIS = 2
};
enum PROFILE
{
	VELOCITY = 70000,	// (70mm/s)
	ACC = 100000,
	DEC = 100000, 
	ENC = 1000	// (1000cts == 1mm)
};

using namespace wmx3Api;
using namespace std;



// 엑셀 파일을 .csv로 저장







