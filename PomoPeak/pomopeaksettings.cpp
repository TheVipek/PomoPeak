#include "pomopeaksettings.h"
#include "ui_pomopeaksettings.h"
#include <QObject>
#include <QDebug>
#include <QFileDialog>
#include "audiomimetypeshelper.h"
#include <QFile>
#include "databasetables.h"
#include <QMimeDatabase>
#include <QMimeData>
#include <QFileInfo>
pomopeaksettings::pomopeaksettings(Settings& _settings, SqliteHandler& _handler, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pomopeaksettings)
    , settings(_settings)
    , handler(_handler)
{
    ui->setupUi(this);


    ui->sessionDoubleSpinBox->setValue((double)_settings.SessionDuration / 60);
    ui->longBreakDoubleSpinBox->setValue((double)_settings.LongBreakDuration / 60);
    ui->shortBreakDoubleSpinBox->setValue((double)_settings.ShortBreakDuration / 60);

    ui->alarmEndBreakRepSpinBox->setValue(_settings.BreakAlarmRepetitions);

    ui->alarmStartCurrentLabel->setText(QFileInfo(_settings.CurrentSessionAlarm.fileName()).fileName());
    ui->alarmEndBreakCurrentLabel->setText(QFileInfo(_settings.CurrentBreakAlarm.fileName()).fileName());

    ui->alarmStartSlider->setValue(_settings.SessionAlarmVolume);
    ui->alarmStartSliderValueLabel->setText(QString::number(_settings.SessionAlarmVolume));

    ui->alarmEndBreakVolumeSlider->setValue(_settings.BreakAlarmVolume);
    ui->alarmEndSliderValue->setText(QString::number(_settings.BreakAlarmVolume));

    ui->quickActionSequenceEdit->setKeySequence(_settings.QuickActionShortcut);

    ui->notificationsCheckbox->setChecked(_settings.Notifications);
    ui->alarmSoundCheckBox->setChecked(_settings.AlarmSound);

    for(auto item : Skin::SkinTypesEnumerable)
    {
        ui->SkinSelectionComboBox->addItem(item.first,QVariant::fromValue(item.second));
    }
    ui->SkinSelectionComboBox->setCurrentIndex(static_cast<int>(_settings.Skin));

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

    connect(ui->notificationsCheckbox, &QCheckBox::clicked, this, &pomopeaksettings::OnCheckBoxValueChanged);
    connect(ui->alarmSoundCheckBox, &QCheckBox::clicked, this, &pomopeaksettings::OnCheckBoxValueChanged);

    connect(ui->SkinSelectionComboBox, &QComboBox::currentIndexChanged, this, &pomopeaksettings::OnIndexInComboBoxChanged);
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
        isDirty = true;
    }
}

void pomopeaksettings::OnSelectAudioClicked()
{
    QObject* obj = sender();
    QString selectedFilePath = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath(), audioMimeTypesHelper::GetAllMimeTypeForFileDialog());

    if(!selectedFilePath.isEmpty())
    {
        QFile file(selectedFilePath);
        QFileInfo fileInfo(file);
        QString ext = fileInfo.suffix();
        bool isValid = audioMimeTypesHelper::IsMimeTypeValid(ext);
        if(isValid)
        {
            qDebug() << "valid!";

            if(obj == ui->alarmStartSelectBtn)
            {
                QString newPath = QDir(QCoreApplication::applicationDirPath() + settings.SessionAlarmsPath).filePath(fileInfo.fileName());

                if(settings.CurrentSessionAlarm.size() == file.size())
                {
                    qDebug() << "you selected the same file? (its size is equal)";
                    return;
                }

                if(settings.CurrentSessionAlarm.size() != settings.DefaultSessionAlarm.size())
                {
                    QFile::remove(settings.CurrentSessionAlarm.fileName());
                    file.copy(newPath);
                }
                else if(settings.CurrentSessionAlarm.size() == settings.DefaultSessionAlarm.size())
                {
                    file.copy(newPath);
                }

                settings.CurrentSessionAlarm.setFileName(newPath);
                ui->alarmStartCurrentLabel->setText(fileInfo.fileName());
                isDirty = true;
                startAlarmChanged = true;
            }
            else if(obj == ui->alarmEndBreakSelectBtn)
            {
                QString newPath = QDir(QCoreApplication::applicationDirPath() + settings.BreakAlarmsPath).filePath(fileInfo.fileName());

                if(settings.CurrentBreakAlarm.size() == file.size())
                {
                    qDebug() << "you selected the same file? (its size is equal)";
                    return;
                }


                if(settings.CurrentBreakAlarm.size() != settings.DefaultBreakAlarm.size())
                {
                    QFile::remove(settings.CurrentBreakAlarm.fileName());
                    file.copy(newPath);
                }
                else if(settings.CurrentBreakAlarm.size() == settings.DefaultBreakAlarm.size())
                {
                    file.copy(newPath);
                }

                settings.CurrentBreakAlarm.setFileName(newPath);
                ui->alarmEndBreakLabel->setText(fileInfo.fileName());
                isDirty = true;
                breakAlarmChanged = true;
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
        isDirty = true;
    }
}

void pomopeaksettings::OnDoubleSpinBoxValueChanged(double value)
{
    QObject* obj = sender();
    if(obj == ui->sessionDoubleSpinBox)
    {
        settings.SetSessionDuration(value);
        isDirty = true;
    }
    else if(obj == ui->longBreakDoubleSpinBox)
    {
        settings.SetLongBreakDuration(value);
        isDirty = true;
    }
    else if(obj == ui->shortBreakDoubleSpinBox)
    {
        settings.SetShortBreakDuration(value);
        isDirty = true;
    }
}

void pomopeaksettings::OnSliderValueChanged(int value)
{
    QObject* obj = sender();
    if(obj == ui->alarmStartSlider)
    {
        ui->alarmStartSliderValueLabel->setText(QString::number(value));
        settings.SessionAlarmVolume = value;
        isDirty = true;
    }
    else if(obj == ui->alarmEndBreakVolumeSlider)
    {
        ui->alarmEndSliderValue->setText(QString::number(value));
        settings.BreakAlarmVolume = value;
        isDirty = true;
    }
}
void pomopeaksettings::OnIndexInComboBoxChanged(int index)
{
    QObject* obj = sender();
    if(obj == ui->SkinSelectionComboBox)
    {
        settings.Skin = static_cast<Skin::SkinTypes>(index);
        isDirty = true;
    }
}
void pomopeaksettings::OnCheckBoxValueChanged(bool value)
{
    QObject* obj = sender();
    if(obj == ui->notificationsCheckbox)
    {
        settings.Notifications = value;
        isDirty = true;
    }
    else if(obj == ui->alarmSoundCheckBox)
    {
        settings.AlarmSound = value;
        isDirty = true;
    }
}

void pomopeaksettings::OnExitClicked()
{
    if(isDirty)
    {
        QList<QPair<QString,QVariant>> conditions = { {"UserID", settings.DefaultID} };
        if(!handler.Exist(DatabaseTables::SETTINGS, conditions))
        {
            qDebug() << "doesnt exist";
            handler.SetData(DatabaseTables::SETTINGS, settings.ToData(settings.DefaultID));
        }
        else
        {
            qDebug() << "updating";
            handler.Update(DatabaseTables::SETTINGS, settings.ToData(),conditions);
        }

        isDirty = false;
    }
    emit OnClose(startAlarmChanged, breakAlarmChanged);
    startAlarmChanged = false;
    breakAlarmChanged = false;
}
