#include "pomopeaksettings.h"
#include "ui_pomopeaksettings.h"

pomopeaksettings::pomopeaksettings(Settings& _settings, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pomopeaksettings)
    , settings(_settings)
{
    ui->setupUi(this);
    ui->sessionDoubleSpinBox->setValue(_settings.SessionDuration / 60);
    ui->longBreakDoubleSpinBox->setValue(_settings.LongBreakDuration / 60);
    ui->shortBreakDoubleSpinBox->setValue(_settings.ShortBreakDuration / 60);
    ui->alarmEndBreakRepSpinBox->setValue(_settings.BreakAlarmRepetitions);
    ui->alarmStartCurrentLabel->setText(_settings.SessionAlarmName);
    ui->alarmEndBreakCurrentLabel->setText(_settings.BreakAlarmName);
    ui->alarmStartSlider->setValue(_settings.SessionAlarmVolume);
    ui->alarmEndBreakVolumeSlider->setValue(_settings.BreakAlarmVolume);
    ui->quickActionSequenceEdit->setKeySequence(_settings.QuickActionShortcut);
}

pomopeaksettings::~pomopeaksettings()
{
    delete ui;
}
