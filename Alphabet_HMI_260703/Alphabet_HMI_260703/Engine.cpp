#include "Engine.h"

Engine::Engine(WMX3Api* wmx) : wmxlib(wmx) {}

std::string Engine::CreateDevice() {	// 디바이스 생성
	err = wmxlib->CreateDevice(_T("C:\\Program Files\\SoftServo\\WMX3"), DeviceType::DeviceTypeNormal, 5000, INFINITE);
	if (err != ErrorCode::None)
	{
		wmxlib->ErrorToString(err, errString, sizeof(errString));
		printf("(%s)", errString);
		return GetErrorMessage();
	}
	return "";
	err = wmxlib->SetDeviceName("bobo");
	if (err != ErrorCode::None)
	{
		wmxlib->ErrorToString(err, errString, sizeof(errString));
		printf("(%s)", errString);
		return GetErrorMessage();
	}
	return "";
}

std::string Engine::StartCommunication() {		// 통신 시작
	err = wmxlib->StartCommunication(5000);	 // 통신 시작될때까지 5초간 대기
	if (err != ErrorCode::None)
	{
		wmxlib->ErrorToString(err, errString, sizeof(errString));
		printf("(%s)", errString);
		return GetErrorMessage();
	}
	return "";
}

void Engine::EndEngine() {	// 통신종료, 디바이스 삭제
	wmxlib->StopCommunication();	// 통신 종료
	wmxlib->CloseDevice();	// 디바이스 삭제
}
Engine::~Engine() {
	EndEngine();
}

