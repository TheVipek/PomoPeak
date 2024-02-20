#ifndef POMOPEAK_H
#define POMOPEAK_H

#include <QMainWindow>
#include <QTimer>
#include "pomosettings.h"
#include "flowhandler.h"
#include "taskmanager.h"

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
    void OnAddTask();
private:
    Ui::PomoPeak* ui;
    QTimer timer;
    PomoSettings settings;
    FlowHandler flowHandler;
    TaskManager taskManager;
    int globalCounter;
    int durationLeft;
    bool isRunning;



    void UpdateCounter();
    void Skip();
    void UpdateTimerLabel(QString value);
    void UpdateTimerDuration(FlowSequence sequence);
    void AdjustButtonsVisibilityDependingOnCurrentState();
};
#endif // POMOPEAK_H
