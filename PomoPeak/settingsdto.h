#ifndef SETTINGSDTO_H
#define SETTINGSDTO_H
#include <QtTypes>
#include <QKeySequence>
#include <qbytearray.h>
#include <QString>
#include <QSqlRecord>
#include <QVariant>
#include <QVariantMap>
#include "Skin.h"
struct SettingsDTO
{
public:
    SettingsDTO(const QString id, const qint16 sessionDur, const qint16 shortBreakDur, const qint16 longBreakDur, const qint16 sessionAlarmVol, const qint16 breakAlarmVol
                , const qint16 breakAlarmRep, const qint16 longBreakFrequency, const QKeySequence shortcut
                , const QByteArray sessionAlarm, const QString sessionAlarmName, const QString sessionAlarmExt, const QByteArray breakAlarm, const QString breakAlarmName, const QString breakAlarmExt
                , const bool notifications, const bool alarmSound, const Skin::SkinTypes skin)
        : UserID(id), SessionDuration(sessionDur), ShortBreakDuration(shortBreakDur), LongBreakDuration(longBreakDur)
        , SessionAlarmVolume(sessionAlarmVol), BreakAlarmVolume(breakAlarmVol), BreakAlarmRepetitions(breakAlarmRep)
        , LongBreakFrequency(longBreakFrequency)
        , QuickActionShortcut(shortcut), SessionAlarm(sessionAlarm), SessionAlarmName(sessionAlarmName), SessionAlarmExt(sessionAlarmExt)
        , BreakAlarm(breakAlarm), BreakAlarmName(breakAlarmName), BreakAlarmExt(breakAlarmExt)
        , Notifications(notifications), AlarmSound(alarmSound), Skin(skin)
        {};

    const QString UserID;

    const qint16 SessionDuration;
    const qint16 ShortBreakDuration;
    const qint16 LongBreakDuration;
    const qint16 SessionAlarmVolume;
    const qint16 BreakAlarmVolume;
    const qint16 BreakAlarmRepetitions;
    const qint16 LongBreakFrequency;

    const QKeySequence QuickActionShortcut;

    const QByteArray SessionAlarm;
    const QString SessionAlarmName;
    const QString SessionAlarmExt;
    const QByteArray BreakAlarm;
    const QString BreakAlarmName;
    const QString BreakAlarmExt;
    static SettingsDTO FromSqlRecord(const QSqlRecord& record);

    const bool Notifications;
    const bool AlarmSound;
    const Skin::SkinTypes Skin;

};

#endif // SETTINGSDTO_H
