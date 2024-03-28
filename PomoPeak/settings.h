#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtTypes>
#include <QString>
#include <QByteArray>
#include <QSqlDatabase>
#include <QUrl>
#include <QShortcut>
#include <QKeySequence>
#include <QFile>
#include <QCoreApplication>
#include <QVariantMap>
#include "settingsdto.h"
#include <QList>
#include <QPair>
#include "Skin.h"
class Settings
{

public:
    Settings();
    Settings(const SettingsDTO& dto);

    qint16 SessionDuration;
    qint16 ShortBreakDuration;
    qint16 LongBreakDuration;
    qint16 SessionAlarmVolume;
    qint16 BreakAlarmVolume;
    qint16 BreakAlarmRepetitions;
    qint16 ShortBreakAfterSessions;
    qint16 LongBreakAfterShortBreaks;

    QKeySequence QuickActionShortcut;
    const QString SessionAlarmsPath = "/data/sounds/sessionAlarms/";
    const QFile DefaultSessionAlarm = QFile(QCoreApplication::applicationDirPath() + "/data/sounds/sessionAlarms/defaultButtonSound.wav");
    const QString BreakAlarmsPath = "/data/sounds/breakAlarms/";
    const QFile DefaultBreakAlarm = QFile(QCoreApplication::applicationDirPath() + "/data/sounds/breakAlarms/defaultEndNotification.wav");
    const QString TempFilesPath = "/data/temp/";
    QFile CurrentSessionAlarm;
    QFile CurrentBreakAlarm;

    bool Notifications;
    bool AlarmSound;
    Skin::SkinTypes Skin;

    QList<QPair<QString,QVariant>> ToData();
    QList<QPair<QString,QVariant>> ToData(const int userID);
    float GetSessionVolumeForAudio();
    float GetBreakVolumeForAudio();
    void SetSessionDuration(double value);
    void SetShortBreakDuration(double value);
    void SetLongBreakDuration(double value);
};


#endif // SETTINGS_H
