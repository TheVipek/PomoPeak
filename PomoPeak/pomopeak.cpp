#include "pomopeak.h"
#include "./ui_pomopeak.h"
#include "ui_taskQT.h"

PomoPeak::PomoPeak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PomoPeak)
    , timer(new QTimer)
    , settings(1,.5f,1.5f)
    , flowHandler(2,1)
    , taskManager()
{
    ui->setupUi(this);
    isRunning = false;
    durationLeft = settings.SessionDuration;
    globalCounter = 0;

    UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
    AdjustButtonsVisibilityDependingOnCurrentState();

    timer.setInterval(1000);

    connect(&timer,  &QTimer::timeout, this, &PomoPeak::OnTimerTimeout);
    connect(ui->ChangeFlowBtn, &QPushButton::clicked, this, &PomoPeak::OnChangeState);
    connect(ui->SkipBtn, &QPushButton::clicked, this, &PomoPeak::Skip);
    connect(ui->AddTaskBtn, &QPushButton::clicked, this, &PomoPeak::OnTryAddTask);

    ui->tasksContentV2->setAlignment(Qt::AlignTop);
}

PomoPeak::~PomoPeak()
{
    delete ui;
}

void PomoPeak::OnChangeState()
{
    isRunning = !isRunning;
    isRunning == true ? timer.start() : timer.stop();
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
            //Call to current task update
            if(currentActiveTask != nullptr)
            {
                currentActiveTask->ElapsedIncrease();
            }

            globalCounter++;
        }

        flowHandler.Next();
        UpdateTimerDuration(flowHandler.GetCurrentSequence());
    }

    UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
}
void PomoPeak::OnTryAddTask()
{
    taskQT* newTaskUI = new taskQT(this);
    newTaskUI -> setMaximumHeight(150);
    connect(newTaskUI, &taskQT::CreateRequest, this, &PomoPeak::AddTask);
    connect(newTaskUI, &taskQT::DeleteRequest, this, &PomoPeak::RemoveTask);
    connect(newTaskUI, &taskQT::OnEnableViewModeRequest, this, &PomoPeak::OnViewModeTaskChanged);
    connect(newTaskUI, &taskQT::OnSelectRequest, this, &PomoPeak::OnCurrentActiveTaskChanged);

    ui->tasksContentV2->addWidget(newTaskUI);

}
void PomoPeak::AddTask(std::shared_ptr<Task> task)
{
    taskManager.AddTask(task);
}
void PomoPeak::RemoveTask(std::shared_ptr<Task> task)
{
    taskManager.RemoveTask(task);
}
void PomoPeak::OnViewModeTaskChanged(taskQT* task)
{
    qDebug() << "requested task:" << task;
    qDebug() << currentInViewModeTask;
    if(currentInViewModeTask != nullptr && currentInViewModeTask != task)
    {
        currentInViewModeTask->DisableViewMode();
    }
    currentInViewModeTask = task;

}
void PomoPeak::OnCurrentActiveTaskChanged(taskQT* task)
{
    if(currentActiveTask != nullptr && currentActiveTask != task)
    {
        currentActiveTask->SwitchSelectState();
    }
    currentActiveTask = task;
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
