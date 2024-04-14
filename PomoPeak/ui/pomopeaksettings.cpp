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
pomopeaksettings::pomopeaksettings(Settings& _settings, SqliteHandler& _handler, GPTHelper& _gptHelper, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pomopeaksettings)
    , settings(_settings)
    , handler(_handler)
    , gptHelper(_gptHelper)

{
    ui->setupUi(this);

    InitializeDataContainer();
    InitializeObjects();
    SubscribeToEvents();
}

pomopeaksettings::~pomopeaksettings()
{
    delete ui;
}

void pomopeaksettings::InitializeDataContainer()
{
    for(const std::pair<QString,Skin::SkinTypes>& item : Skin::SkinTypesEnumerable)
    {
        ui->SkinSelectionComboBox->addItem(item.first,QVariant::fromValue(item.second));
    }
    ui->SkinSelectionComboBox->setCurrentIndex(static_cast<int>(settings.Skin));
}

void pomopeaksettings::InitializeObjects()
{
    ui->sessionDoubleSpinBox->setValue((double)settings.SessionDuration / 60);
    ui->longBreakDoubleSpinBox->setValue((double)settings.LongBreakDuration / 60);
    ui->shortBreakDoubleSpinBox->setValue((double)settings.ShortBreakDuration / 60);

    ui->alarmEndBreakRepSpinBox->setValue(settings.BreakAlarmRepetitions);

    ui->alarmStartCurrentLabel->setText(QFileInfo(settings.CurrentSessionAlarm.fileName()).fileName());
    ui->alarmEndBreakCurrentLabel->setText(QFileInfo(settings.CurrentBreakAlarm.fileName()).fileName());

    ui->alarmStartSlider->setValue(settings.SessionAlarmVolume);
    ui->alarmStartSliderValueLabel->setText(QString::number(settings.SessionAlarmVolume));

    ui->alarmEndBreakVolumeSlider->setValue(settings.BreakAlarmVolume);
    ui->alarmEndSliderValue->setText(QString::number(settings.BreakAlarmVolume));

    ui->quickActionSequenceEdit->setKeySequence(settings.QuickActionShortcut);

    ui->notificationsCheckbox->setChecked(settings.Notifications);
    ui->alarmSoundCheckBox->setChecked(settings.AlarmSound);
    ui->minimizeToTrayCheckbox->setChecked(settings.MinimizeToTray);
    ui->chatGPTLineEdit->setText(settings.ChatGPTApiKey);
    SetChatGPTLineEditEchoMode(QLineEdit::Password);
}

void pomopeaksettings::SubscribeToEvents()
{
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
    connect(ui->minimizeToTrayCheckbox, &QCheckBox::clicked, this, &pomopeaksettings::OnCheckBoxValueChanged);

    connect(ui->SkinSelectionComboBox, &QComboBox::currentIndexChanged, this, &pomopeaksettings::OnIndexInComboBoxChanged);

    connect(ui->chatGPTVisibilityBtn, &QPushButton::clicked, this, &pomopeaksettings::OnChangeChatGPTVisibility);
    connect(ui->chatGPTLineEdit, &QLineEdit::textEdited, this, &pomopeaksettings::OnChatGPTApiChanged);
}

void pomopeaksettings::OnChatGPTApiChanged(const QString& str)
{
    settings.ChatGPTApiKey = str;
    isDirty = true;
}
void pomopeaksettings::OnChangeChatGPTVisibility()
{
    QObject* obj = sender();

    if(obj == ui->chatGPTVisibilityBtn)
    {
        ui->chatGPTLineEdit->echoMode() == QLineEdit::Password ? SetChatGPTLineEditEchoMode(QLineEdit::Normal) : SetChatGPTLineEditEchoMode(QLineEdit::Password);
    }
}
void pomopeaksettings::SetChatGPTLineEditEchoMode(const QLineEdit::EchoMode& mode)
{
    ui->chatGPTLineEdit->setEchoMode(mode);
    mode == QLineEdit::Password ? ui->chatGPTVisibilityBtn->setText("Show") : ui->chatGPTVisibilityBtn->setText("Hide");
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
        if(!isValid)
        {
            return;
        }

        qDebug() << "valid!";

        if(obj == ui->alarmStartSelectBtn)
        {
            if(settings.CurrentSessionAlarm.size() == file.size())
            {
                qDebug() << "you selected the same file? (its size is equal)";
                return;
            }

            QString newPath = QDir(QCoreApplication::applicationDirPath() + settings.SessionAlarmsPath).filePath(fileInfo.fileName());

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
            if(settings.CurrentBreakAlarm.size() == file.size())
            {
                qDebug() << "you selected the same file? (its size is equal)";
                return;
            }

            QString newPath = QDir(QCoreApplication::applicationDirPath() + settings.BreakAlarmsPath).filePath(fileInfo.fileName());

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
    else if(obj == ui->minimizeToTrayCheckbox)
    {
        settings.MinimizeToTray = value;
        isDirty = true;
    }
}

void pomopeaksettings::OnExitClicked()
{
    if(isDirty)
    {
        QList<QPair<QString,QVariant>> conditions = { {"UserID", 0} };

        handler.Exist(DatabaseTables::SETTINGS, conditions) ? handler.Update(DatabaseTables::SETTINGS, settings.ToData(),conditions) : handler.SetData(DatabaseTables::SETTINGS, settings.ToData(0));
        isDirty = false;
    }
    emit OnClose(startAlarmChanged, breakAlarmChanged);
    startAlarmChanged = false;
    breakAlarmChanged = false;
    SetChatGPTLineEditEchoMode(QLineEdit::Password);
}
