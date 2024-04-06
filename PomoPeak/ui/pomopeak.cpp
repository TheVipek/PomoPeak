#include "pomopeak.h"
#include "./ui_pomopeak.h"
#include "settingsdto.h"
#include "databasetables.h"
#include <QDir>
#include "userstatsdto.h"
PomoPeak::PomoPeak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PomoPeak)
    , sqliteHandler(new SqliteHandler(QCoreApplication::applicationDirPath() + "/data/database/applicationData.db"))
    , gptHelper(new GPTHelper())
    , timer(new QTimer)
    , taskManager()
    , trayIconHandler()
{
    ui->setupUi(this);
    ui->widgetsLayout->setAlignment(Qt::AlignCenter);
    ui->tasksContentV2->setAlignment(Qt::AlignTop);

    trayIconHandler.Show();
    InitializeDataContainer();
    InitializeObjects();
    SubscribeToEvents();

    gptHelper->SetAPIKey(settings->ChatGPTApiKey);
    //gptHelper->Ask("Give me 10 facts about pomodoro technique", "You're pomodoro professional, all questions are SHORT, by it I mean, you're jsut giving what user want, without any additionals, start every fact with, 'Did You Know That' ");
}

PomoPeak::~PomoPeak()
{
    delete ui;
    delete flowHandler;
    delete startButtonClickEffect;
    delete endBreakEffect;
    delete settings;
    delete sqliteHandler;
    delete userStats;
    delete pomopeakStats;
    delete quickActionShortcut;
}

void PomoPeak::InitializeDataContainer()
{
    QString settingsQuery = QString("SELECT * FROM %1 WHERE UserID = 0 LIMIT 1").arg(DatabaseTables::SETTINGS);

    auto settingsDTO = sqliteHandler->GetData<SettingsDTO>(settingsQuery);
    if(!settingsDTO.empty())
    {
        qDebug() << "Creating settings from DTO";
        settings = new Settings(settingsDTO.front());
    }
    else
    {
        qDebug() << "Creatin default settings";
        settings = new Settings();
        sqliteHandler->SetData(DatabaseTables::SETTINGS, settings->ToData(0));
    }

    QString statsQuery = QString("SELECT * FROM %1 WHERE UserID = 0 LIMIT 1").arg(DatabaseTables::STATS);
    auto statsDTO = sqliteHandler->GetData<UserStatsDTO>(statsQuery);
    if(!statsDTO.empty())
    {
        qDebug() << "Creating stats from DTO";
        userStats = new UserStats(statsDTO.front());
    }
    else
    {
        qDebug() << "Creatin default stats";
        userStats = new UserStats();
        sqliteHandler->SetData(DatabaseTables::STATS, userStats->ToData(0));
    }


}

void PomoPeak::InitializeObjects()
{
    flowHandler = new FlowHandler(*settings);

    pomopeakSettings = new pomopeaksettings(*settings, *sqliteHandler, *gptHelper , this);
    pomopeakSettings->hide();
    ui->widgetsLayout->addWidget(pomopeakSettings);

    pomopeakStats = new PomopeakStats(*userStats,this);
    pomopeakStats->hide();
    ui->widgetsLayout->addWidget(pomopeakStats);

    quickActionShortcut = new QShortcut(settings->QuickActionShortcut, this);

    startButtonClickEffect = new QSoundEffect(this);
    startButtonClickEffect->setSource(QUrl::fromLocalFile(settings->CurrentSessionAlarm.fileName()));
    startButtonClickEffect->setVolume(settings->GetSessionVolumeForAudio());

    endBreakEffect = new QSoundEffect(this);
    endBreakEffect->setSource(QUrl::fromLocalFile(settings->CurrentBreakAlarm.fileName()));
    endBreakEffect->setVolume(settings->GetBreakVolumeForAudio());
    endBreakEffect->setLoopCount(settings->BreakAlarmRepetitions);


    UpdateTimerDuration(flowHandler->GetCurrentFlowSequence());
    UpdateTimerLabel(QString("%1:%2").arg(durationLeft / 60,2,10,QChar('0')).arg((durationLeft % 60),2,10,QChar('0')));
    timer.setInterval(1000);

    AdjustButtonsVisibilityDependingOnCurrentState();
}

