#pragma once
#include "Common.h"
#include "Error.h"

class Axis : public Error
{
public:
	Axis(WMX3Api* wmx);
	std::string ServoOnOff(const int select);	// 서보 on/off
	std::string GetAxisPosition(int axis_, double* position);		// 현재 위치
	std::string GetAxisVelocity(int axis_, double* velocity);		// 현재 속도
private:
	WMX3Api* wmxlib;
	std::shared_ptr<CoreMotion> cMotion;
	std::shared_ptr<AxisSelection> axisSel;
	std::shared_ptr<AxisControl> axisCtrl;

};