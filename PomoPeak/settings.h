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

    const QString DefaultSessionAlarmPath = "/sounds/defaultButtonSound";
    const QString CustomSessionAlarmPath = "/sounds/customButtonSound";

    const QString DefaultBreakAlarmPath = "/sounds/defaultEndNotification";
    const QString CustomBreakAlarmPath = "/sounds/customBreakNotification";

    QString SessionAlarm;
    QString BreakAlarm;

    QVariantMap ToVariantMap(QString userID);
};

#endif // SETTINGS_H
