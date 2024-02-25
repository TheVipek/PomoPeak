#ifndef POMOPEAK_H
#define POMOPEAK_H

#include <QMainWindow>
#include <QTimer>
#include "pomosettings.h"
#include "flowhandler.h"
#include "taskmanager.h"
#include <vector>
#include "taskQT.h"
#include <QVBoxLayout>
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
public slots:
    void OnChangeState();
    void OnTimerTimeout();
    void OnTryAddTask();
    void OnCurrentActiveTaskChanged(taskQT* task);
    void OnViewModeTaskChanged(taskQT* task);
private:
    Ui::PomoPeak* ui;
    std::vector<Ui::taskQT*> avaliableTasks;
    taskQT* currentActiveTask = nullptr;
    taskQT* currentInViewModeTask = nullptr;
    QTimer timer;
    PomoSettings settings;
    FlowHandler flowHandler;
    TaskManager taskManager;
    int globalCounter;
    int durationLeft;
    bool isRunning;
    bool isTaskBeingCreated = false;
    void UpdateCounter();
    void Skip();
    void UpdateTimerLabel(QString value);
    void UpdateTimerDuration(FlowSequence sequence);
    void AdjustButtonsVisibilityDependingOnCurrentState();
    void AddTask(std::shared_ptr<Task> task);
    void RemoveTask(std::shared_ptr<Task> task);
};
#endif // POMOPEAK_H
