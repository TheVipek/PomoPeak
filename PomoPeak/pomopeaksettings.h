#ifndef POMOPEAKSETTINGS_H
#define POMOPEAKSETTINGS_H

#include <QWidget>
#include "settings.h"
#include "sqliteHandler.h"
#include <QMediaPlayer>
namespace Ui {
class pomopeaksettings;
}

class pomopeaksettings : public QWidget
{
    Q_OBJECT

public:
    explicit pomopeaksettings(Settings& _settings, SqliteHandler& _handler, QWidget *parent = nullptr);
    ~pomopeaksettings();
    void setVisible(bool visible) override
    {
        QWidget::setVisible(visible);
        IsOpened  = visible;
    }
    bool IsOpened = false;
signals:
    void OnClose(const bool alarmStartChanged,const bool alarmBreakChanged);
private slots:
    void OnSliderValueChanged(int value);
    void OnDoubleSpinBoxValueChanged(double value);
    void OnSpinBoxValueChanged(int value);
    void OnExitClicked();
    void OnSelectAudioClicked();
    void OnQuickActionSequenceFinished();
private:
    Ui::pomopeaksettings *ui;
    Settings& settings;
    SqliteHandler& handler;
    bool isDirty = false;
    bool startAlarmChanged = false;
    bool breakAlarmChanged = false;
};

#endif // POMOPEAKSETTINGS_H
