#include "HMI_main.h"

#include <iostream>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QCloseEvent>
#include <QMessageBox>

HMI_main::HMI_main(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	auto* alphabetValidator = new QRegularExpressionValidator(QRegularExpression("[A-Za-z]*"), this);
	
	ui.leAlphabetInput->setValidator(alphabetValidator);

	// 알파벳 입력 처리
	connect(ui.leAlphabetInput, &QLineEdit::returnPressed, this, &HMI_main::InputAlphabet);	// 엔터키 입력 시 InputAlphabet() 호출

	// Axis 상태
    connect(&monitorTimer, &QTimer::timeout, this, &HMI_main::UpdateAxisStatus);
	monitorTimer.start(100); // 100 ms

	// 알람
	connect(&wmxCtrl, &WmxHandler::AlarmOccurred, this, &HMI_main::ShowAlarm);

	// Progress
	connect(&wmxCtrl, &WmxHandler::ProgressChanged, this, &HMI_main::UpdateProgressBar);
	ui.pbProgress->setRange(0, 100);
	ui.pbProgress->setValue(0);

	// Lamp
	connect(&wmxCtrl, &WmxHandler::LampStateChanged, this, &HMI_main::UpdateHmiLamp);

}

HMI_main::~HMI_main()
{
	monitorTimer.stop();
}

void HMI_main::on_btnServoOn_clicked()   // Servo On
{
	wmxCtrl.ServoOnOff(1);
}

void HMI_main::on_btnServoOff_clicked()  // Servo Off
{
	wmxCtrl.ServoOnOff(0);
}	

void HMI_main::on_btnStart_clicked()   // Start
{
	UpdateHmiLamp(1, 1);
	if (totalCount <= 0) return;
}

void HMI_main::on_btnStop_clicked()   // Stop
{
	UpdateHmiLamp(0, 1);
}

void HMI_main::UpdateAxisStatus()   // Axis 상태 읽기
{
	QLabel* lblPos[3] = {
		ui.lblXPos,
		ui.lblYPos,
		ui.lblZPos,
	};
	QLabel* lblVel[3] = {
		ui.lblXVel,
		ui.lblYVel,
		ui.lblZVel,
	};

	for (int i = 0; i < 3; i++)	// 현재 위치 갱신
	{
		double pos = 0.0;
		wmxCtrl.GetAxisPosition(wmxCtrl.GetAxisNum(i), &pos);
		lblPos[i]->setText(QString::number(pos, 'f', 2));
	}
	for (int i = 0; i < 3; i++)	// 현재 속도 갱신
	{
		double vel = 0.0;
		wmxCtrl.GetAxisVelocity(wmxCtrl.GetAxisNum(i), &vel);
		lblVel[i]->setText(QString::number(vel, 'f', 2));
	}
}

void HMI_main::InputAlphabet()
{
	alphabetInput = ui.leAlphabetInput->text().trimmed().toUpper();
	qDebug() << "Input Alphabet: " << alphabetInput;
	
	if (wmxCtrl.SetAlphabetString(alphabetInput.toStdString()))	// QString -> std::string 변환 후 WmxHandler에 전달
	{
		// 입력한 문자열 띄우기
		ui.lblInputResult->setText(WrapEnglishText(alphabetInput, ui.lblInputResult));
		totalCount = alphabetInput.length();
		ui.pbProgress->setValue(0);
	}
	ui.leAlphabetInput->clear();	// 입력창 초기화
}

void HMI_main::closeEvent(QCloseEvent* event)
{
	monitorTimer.stop();
	QMainWindow::closeEvent(event);
}

void HMI_main::ShowAlarm(const QString& message)
{
	QMessageBox::critical(this, "ALARM", message);
}

QString HMI_main::WrapEnglishText(const QString& text, QLabel* label)
{
	// 알파벳 자동 줄바꿈 처리
	QFontMetrics fontMetrics(label->font());

	QString result;
	QString line;

	const int maxWidth = label->width();

	for (const QChar& ch : text)
	{
		if (fontMetrics.horizontalAdvance(line + ch) > maxWidth)
		{
			result += line + '\n';
			line.clear();
		}

		line += ch;
	}

	result += line;

	return result;
}

void HMI_main::UpdateProgressBar(int completedCount)
{
	if (totalCount <= 0)
	{
		ui.pbProgress->setValue(0);
		return;
	}

	int progress = completedCount * 100 / totalCount;

	ui.pbProgress->setValue(progress);
}

void HMI_main::UpdateHmiLamp(bool isOn, int select)
{
	// 0: Servo Lamp, 1: Move Lamp
	QString strOn = {
		"background-color: lime;"
		"border: none;"
		"border-radius: 5px;"
	};
	QString strOff = {
		"background-color: gray;"
		"border: none;"
		"border-radius: 5px;"
	};
	
	switch (select)
	{
	case 0:	// Servo Lamp
		if (isOn)
		{
			ui.lblServoLamp->setStyleSheet(strOn);
		}
		else
		{
			ui.lblServoLamp->setStyleSheet(strOff);
		}
		break;
	case 1: // Move Lamp
		if (isOn)
		{
			ui.lblMoveLamp->setStyleSheet(strOn);
		}
		else
		{
			ui.lblMoveLamp->setStyleSheet(strOff);
		}
		break;
	}

}
