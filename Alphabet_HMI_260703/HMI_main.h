#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QString>
#include "ui_HMI_main.h"
#include "WmxHandler.h"

class HMI_main : public QMainWindow
{
    Q_OBJECT

public:
    HMI_main(QWidget *parent = nullptr);
    ~HMI_main();
private slots:
    void on_btnServoOn_clicked();
    void on_btnServoOff_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
	void UpdateAxisStatus();
	void InputAlphabet();
    void closeEvent(QCloseEvent* event) override;
    void ShowAlarm(const QString& message);
    QString WrapEnglishText(const QString& text, QLabel* label);
    void UpdateProgressBar(int completedCount);
    void UpdateHmiLamp(bool isOn, int select);
private:
    Ui::HMI_mainClass ui;
    QTimer monitorTimer;
    QString alphabetInput;
    WmxHandler wmxCtrl;
    int totalCount = 0;
};