void PomoPeak::SubscribeToEvents()
{
    connect(pomopeakSettings, &pomopeaksettings::OnClose, this, &PomoPeak::OnHideSettings);
    connect(pomopeakStats, &PomopeakStats::OnClose, this, &PomoPeak::OnHideStats);
    connect(quickActionShortcut, &QShortcut::activated, this, &PomoPeak::TriggerQuickAction);
    connect(ui->settingsBtn, &QPushButton::clicked, this, &PomoPeak::OnOpenSettings);
    connect(ui->statsBtn, &QPushButton::clicked, this, &PomoPeak::OnOpenStats);
    connect(&timer, &QTimer::timeout, this, &PomoPeak::OnTimerTimeout);
    connect(ui->ChangeFlowBtn, &QPushButton::clicked, this, &PomoPeak::OnChangeState);
    connect(ui->SkipBtn, &QPushButton::clicked, this, &PomoPeak::Skip);
    connect(ui->AddTaskBtn, &QPushButton::clicked, this, &PomoPeak::OnTryAddTask);
    connect(&trayIconHandler, &TrayIconHandler::Open, this, &PomoPeak::show);
    connect(&trayIconHandler, &TrayIconHandler::Exit, this, &PomoPeak::OnAppQuit);
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
        auto currentSequence = flowHandler->GetCurrentFlowSequence();

        if(currentSequence == FlowSequence::Session)
        {
            if(currentActiveTaskUI != nullptr)
            {
                currentActiveTaskUI->IncreasePomodorosDone();
            }
            userStats->AddTimeSpend(((float)settings->SessionDuration/60));
            globalCounter++;
            PlayNotification("Break", "", 5000);

        }
        if(currentSequence == FlowSequence::LongBreak || currentSequence == FlowSequence::ShortBreak)
        {
            PlaySoundEffect(endBreakEffect, true);
            PlayNotification("Session", "", 5000);

        }
        UpdateTimerDuration(flowHandler->Next());
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
    connect(newTaskUI, &taskQT::OnStatusChanged, this, &PomoPeak::TaskStatusChanged);
    ui->tasksContentV2->addWidget(newTaskUI);
}

void PomoPeak::AddTask(std::shared_ptr<Task> task)
{
    taskManager.AddTask(task);
}

void PomoPeak::RemoveTask(std::shared_ptr<Task> task, taskQT* taskUI)
{
    if(currentInViewModeTaskUI == taskUI)
    {
        currentInViewModeTaskUI = nullptr;
    }
    if(currentActiveTaskUI == taskUI)
    {
        currentActiveTaskUI = nullptr;
    }
    taskManager.RemoveTask(task);
    taskUI->deleteLater();
}

void PomoPeak::OnViewModeTaskChanged(taskQT* taskUI)
{
    if(currentInViewModeTaskUI != nullptr && currentInViewModeTaskUI != taskUI)
    {
        currentInViewModeTaskUI->ChangeMode(taskQT::Mode::None);
    }
    currentInViewModeTaskUI = taskUI;
}

void PomoPeak::OnCurrentActiveTaskChanged(taskQT* taskUI)
{
    if(currentActiveTaskUI != nullptr && currentActiveTaskUI != taskUI)
    {
        currentActiveTaskUI->SwitchTaskActivation();
    }
    currentActiveTaskUI = taskUI;
}

void PomoPeak::TaskStatusChanged(bool done)
{
    userStats->AddTaskCompletion(QDate::currentDate(), done ? 1 : -1);
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
    ui->SkipBtn->setVisible(isRunning);
    isRunning ? ui->ChangeFlowBtn->setText("Stop") :  ui->ChangeFlowBtn->setText("Start");
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

    switch(flowHandler->GetCurrentFlowSequence())
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

    pomopeakStats->ForceUpdateChart();
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
        play ? effect->play() : effect->stop();
    }
}

void PomoPeak::PlayNotification(const QString title, const QString message, const int msDuration, bool ignoreRestrictions)
{
    if(ignoreRestrictions)
    {
        trayIconHandler.SendMessage(title, message, msDuration);
    }
    else
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
}

void PomoPeak::OnAppQuit()
{
    isQuitting = true;
    QList<QPair<QString,QVariant>> conditions = { {"UserID", 0} };
    sqliteHandler->Update(DatabaseTables::STATS, userStats->ToData(),conditions);
    QApplication::quit();
}
