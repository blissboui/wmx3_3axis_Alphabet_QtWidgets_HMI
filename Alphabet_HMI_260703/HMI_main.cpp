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

	connect(ui.leAlphabetInput, &QLineEdit::returnPressed, this, &HMI_main::InputAlphabet);	// 엔터키 입력 시 InputAlphabet() 호출


    connect(&monitorTimer, &QTimer::timeout, this, &HMI_main::UpdateAxisStatus);

	monitorTimer.start(100); // Update every 100 ms

	connect(&wmxCtrl, &WmxHandler::AlarmOccurred, this, &HMI_main::ShowAlarm);
}

HMI_main::~HMI_main()
{
	monitorTimer.stop();
}

void HMI_main::on_btnServoOn_clicked()   // Servo On
{
	wmxCtrl.ServoOnOff(1);  // Servo On
}

void HMI_main::on_btnServoOff_clicked()  // Servo Off
{
	wmxCtrl.ServoOnOff(0);  // Servo Off
}	

void HMI_main::on_btnStart_clicked()   // Start
{

}

void HMI_main::on_btnStop_clicked()   // Stop
{

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
		ui.lblInputResult->setText(WrapEnglishText(alphabetInput, ui.lblInputResult));  // 확인용 Label
	}
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