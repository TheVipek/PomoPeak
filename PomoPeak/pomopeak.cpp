#include "pomopeak.h"
#include "./ui_pomopeak.h"

PomoPeak::PomoPeak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PomoPeak)
    , timer(new QTimer)
    , flowHandler(PomoSettings::ShortBreakAfterSessions, PomoSettings::LongBreakAfterShortBreaks)
    , taskManager()
    , startButtonClickEffect(new QSoundEffect(this))
    , endBreakEffect(new QSoundEffect(this))

{
    ui->setupUi(this);
    isRunning = false;
    durationLeft = PomoSettings::SessionDuration;
    globalCounter = 0;
\

    startButtonClickEffect->setSource(QUrl("qrc:/sounds/sounds/clickButtonSound.wav"));
    startButtonClickEffect->setVolume(.5f);
    endBreakEffect->setSource(QUrl("qrc:/sounds/sounds/breakEndNotification.wav"));
    endBreakEffect->setVolume(.5f);
    endBreakEffect->setLoopCount(10);

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
    if(isRunning == true)
    {
        if(endBreakEffect->isPlaying())
        {
            endBreakEffect->stop();
        }
        startButtonClickEffect->play();
        timer.start();

    }
    else
    {
        timer.stop();
    }

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
        if(flowHandler.GetCurrentSequence() == FlowSequence::LongBreak || flowHandler.GetCurrentSequence() == FlowSequence::ShortBreak)
        {
            endBreakEffect->play();
        }

        flowHandler.Next();
        UpdateTimerDuration(flowHandler.GetCurrentSequence());
    }

    UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
}
void PomoPeak::OnTryAddTask()
{
    taskQT* newTaskUI = new taskQT(this);
    newTaskUI -> setMaximumHeight(175);
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
        durationLeft = PomoSettings::SessionDuration;
            break;
        case FlowSequence::ShortBreak:
            durationLeft = PomoSettings::ShortBreakDuration;
            break;
        case FlowSequence::LongBreak:
            durationLeft = PomoSettings::LongBreakDuration;
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
