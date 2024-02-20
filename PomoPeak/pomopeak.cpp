#include "pomopeak.h"
#include "./ui_pomopeak.h"


PomoPeak::PomoPeak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PomoPeak)
    , timer(new QTimer)
    , settings(.05f,.1f,.15f)
    , flowHandler(2,1)
{
    ui->setupUi(this);
    isRunning = false;
    durationLeft = settings.SessionDuration;
    globalCounter = 0;

    UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
    AdjustButtonsVisibilityDependingOnCurrentState();

    timer->setInterval(1000);
    connect(timer,  &QTimer::timeout, this, &PomoPeak::OnTimerTimeout);
    connect(ui->ChangeFlowBtn, &QPushButton::clicked, this, &PomoPeak::OnChangeState);
    connect(ui->SkipBtn, &QPushButton::clicked,this, &PomoPeak::Skip);
}

PomoPeak::~PomoPeak()
{
    delete ui;
    delete timer;
}

void PomoPeak::OnChangeState()
{
    isRunning = !isRunning;
    isRunning == true ? timer ->start() : timer->stop();
    AdjustButtonsVisibilityDependingOnCurrentState();
}
void PomoPeak::OnTimerTimeout()
{
    durationLeft -= 1;
    if(durationLeft <= 0)
    {
        OnChangeState();
        if(flowHandler.GetCurrentSequence() == FlowSequence::Session)
        {
            UpdateCounter();
        }

        flowHandler.Next();
        UpdateTimerDuration(flowHandler.GetCurrentSequence());
    }

    UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
}
void PomoPeak::UpdateCounter()
{
    globalCounter++;
    ui->CounterLabel->setText(QString("%1").arg(globalCounter));
}
void PomoPeak::Skip()
{
    durationLeft = 0;
    OnTimerTimeout();
}
void PomoPeak::UpdateTimerLabel(QString value)
{
    ui->TimeLabel->setText(value);
}
void PomoPeak::UpdateTimerDuration(FlowSequence sequence)
{
    qDebug() << sequence;
    switch(sequence)
    {
        case FlowSequence::Session:
            durationLeft = settings.SessionDuration;
            break;
        case FlowSequence::ShortBreak:
            durationLeft = settings.ShortBreakDuration;
            break;
        case FlowSequence::LongBreak:
            durationLeft = settings.LongBreakDuration;
            break;
        default:
            qDebug() << "something wrong with updatingTimerDuration";
            break;
    }
}
void PomoPeak::AdjustButtonsVisibilityDependingOnCurrentState()
{
    if(isRunning)
    {
        ui->ChangeFlowBtn->setText("Stop");
        ui->SkipBtn->setVisible(true);
    }
    else
    {
        ui->ChangeFlowBtn->setText("Start");
        ui->SkipBtn->setVisible(false);
    }
}
