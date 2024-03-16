#include "pomopeak.h"
#include "./ui_pomopeak.h"

#include <sstream>
#include "settingsdto.h"
#include "databasetables.h"
#include <QDir>
PomoPeak::PomoPeak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PomoPeak)
    , timer(new QTimer)
    , taskManager()
{
    sqliteHandler = new SqliteHandler(QDir::currentPath() + "/data/database/applicationData.sqlite");

    std::ostringstream ss;
    QString query = QString("SELECT * FROM settings WHERE %1 UserID = 'Default' LIMIT 1").arg(DatabaseTables::SETTINGS);
    auto DTO = sqliteHandler->GetData<SettingsDTO>(query);
    if(!DTO.empty())
    {
        qDebug() << "Creating settings from DTO";
        settings = new Settings(DTO.front());
    }
    else
    {
        qDebug() << "Creatin default settings";
        settings = new Settings();
    }

    flowHandler = new FlowHandler(*settings);

    ui->setupUi(this);
    ui->widgetsLayout->setAlignment(Qt::AlignCenter);

    pomopeakSettings = new pomopeaksettings(*settings, *sqliteHandler , this);
    connect(pomopeakSettings, &pomopeaksettings::OnClose, this, &PomoPeak::OnHideSettings);
    pomopeakSettings->hide();

    ui->widgetsLayout->addWidget(pomopeakSettings);

    isRunning = false;
    durationLeft = settings->SessionDuration;
    baseDuration = settings->SessionDuration;
    globalCounter = 0;

    quickActionShortcut = new QShortcut(settings->QuickActionShortcut, this);
    connect(quickActionShortcut, &QShortcut::activated, this, &PomoPeak::TriggerQuickAction);

    startButtonClickEffect = new QSoundEffect(this);
    startButtonClickEffect->setSource(QUrl::fromLocalFile(settings->CurrentSessionAlarm.fileName()));
    startButtonClickEffect->setVolume(settings->SessionAlarmVolume);

    endBreakEffect = new QSoundEffect(this);
    endBreakEffect->setSource(QUrl::fromLocalFile(settings->CurrentBreakAlarm.fileName()));
    endBreakEffect->setLoopCount(settings->BreakAlarmRepetitions);

    UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
    AdjustButtonsVisibilityDependingOnCurrentState();

    timer.setInterval(1000);

    connect(ui->settingsBtn, &QPushButton::clicked, this, &PomoPeak::OnOpenSettings);
    connect(&timer, &QTimer::timeout, this, &PomoPeak::OnTimerTimeout);
    connect(ui->ChangeFlowBtn, &QPushButton::clicked, this, &PomoPeak::OnChangeState);
    connect(ui->SkipBtn, &QPushButton::clicked, this, &PomoPeak::Skip);
    connect(ui->AddTaskBtn, &QPushButton::clicked, this, &PomoPeak::OnTryAddTask);

    ui->tasksContentV2->setAlignment(Qt::AlignTop);
}

