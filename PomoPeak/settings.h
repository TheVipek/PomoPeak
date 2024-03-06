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

    static QString customSessionAlarmName;
    static QString customBreakAlarmName;

    const QString DefaultSessionAlarmPath = "/data/sounds/defaultButtonSound";
    const QString CustomSessionAlarmPath = "/data/sounds/customButtonSound";

    const QString DefaultBreakAlarmPath = "/data/sounds/defaultEndNotification";
    const QString CustomBreakAlarmPath = "/data/sounds/customEndNotification";

    QString currentSessionAlarmExt = ".wav";
    QString currentBreakAlarmExt = ".wav";

    QString SessionAlarm;
    QString SessionAlarmName;
    QString BreakAlarm;
    QString BreakAlarmName;

    QVariantMap ToVariantMap(QString userID);

};

#endif // SETTINGS_H
