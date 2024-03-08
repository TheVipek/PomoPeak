#include "pomopeaksettings.h"
#include "ui_pomopeaksettings.h"
#include <QObject>
#include <QDebug>
#include <QFileDialog>
#include "audiocodeshelper.h"
#include <algorithm>
#include <unordered_set>
#include <QFile>
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
    ui->alarmStartSliderValueLabel->setText(QString::number(_settings.SessionAlarmVolume));

    ui->alarmEndBreakVolumeSlider->setValue(_settings.BreakAlarmVolume);
    ui->alarmEndSliderValue->setText(QString::number(_settings.BreakAlarmVolume));

    ui->quickActionSequenceEdit->setKeySequence(_settings.QuickActionShortcut);


    connect(ui->sessionDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, &pomopeaksettings::OnDoubleSpinBoxValueChanged);
    connect(ui->longBreakDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, &pomopeaksettings::OnDoubleSpinBoxValueChanged);
    connect(ui->shortBreakDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, &pomopeaksettings::OnDoubleSpinBoxValueChanged);

    connect(ui->alarmEndBreakRepSpinBox, &QSpinBox::valueChanged, this, &pomopeaksettings::OnSpinBoxValueChanged);


    connect(ui->alarmStartSlider,&QSlider::valueChanged, this, &pomopeaksettings::OnSliderValueChanged);
    connect(ui->alarmEndBreakVolumeSlider,&QSlider::valueChanged, this, &pomopeaksettings::OnSliderValueChanged);

    connect(ui->exitBtn, &QPushButton::clicked, this, &pomopeaksettings::OnExitClicked);

    connect(ui->alarmStartSelectBtn, &QPushButton::clicked, this, &pomopeaksettings::OnSelectAudioClicked);
    connect(ui->alarmEndBreakSelectBtn, &QPushButton::clicked, this, &pomopeaksettings::OnSelectAudioClicked);

    connect(ui->quickActionSequenceEdit, &QKeySequenceEdit::editingFinished, this, &pomopeaksettings::OnQuickActionSequenceFinished);
    //1. If its hiding update settings
    //2. If its not hidden but destructor is called
}
pomopeaksettings::~pomopeaksettings()
{
    delete ui;
}
void pomopeaksettings::OnQuickActionSequenceFinished()
{
    QObject* obj = sender();

    if(obj == ui->quickActionSequenceEdit)
    {
        settings.QuickActionShortcut = ui->quickActionSequenceEdit->keySequence();
    }
}

void pomopeaksettings::OnSelectAudioClicked()
{
    QObject* obj = sender();

    QString selectedFilePath = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath());

    if(!selectedFilePath.isEmpty())
    {
        QFileInfo fileInfo(selectedFilePath);
        QString ext = fileInfo.suffix();
        std::unordered_set<QString> formats = audioCodesHelper::getSupportedAudioFormats();
        bool isValid = std::any_of(formats.begin(), formats.end(), [ext](QString x){ return x.startsWith(ext); });

        if(isValid)
        {
            if(obj == ui->alarmStartSelectBtn)
            {
                for(auto& format : formats)
                {
                    QFile file(QCoreApplication::applicationDirPath() + settings.CustomSessionAlarmPath + "." + format);
                    if(file.exists())
                    {
                        file.remove();
                    }
                }

                QFile newFile(selectedFilePath);
                newFile.rename(QCoreApplication::applicationDirPath() + settings.CustomSessionAlarmPath + "." + ext);
                settings.SessionAlarm = selectedFilePath;
                ui->alarmStartCurrentLabel->setText(fileInfo.fileName());

            }
            else if(obj == ui->alarmEndBreakSelectBtn)
            {
                for(auto& format : formats)
                {
                    QFile file(QCoreApplication::applicationDirPath() + settings.CustomBreakAlarmPath + "." + format);
                    if(file.exists())
                    {
                        file.remove();
                    }
                }

                QFile newFile(selectedFilePath);
                newFile.copy(QCoreApplication::applicationDirPath() + settings.CustomBreakAlarmPath + "." + ext);
                settings.BreakAlarm = selectedFilePath;
                ui->alarmEndBreakCurrentLabel->setText(fileInfo.fileName());
            }
        }
    }
}
void pomopeaksettings::OnSpinBoxValueChanged(int value)
{
    QObject* obj = sender();
    if(obj == ui->alarmEndBreakRepSpinBox)
    {
        settings.BreakAlarmRepetitions = value;
    }
}

void pomopeaksettings::OnDoubleSpinBoxValueChanged(double value)
{
    QObject* obj = sender();
    if(obj == ui->sessionDoubleSpinBox)
    {
        settings.SessionDuration = value;
    }
    else if(obj == ui->longBreakDoubleSpinBox)
    {
        settings.LongBreakDuration = value;
    }
    else if(obj == ui->shortBreakDoubleSpinBox)
    {
        settings.ShortBreakDuration = value;
    }
}

void pomopeaksettings::OnSliderValueChanged(int value)
{
    QObject* obj = sender();
    if(obj == ui->alarmStartSlider)
    {
        ui->alarmStartSliderValueLabel->setText(QString::number(value));
        settings.SessionAlarmVolume = value;
    }
    else if(obj == ui->alarmEndBreakVolumeSlider)
    {
        ui->alarmEndSliderValue->setText(QString::number(value));
        settings.BreakAlarmVolume = value;
    }
}

void pomopeaksettings::OnExitClicked()
{
    emit OnClose();
}
