#ifndef POMOPEAK_H
#define POMOPEAK_H

#include <QMainWindow>
#include <QTimer>
#include <QVBoxLayout>
#include <QSoundEffect>
#include <QSqlDriver>
#include "pomopeaksettings.h"
#include "flowhandler.h"
#include "taskmanager.h"
#include "sqliteHandler.h"
#include "taskQT.h"
#include "settings.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <memory>
#include <QKeySequence>
#include <QShortcut>
#include "trayiconhandler.h"
#include "pomopeakstats.h"
#include "QObjectInitialization.h"
#include "GPTHelper.h"
#include "PomodoroFacts.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class PomoPeak;
}
QT_END_NAMESPACE

class PomoPeak : public QMainWindow, public QObjectInitialization
{
    Q_OBJECT

public:
    PomoPeak(QWidget *parent = nullptr);
    ~PomoPeak();
protected:
    void closeEvent(QCloseEvent* event) override
    {
        if(settings != nullptr && settings->MinimizeToTray == false)
        {
            OnAppQuit();
        }
        else if(!isQuitting)
        {
            PlayNotification("Application has been minimized to tray","", 1000, true);
        }
        QMainWindow::closeEvent(event);
    }

    void InitializeDataContainer() override;
    void InitializeObjects() override;
    void SubscribeToEvents() override;
protected slots:
    void OnChangeState();
    void OnTimerTimeout();
    void OnTryAddTask();
    void OnCurrentActiveTaskChanged(taskQT* taskUI);
    void OnViewModeTaskChanged(taskQT* taskUI);
    void OnNoneTaskMode(taskQT* taskUI);
    void OnOpenSettings();
    void OnHideSettings(const bool alarmStartChanged,const bool alarmBreakChanged);
    void OnOpenStats();
    void OnHideStats();
private:
    Ui::PomoPeak* ui;
    pomopeaksettings* pomopeakSettings;
    PomopeakStats* pomopeakStats;
    std::shared_ptr<Settings> settings;
    std::shared_ptr<UserStats> userStats;
    std::shared_ptr<SqliteHandler> sqliteHandler;
    taskQT* currentActiveTaskUI = nullptr;
    taskQT* currentInViewModeTaskUI = nullptr;
    std::shared_ptr<FlowHandler> flowHandler;
    QSoundEffect* startButtonClickEffect;
    QSoundEffect* endBreakEffect;
    QShortcut* quickActionShortcut;

    std::shared_ptr<GPTHelper> gptHelper;
    std::shared_ptr<PomodoroFacts> pomodoroFacts;

    QTimer timer;
    TaskManager taskManager;
    TrayIconHandler trayIconHandler;

    int globalCounter = 0;
    int durationLeft;
    int baseDuration;

    bool isRunning = false;
    bool isTaskBeingCreated = false;
    bool isQuitting = false;



    void ChangeAddTaskBtnInteractability(bool v);
    void UpdateCounter();
    void Skip();
    void UpdateTimerLabel(QString value);
    void UpdateTimerDuration(FlowSequence sequence);
    void AdjustButtonsVisibilityDependingOnCurrentState();
    void AddTask(std::shared_ptr<Task> task);
    void RemoveTask(std::shared_ptr<Task> task, taskQT* taskU);
    void TaskStatusChanged(bool done);
    void ForceTimerUpdate(int& durationLeft, int& baseDuration, const int targetDuration);
    void TriggerQuickAction();
    void PlaySoundEffect(QSoundEffect* effect, bool play);
    void PlayNotification(const QString title, const QString message, const int msDuration, bool ignoreRestrictions = false);
    void OnAppQuit();
};
#endif // POMOPEAK_H
