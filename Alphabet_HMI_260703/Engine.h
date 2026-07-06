#pragma once
#include "Common.h"
#include "Error.h"

class Engine : public Error {
public:
	Engine(WMX3Api* wmx);
	std::string CreateDevice();
	std::string StartCommunication();
	void EndEngine();
	~Engine();
private:
	WMX3Api* wmxlib;
	int err;
	char* errString;
};