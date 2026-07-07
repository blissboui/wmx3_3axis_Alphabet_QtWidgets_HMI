#include "Axis.h"

Axis::Axis(WMX3Api* wmx) : wmxlib(wmx)
{
	cMotion = std::make_shared<CoreMotion>(wmxlib);
	axisCtrl = std::make_shared<AxisControl>(cMotion.get());
	axisSel = std::make_shared<AxisSelection>();

	axisSel->axisCount = 3; // XYZ 축 선택
	for (int i = 0; i < axisSel->axisCount; i++) {
		// X:0 Y:1 Z:2
		axisSel->axis[i] = i;
	}
}

std::string Axis::ServoOnOff(const int select) // XYZ servo on/off
{
	err = cMotion->axisControl->SetServoOn(axisSel.get(), select, 5000);
	if (err != ErrorCode::None)
	{
		wmxlib->ErrorToString(err, errString, sizeof(errString));
		return GetErrorMessage();
	}
	return "";
}

std::string Axis::GetAxisPosition(int axis_, double* position)
{
	err = axisCtrl->GetPosFeedback(axis_, position);
	if (err != ErrorCode::None)
	{
		wmxlib->ErrorToString(err, errString, sizeof(errString));
		return GetErrorMessage();
	}
	return "";
}

std::string Axis::GetAxisVelocity(int axis_, double* velocity)
{
	err = axisCtrl->GetVelFeedback(axis_, velocity);
	if (err != ErrorCode::None)
	{
		wmxlib->ErrorToString(err, errString, sizeof(errString));
		return GetErrorMessage();
	}
	return "";
}

std::string Axis::GetStatus(CoreMotionStatus* st)
{

	err = cMotion->GetStatus(st);
	if (err != ErrorCode::None)
	{
		wmxlib->ErrorToString(err, errString, sizeof(errString));
		return GetErrorMessage();
	}
	return "";
}
