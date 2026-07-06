#pragma once

#include "Alphabet.h"
#include "InputString.h"
#include "Engine.h"
#include "Axis.h"
#include <memory>
#include <QObject>


class WmxHandler : public QObject
{
	Q_OBJECT
public:
	WmxHandler(QObject* parent = nullptr);
	void CreateDevice();		// 디바이스 생성
	void StartCommunication();	// 통신 시작
	void ServoOnOff(const int select);		// 서보 on/off
	void GetAxisPosition(int axis_, double* position);		// 현재 위치
	void GetAxisVelocity(int axis_, double* velocity);		// 현재 속도
	void SetOffset(const InputString& str_);
	int GetAxisNum(int i) const;
signals:
	void AlarmOccurred(const QString& message);
private:
	WMX3Api wmx;
	std::shared_ptr<Alphabet> alphabet;
	std::shared_ptr<Engine> engineCtrl;
	std::shared_ptr<InputString> str;
	std::shared_ptr<Axis> axisCtrl;
	std::vector<Alphabet> alphabet_AZ;
	std::string errMes;
};

