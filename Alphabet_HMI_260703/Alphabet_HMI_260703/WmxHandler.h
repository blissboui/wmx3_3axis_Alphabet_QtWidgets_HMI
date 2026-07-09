#pragma once

#include "Alphabet.h"
#include "AlphabetString.h"
#include "Engine.h"
#include "Axis.h"
#include <memory>
#include <QObject>
#include "SharedMemory.h"

class WmxHandler : public QObject
{
	Q_OBJECT
public:
	WmxHandler(QObject* parent = nullptr);
	void CreateDevice();		// 디바이스 생성
	void StartCommunication();	// 통신 시작
	void ServoOnOff(const int state);		// 서보 on/off
	void GetServoState();
	void GetAxisPosition(int axis_, double* position);		// 현재 위치
	void GetAxisVelocity(int axis_, double* velocity);		// 현재 속도
	void SetOffset();
	int GetAxisNum(int i) const;
	bool SetAlphabetString(const std::string str_);
	void SendCurrAlphabet();
	void SendStartMove();
	void SendStopMove();
	void UpdateCompletedCount();
	void SetCompletedCount(int i);
signals:
	void AlarmOccurred(const QString& message);
	void ProgressChanged(int completedCount);
	void LampStateChanged(bool isOn, int select);
private:
	WMX3Api wmx;
	std::shared_ptr<Engine> engineCtrl;
	std::shared_ptr<AlphabetString> inputStr;
	std::shared_ptr<Axis> axisCtrl;
	std::shared_ptr<SharedMemory> sharedMemCtrl;
	std::vector<Alphabet> alphabet_AZ;
	std::vector<Alphabet> currAlphabet;
	std::string errMes;
	const int gAxis[3] = { X_AXIS, Y_AXIS, Z_AXIS };
	bool lastServoOn = false;
};

