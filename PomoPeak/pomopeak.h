#ifndef POMOPEAK_H
#define POMOPEAK_H

#include <QMainWindow>
#include <QTimer>
#include "PomoPeakSettings.h"

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
private:
    Ui::PomoPeak* ui;
    QTimer* timer;

    Settings settings;
    int durationLeft;
    bool isRunning;
    void SwitchFlow();
    void UpdateCounter();
    void Skip();
    void UpdateTimer(QString value);
    void ResetTimer();
};
#endif // POMOPEAK_H