PomoPeak::~PomoPeak()
{
    delete ui;

    delete flowHandler;

    delete startButtonClickEffect;

    delete endBreakEffect;

    delete settings;
    delete sqliteHandler;
    //https://doc.qt.io/qt-6/objecttrees.html

    //no need to do it?
       // delete pomopeakSettings;
    // for (auto taskPtr : avaliableTasks) {
    //     delete taskPtr;
    // }
    //avaliableTasks.clear();

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
        if(startButtonClickEffect->isPlaying())
        {
            startButtonClickEffect->stop();
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
    qDebug() << "timeout?";
    durationLeft -= 1;
    if(durationLeft <= 0)
    {
        OnChangeState();


        if(flowHandler->GetCurrentSequence() == FlowSequence::Session)
        {
            //Call to current task update
            if(currentActiveTaskUI != nullptr)
            {
                currentActiveTaskUI->ElapsedIncrease();
            }
            globalCounter++;

        }
        if(flowHandler->GetCurrentSequence() == FlowSequence::LongBreak || flowHandler->GetCurrentSequence() == FlowSequence::ShortBreak)
        {
            endBreakEffect->play();
        }

        flowHandler->Next();
        UpdateTimerDuration(flowHandler->GetCurrentSequence());
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
void PomoPeak::OnViewModeTaskChanged(taskQT* taskUI)
{
    if(currentInViewModeTaskUI != nullptr && currentInViewModeTaskUI != taskUI)
    {
        currentInViewModeTaskUI->DisableViewMode();
    }
    currentInViewModeTaskUI = taskUI;

}
void PomoPeak::OnCurrentActiveTaskChanged(taskQT* taskUI)
{
    if(currentActiveTaskUI != nullptr && currentActiveTaskUI != taskUI)
    {
        currentActiveTaskUI->SwitchSelectState();
    }
    currentActiveTaskUI = taskUI;
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
            baseDuration = settings->SessionDuration;
            break;
        case FlowSequence::ShortBreak:
            baseDuration = settings->ShortBreakDuration;
            break;
        case FlowSequence::LongBreak:
            baseDuration = settings->LongBreakDuration;
            break;
        default:
            qDebug() << "something wrong with updatingTimerDuration";
            break;
    }

    durationLeft = baseDuration;
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
void PomoPeak::OnOpenSettings()
{
    if(!pomopeakSettings->IsOpened)
    {
        ui->widget->hide();
        pomopeakSettings->show();
    }
}

void PomoPeak::ForceTimerUpdate(int& durationLeft, int& baseDuration, const int targetDuration)
{
    if(baseDuration != targetDuration)
    {
        if(durationLeft > targetDuration)
        {
            durationLeft = targetDuration;
        }
        else if(baseDuration < targetDuration)
        {
            durationLeft = targetDuration - (baseDuration - durationLeft);
        }
        baseDuration = targetDuration;
        UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60, 2, 10, QChar('0')).arg((durationLeft % 60), 2, 10, QChar('0')));
    }
}
void PomoPeak::TriggerQuickAction()
{
    if(!pomopeakSettings->IsOpened)
    {
        OnChangeState();
    }
}
void PomoPeak::OnHideSettings(const bool alarmStartChanged,const bool alarmBreakChanged)
{
    if(pomopeakSettings->IsOpened)
    {
        ui->widget->show();
        pomopeakSettings->hide();
    }

    switch(flowHandler->GetCurrentSequence())
    {
        case FlowSequence::Session:
            ForceTimerUpdate(durationLeft, baseDuration, settings->SessionDuration);
            break;
        case FlowSequence::ShortBreak:
            ForceTimerUpdate(durationLeft, baseDuration, settings->ShortBreakDuration);
            break;
        case FlowSequence::LongBreak:
            ForceTimerUpdate(durationLeft, baseDuration, settings->LongBreakDuration);
            break;
        default:
            qDebug() << "something wrong with updatingTimerDuration";
            break;
    }

    if(alarmStartChanged)
    {
        qDebug() << "alarm start changed";
        startButtonClickEffect->setSource(QUrl::fromLocalFile(settings->CurrentSessionAlarm.fileName()));
        startButtonClickEffect->play();
    }

    if(settings->GetSessionVolumeForAudio() != startButtonClickEffect->volume())
    {
        startButtonClickEffect->setVolume(settings->GetSessionVolumeForAudio());
    }

    if(alarmBreakChanged)
    {
        qDebug() << "break path setting";
        endBreakEffect->setSource(QUrl::fromLocalFile(settings->CurrentSessionAlarm.fileName()));
    }

    if(settings->GetBreakVolumeForAudio() != endBreakEffect->volume())
    {
        endBreakEffect->setVolume(settings->GetBreakVolumeForAudio());
    }

    if(settings->BreakAlarmRepetitions != endBreakEffect->loopCount())
    {
        endBreakEffect->setLoopCount(settings->BreakAlarmRepetitions);
    }

    if(quickActionShortcut->key() != settings->QuickActionShortcut)
    {
        quickActionShortcut->setKey(settings->QuickActionShortcut);
    }

}
