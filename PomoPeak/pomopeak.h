#ifndef POMOPEAK_H
#define POMOPEAK_H

#include <QMainWindow>
#include <QTimer>
#include <QVBoxLayout>
#include <QSoundEffect>
#include <vector>

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
QT_BEGIN_NAMESPACE
namespace Ui {
class PomoPeak;
}
QT_END_NAMESPACE

class PomoPeak : public QMainWindow
{
    Q_OBJECT

public:
    PomoPeak(QWidget *parent = nullptr);
    ~PomoPeak();
protected slots:
    void OnChangeState();
    void OnTimerTimeout();
    void OnTryAddTask();
    void OnCurrentActiveTaskChanged(taskQT* taskUI);
    void OnViewModeTaskChanged(taskQT* taskUI);
    void OnOpenSettings();
    void OnHideSettings(const bool alarmStartChanged,const bool alarmBreakChanged);
private:
    Ui::PomoPeak* ui;
    pomopeaksettings* pomopeakSettings;
    Settings* settings;
    SqliteHandler* sqliteHandler;
    std::vector<Ui::taskQT*> avaliableTasks;
    taskQT* currentActiveTaskUI = nullptr;
    taskQT* currentInViewModeTaskUI = nullptr;

    QSoundEffect* startButtonClickEffect;
    QSoundEffect* endBreakEffect;
    FlowHandler* flowHandler;

    QTimer timer;
    TaskManager taskManager;
    TrayIconHandler trayIconHandler;
    int globalCounter;

    int durationLeft;
    int baseDuration;

    bool isRunning;

    bool isTaskBeingCreated = false;

    QShortcut* quickActionShortcut;

    void UpdateCounter();
    void Skip();
    void UpdateTimerLabel(QString value);
    void UpdateTimerDuration(FlowSequence sequence);
    void AdjustButtonsVisibilityDependingOnCurrentState();
    void AddTask(std::shared_ptr<Task> task);
    void RemoveTask(std::shared_ptr<Task> task);
    void ForceTimerUpdate(int& durationLeft, int& baseDuration, const int targetDuration);
    void TriggerQuickAction();
};
#endif // POMOPEAK_H
