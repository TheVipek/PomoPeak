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
    , trayIconHandler()
{
    sqliteHandler = new SqliteHandler(QCoreApplication::applicationDirPath() + "/data/database/applicationData.sqlite");

    std::ostringstream ss;
    QString query = QString("SELECT * FROM %1 WHERE UserID = 0 LIMIT 1").arg(DatabaseTables::SETTINGS);
    auto DTO = sqliteHandler->GetData<SettingsDTO>(query);
    qDebug() << DTO.data();
    if(!DTO.empty())
    {
        qDebug() << "Creating settings from DTO";
        settings = new Settings(DTO.front());
    }
    else
    {
        qDebug() << "Creatin default settings";
        settings = new Settings();
        sqliteHandler->SetData(DatabaseTables::SETTINGS, settings->ToData(settings->DefaultID));
    }

    flowHandler = new FlowHandler(*settings);

    ui->setupUi(this);
    ui->widgetsLayout->setAlignment(Qt::AlignCenter);

    pomopeakSettings = new pomopeaksettings(*settings, *sqliteHandler , this);
    connect(pomopeakSettings, &pomopeaksettings::OnClose, this, &PomoPeak::OnHideSettings);
    pomopeakSettings->hide();

    pomopeakStats = new PomopeakStats(this);
    connect(pomopeakStats, &PomopeakStats::OnClose, this, &PomoPeak::OnHideStats);
    pomopeakStats->hide();


    ui->widgetsLayout->addWidget(pomopeakSettings);
    ui->widgetsLayout->addWidget(pomopeakStats);


    isRunning = false;
    durationLeft = settings->SessionDuration;
    baseDuration = settings->SessionDuration;
    globalCounter = 0;

    quickActionShortcut = new QShortcut(settings->QuickActionShortcut, this);
    connect(quickActionShortcut, &QShortcut::activated, this, &PomoPeak::TriggerQuickAction);

    startButtonClickEffect = new QSoundEffect(this);
    startButtonClickEffect->setSource(QUrl::fromLocalFile(settings->CurrentSessionAlarm.fileName()));
    startButtonClickEffect->setVolume(settings->GetSessionVolumeForAudio());

    endBreakEffect = new QSoundEffect(this);
    endBreakEffect->setSource(QUrl::fromLocalFile(settings->CurrentBreakAlarm.fileName()));
    endBreakEffect->setVolume(settings->GetBreakVolumeForAudio());
    endBreakEffect->setLoopCount(settings->BreakAlarmRepetitions);

    UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
    AdjustButtonsVisibilityDependingOnCurrentState();

    timer.setInterval(1000);

    connect(ui->settingsBtn, &QPushButton::clicked, this, &PomoPeak::OnOpenSettings);
    connect(ui->statsBtn, &QPushButton::clicked, this, &PomoPeak::OnOpenStats);

    connect(&timer, &QTimer::timeout, this, &PomoPeak::OnTimerTimeout);
    connect(ui->ChangeFlowBtn, &QPushButton::clicked, this, &PomoPeak::OnChangeState);
    connect(ui->SkipBtn, &QPushButton::clicked, this, &PomoPeak::Skip);
    connect(ui->AddTaskBtn, &QPushButton::clicked, this, &PomoPeak::OnTryAddTask);
    connect(&trayIconHandler, &TrayIconHandler::Open, this, &PomoPeak::show);
    connect(&trayIconHandler, &TrayIconHandler::Exit, this, &PomoPeak::OnAppQuit);
    ui->tasksContentV2->setAlignment(Qt::AlignTop);

    trayIconHandler.Show();
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
            PlaySoundEffect(endBreakEffect, false);
        }
        if(startButtonClickEffect->isPlaying())
        {
            PlaySoundEffect(startButtonClickEffect, false);
        }
        PlaySoundEffect(startButtonClickEffect, true);
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


        if(flowHandler->GetCurrentSequence() == FlowSequence::Session)
        {
            //Call to current task update
            if(currentActiveTaskUI != nullptr)
            {
                currentActiveTaskUI->ElapsedIncrease();
            }
            globalCounter++;
            PlayNotification("Break", "", 5000);

        }
        if(flowHandler->GetCurrentSequence() == FlowSequence::LongBreak || flowHandler->GetCurrentSequence() == FlowSequence::ShortBreak)
        {
            PlaySoundEffect(endBreakEffect, true);
            PlayNotification("Session", "", 5000);

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

void PomoPeak::OnOpenSettings()
{
    if(!pomopeakSettings->IsOpened)
    {
        ui->widget->hide();
        pomopeakSettings->show();
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
        startButtonClickEffect->setSource(QUrl::fromLocalFile(settings->CurrentSessionAlarm.fileName()));
    }

    if(settings->GetSessionVolumeForAudio() != startButtonClickEffect->volume())
    {
        startButtonClickEffect->setVolume(settings->GetSessionVolumeForAudio());
    }

    if(alarmBreakChanged)
    {
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

void PomoPeak::OnOpenStats()
{
    ui->widget->hide();
    pomopeakStats->show();
}

void PomoPeak::OnHideStats()
{
    pomopeakStats->hide();
    ui->widget->show();
}
void PomoPeak::PlaySoundEffect(QSoundEffect* effect, bool play)
{
    if(settings->AlarmSound)
    {
        if(play)
        {
            effect->play();
        }
        else
        {
            effect->stop();
        }
    }
}
void PomoPeak::PlayNotification(const QString title, const QString message, const int msDuration)
{
    if(!settings->AlarmSound)
    {
        if(settings->Notifications)
        {
            trayIconHandler.SendMessage(title, message, msDuration);
        }
    }
    else if(settings->Notifications && !this->isActiveWindow())
    {
        trayIconHandler.SendMessage(title, message, msDuration);
    }
}

void PomoPeak::OnAppQuit()
{
    isQuitting = true;
    QApplication::quit();
}
