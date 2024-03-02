#ifndef SETTINGSDTO_H
#define SETTINGSDTO_H
#include <QtTypes>
#include <QKeySequence>
#include <qbytearray.h>
#include <QString>
#include <QSqlRecord>
#include <QVariant>
#include <QVariantMap>
struct SettingsDTO
{
public:
    SettingsDTO(const QString id, const qint16 sessionDur, const qint16 shortBreakDur, const qint16 longBreakDur, const qint16 sessionAlarmVol, const qint16 breakAlarmVol
                , const qint16 breakAlarmRep, const qint16 shortBreakAfterSession, const qint16 longBreakAfterShortBreaks, const QKeySequence shortcut
                , const QByteArray sessionAlarm, const QByteArray breakAlarm)
        : UserID(id), SessionDuration(sessionDur), ShortBreakDuration(shortBreakDur), LongBreakDuration(longBreakDur)
        , SessionAlarmVolume(sessionAlarmVol), BreakAlarmVolume(breakAlarmVol), BreakAlarmRepetitions(breakAlarmRep)
        , ShortBreakAfterSessions(shortBreakAfterSession), LongBreakAfterShortBreaks(longBreakAfterShortBreaks)
        , QuickActionShortcut(shortcut),  SessionAlarm(sessionAlarm), BreakAlarm(breakAlarm)
        {};

    const QString UserID;

    const qint16 SessionDuration;
    const qint16 ShortBreakDuration;
    const qint16 LongBreakDuration;
    const qint16 SessionAlarmVolume;
    const qint16 BreakAlarmVolume;
    const qint16 BreakAlarmRepetitions;
    const qint16 ShortBreakAfterSessions;
    const qint16 LongBreakAfterShortBreaks;

    const QKeySequence QuickActionShortcut;

    const QByteArray SessionAlarm;

    const QByteArray BreakAlarm;

    static SettingsDTO FromSqlRecord(const QSqlRecord& record);
};

#endif // SETTINGSDTO_H