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
private:
    Ui::PomoPeak* ui;
    pomopeaksettings* pomopeakSettings;
    Settings* settings;
    SqliteHandler* sqliteHandler;

    std::vector<Ui::taskQT*> avaliableTasks;
    taskQT* currentActiveTaskUI = nullptr;
    taskQT* currentInViewModeTaskUI = nullptr;

    QMediaPlayer* startButtonClickEffect;
    QMediaPlayer* endBreakEffect;
    QAudioOutput* startButtonOutput;
    QAudioOutput* endBreakOutput;
    FlowHandler* flowHandler;

    QTimer timer;
    TaskManager taskManager;

    int globalCounter;
    int durationLeft;

    bool isRunning;
    bool isTaskBeingCreated = false;
    bool settingsOpen = false;


    void UpdateCounter();
    void Skip();
    void UpdateTimerLabel(QString value);
    void UpdateTimerDuration(FlowSequence sequence);
    void AdjustButtonsVisibilityDependingOnCurrentState();
    void AddTask(std::shared_ptr<Task> task);
    void RemoveTask(std::shared_ptr<Task> task);
};
#endif // POMOPEAK_H
