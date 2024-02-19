#include "pomopeak.h"
#include "./ui_pomopeak.h"


PomoPeak::PomoPeak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PomoPeak)
    , timer(new QTimer)
    , settings(25,5,10)

{

    ui->setupUi(this);
    isRunning = false;
    durationLeft = settings.SessionDuration;
    UpdateTimer(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
    timer->setInterval(1000);
    connect(timer,  &QTimer::timeout, this, &PomoPeak::OnTimerTimeout);
    connect(ui->ChangeFlowBtn, &QPushButton::clicked, this, &PomoPeak::OnChangeState);
}

PomoPeak::~PomoPeak()
{
    delete ui;
    delete timer;
}

void PomoPeak::OnChangeState()
{
    isRunning = !isRunning;
    if(isRunning)
    {
        ui->ChangeFlowBtn->setText("Stop");
        timer->start();
    }
    else
    {
        ui->ChangeFlowBtn->setText("Start");
        timer->stop();
    }
}
void PomoPeak::OnTimerTimeout()
{

    durationLeft -= 1;
    qDebug() << durationLeft;
    UpdateTimer(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
}

void PomoPeak::SwitchFlow()
{
    isRunning = true;
}
void PomoPeak::UpdateCounter()
{

}
void PomoPeak::Skip()
{

}
void PomoPeak::UpdateTimer(QString value)
{
    qDebug() << value;
    ui->TimeLabel->setText(value);
}
void PomoPeak::ResetTimer()
{

}
