#include "WmxHandler.h"
#include <QString>


WmxHandler::WmxHandler(QObject* parent) :
	QObject(parent),
	engineCtrl(std::make_shared<Engine>(&wmx)),
	inputStr(std::make_shared<AlphabetString>()),
	axisCtrl(std::make_shared<Axis>(&wmx)),
	sharedMemCtrl(std::make_shared<SharedMemory>()),
	alphabet_AZ(27)
{
	// 디바이스 생성
	CreateDevice();
	// 통신 시작
	StartCommunication();
	// 엑셀 csv 파일로부터 A~Z 알파벳 좌표 데이터 읽은 후 전달한 변수에 저장
	Alphabet::SetAlphabetData(alphabet_AZ, (std::string)"C:\\Users\\boboy\\source\\repos\\Data\\AlphabetData.csv", gAxis);
	// 공유 메모리 생성
	sharedMemCtrl->Create();
}

void WmxHandler::CreateDevice()
{
	errMes = engineCtrl->CreateDevice();
	if (!errMes.empty())
	{
		emit AlarmOccurred(QString::fromStdString(errMes));
	}
}

void WmxHandler::StartCommunication()
{
	errMes = engineCtrl->StartCommunication();
	if (!errMes.empty())
	{
		emit AlarmOccurred(QString::fromStdString(errMes));
	}
}

void WmxHandler::ServoOnOff(const int state)
{
	errMes = axisCtrl->ServoOnOff(state);
	if (!errMes.empty())
	{
		emit AlarmOccurred(QString::fromStdString(errMes));
	}
}

void WmxHandler::GetServoState()
{
	bool allServo;
	errMes = (string)axisCtrl->GetStatus(&allServo);
	if (!errMes.empty())
	{
		emit AlarmOccurred(QString::fromStdString(errMes));
	}
	if (allServo != lastServoOn)
	{
		// 3축 전체 서보 On 일때만 램프 On
		// 서보 상태 변화 있을때만 램프 상태 변경
		emit LampStateChanged(allServo ? true : false, 0);
		lastServoOn = allServo;
	}
}

void WmxHandler::GetAxisPosition(int axis_, double* position)
{
	axisCtrl->GetAxisPosition(axis_, position);
}

void WmxHandler::GetAxisVelocity(int axis_, double* velocity)
{
	axisCtrl->GetAxisVelocity(axis_, velocity);
}

void WmxHandler::SetOffset()
{
	int row = 0, col = 0;
	currAlphabet.clear();	// 이전 문자열 초기화

	for (int idx = 0; idx < inputStr->GetStrLen(); idx++)
	{
		currAlphabet.push_back(alphabet_AZ[inputStr->GetStrType(idx)]);	// 알파벳 타입에 맞게 깊은 복사

		for (int i = 0; i < currAlphabet[idx].GetCoordNum(); i++)
		{
			currAlphabet[idx].SetTargetPos(i, ROW_OFFSET_LEN * row, COL_OFFSET_LEN * col);	// 좌표에 옵셋 적용
		}
		col++;
		if (col >= MAX_COL) { 

			col = 0;
			row++;
		}
		if (row >= MAX_ROW) {
			emit AlarmOccurred("작성 가능한 글자수를 초과했습니다.");
			return;
		}
	}
}

int WmxHandler::GetAxisNum(int i) const
{
	return gAxis[i];
}

bool WmxHandler::SetAlphabetString(const std::string str_)
{
	if (str_.size() < MAX_COL * MAX_ROW)
	{
		inputStr->SetString(str_);
		SetOffset();	// 문자 간격, 줄바꿈 설정
		return true;
	}
	else
	{
		emit AlarmOccurred("작성 가능한 글자수를 초과했습니다.");
		return false;
	}
}

void WmxHandler::SendCurrAlphabet()
{
	if (!sharedMemCtrl->SendCoord(currAlphabet))
	{
		emit AlarmOccurred("공유 메모리 좌표 전송 실패");
		return;
	}
	sharedMemCtrl->SendInput();
}

void WmxHandler::SendStartMove()
{
	sharedMemCtrl->SendMove();
}

void WmxHandler::SendStopMove()
{
	sharedMemCtrl->SendStop();
}

void WmxHandler::UpdateCompletedCount()
{
	emit ProgressChanged(sharedMemCtrl->GetCompletedCount());
}

void WmxHandler::SetCompletedCount(int i)
{
	sharedMemCtrl->SetCompletedCount(i);
}