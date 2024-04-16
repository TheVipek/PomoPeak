#include "pomopeak.h"
#include "./ui_pomopeak.h"
#include "settingsdto.h"
#include "databasetables.h"
#include <QDir>
#include "userstatsdto.h"
PomoPeak::PomoPeak(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PomoPeak)
    , timer(QTimer(this))
    , taskManager()
    , trayIconHandler()
{
    ui->setupUi(this);

    trayIconHandler.Show();

    sqliteHandler = std::make_shared<SqliteHandler>(QCoreApplication::applicationDirPath() + "/data/database/applicationData.db");
    InitializeDataContainer();
    InitializeObjects();
    SubscribeToEvents();
}

PomoPeak::~PomoPeak()
{
    UnsubscribeFromEvents();

    delete ui;
}

void PomoPeak::InitializeDataContainer()
{
    QString settingsQuery = QString("SELECT * FROM %1 WHERE UserID = 0 LIMIT 1").arg(DatabaseTables::SETTINGS);

    auto settingsDTO = sqliteHandler->GetData<SettingsDTO>(settingsQuery);
    if(!settingsDTO.empty())
    {
        qDebug() << "Creating settings from DTO";
        settings = std::make_shared<Settings>(settingsDTO.front());
    }
    else
    {
        qDebug() << "Creatin default settings";
        settings = std::make_shared<Settings>();
        sqliteHandler->SetData(DatabaseTables::SETTINGS, settings->ToData(0));
    }

    QString statsQuery = QString("SELECT * FROM %1 WHERE UserID = 0 LIMIT 1").arg(DatabaseTables::STATS);
    auto statsDTO = sqliteHandler->GetData<UserStatsDTO>(statsQuery);
    if(!statsDTO.empty())
    {
        qDebug() << "Creating stats from DTO";
        userStats = std::make_shared<UserStats>(statsDTO.front());
    }
    else
    {
        qDebug() << "Creatin default stats";
        userStats = std::make_shared<UserStats>();
        sqliteHandler->SetData(DatabaseTables::STATS, userStats->ToData(0));
    }


}

void PomoPeak::InitializeObjects()
{
    gptHelper = std::make_shared<GPTHelper>();
    gptHelper->SetAPIKey(settings->ChatGPTApiKey);

    pomodoroFacts = std::make_shared<PomodoroFacts>(gptHelper);

    flowHandler = std::make_shared<FlowHandler>(settings);
    pomopeakSettings = new pomopeaksettings(settings, sqliteHandler, gptHelper , this);
    pomopeakSettings->hide();
    ui->widgetsLayout->addWidget(pomopeakSettings);
    pomopeakStats = new PomopeakStats(userStats,this);
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
void PomoPeak::UnsubscribeFromEvents()
{
    disconnect(pomopeakSettings, &pomopeaksettings::OnClose, this, &PomoPeak::OnHideSettings);
    disconnect(pomopeakStats, &PomopeakStats::OnClose, this, &PomoPeak::OnHideStats);
    disconnect(quickActionShortcut, &QShortcut::activated, this, &PomoPeak::TriggerQuickAction);
    disconnect(ui->settingsBtn, &QPushButton::clicked, this, &PomoPeak::OnOpenSettings);
    disconnect(ui->statsBtn, &QPushButton::clicked, this, &PomoPeak::OnOpenStats);
    disconnect(&timer, &QTimer::timeout, this, &PomoPeak::OnTimerTimeout);
    disconnect(ui->ChangeFlowBtn, &QPushButton::clicked, this, &PomoPeak::OnChangeState);
    disconnect(ui->SkipBtn, &QPushButton::clicked, this, &PomoPeak::Skip);
    disconnect(ui->AddTaskBtn, &QPushButton::clicked, this, &PomoPeak::OnTryAddTask);
    disconnect(&trayIconHandler, &TrayIconHandler::Open, this, &PomoPeak::show);
    disconnect(&trayIconHandler, &TrayIconHandler::Exit, this, &PomoPeak::OnAppQuit);
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
            PlayNotification("Break", pomodoroFacts->GetNextFact(), 5000);

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
    connect(newTaskUI, &taskQT::CreateRequest, this, &PomoPeak::AddTask);
    connect(newTaskUI, &taskQT::DeleteRequest, this, &PomoPeak::RemoveTask);
    connect(newTaskUI, &taskQT::OnEnableViewModeRequest, this, &PomoPeak::OnViewModeTaskChanged);
    connect(newTaskUI, &taskQT::OnSelectRequest, this, &PomoPeak::OnCurrentActiveTaskChanged);
    connect(newTaskUI, &taskQT::OnStatusChanged, this, &PomoPeak::TaskStatusChanged);
    connect(newTaskUI, &taskQT::OnNoneModeRequest, this, &PomoPeak::OnNoneTaskMode);
    ui->taskLayoutListScrollAreaContent->layout()->addWidget(newTaskUI);

    ChangeAddTaskBtnInteractability(false);
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

    ChangeAddTaskBtnInteractability(true);
}

void PomoPeak::OnViewModeTaskChanged(taskQT* taskUI)
{
    if(currentInViewModeTaskUI != nullptr && currentInViewModeTaskUI != taskUI)
    {
        currentInViewModeTaskUI->ChangeMode(taskQT::Mode::None);
    }
    currentInViewModeTaskUI = taskUI;

    ChangeAddTaskBtnInteractability(false);

}

void PomoPeak::OnNoneTaskMode(taskQT* taskUI)
{
    ChangeAddTaskBtnInteractability(true);
}

void PomoPeak::ChangeAddTaskBtnInteractability(bool v)
{
    ui->AddTaskBtn->setEnabled(v);
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

    if(gptHelper->GetAPIKey() != settings->ChatGPTApiKey)
    {
        gptHelper->SetAPIKey(settings->ChatGPTApiKey);

        pomodoroFacts->TryRefillFacts();
    }

}

void PomoPeak::OnOpenStats()
{
    ui->widget->hide();

    pomopeakStats->ForceUpdate();
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
    QList<QPair<QString,QVariant>> conditions = { QPair<QString, QVariant>("UserID", QVariant(0)) };
    sqliteHandler->Update(DatabaseTables::STATS, userStats->ToData(),conditions);
    QApplication::quit();
}
