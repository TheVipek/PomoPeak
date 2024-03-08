#include "settingsdto.h"
SettingsDTO SettingsDTO::FromSqlRecord(const QSqlRecord& record)
{
    QString userID = record.value("UserID").isNull() ? QString("DefaultUserID") : record.value("UserID").toString();
    qint16 sessionDuration = record.value("SessionDuration").isNull() ? 0 : static_cast<qint16>(record.value("SessionDuration").toInt());
    qint16 shortBreakDuration = record.value("ShortBreakDuration").isNull() ? 0 : static_cast<qint16>(record.value("ShortBreakDuration").toInt());
    qint16 longBreakDuration = record.value("LongBreakDuration").isNull() ? 0 : static_cast<qint16>(record.value("LongBreakDuration").toInt());
    qint16 sessionAlarmVolume = record.value("SessionAlarmVolume").isNull() ? 0 : static_cast<qint16>(record.value("SessionAlarmVolume").toInt());
    qint16 breakAlarmVolume = record.value("BreakAlarmVolume").isNull() ? 0 : static_cast<qint16>(record.value("BreakAlarmVolume").toInt());
    qint16 breakAlarmRepetitions = record.value("BreakAlarmRepetitions").isNull() ? 0 : static_cast<qint16>(record.value("BreakAlarmRepetitions").toInt());
    qint16 shortBreakAfterSessions = record.value("ShortBreakAfterSessions").isNull() ? 0 : static_cast<qint16>(record.value("ShortBreakAfterSessions").toInt());
    qint16 longBreakAfterShortBreaks = record.value("LongBreakAfterShortBreaks").isNull() ? 0 : static_cast<qint16>(record.value("LongBreakAfterShortBreaks").toInt());
    QKeySequence quickActionShortcut = record.value("QuickActionShortcut").isNull() ? QKeySequence() : static_cast<QKeySequence>(record.value("QuickActionShortcut").toString());
    QByteArray sessionAlarm = record.value("SessionAlarm").isNull() ? QByteArray() : record.value("SessionAlarm").toByteArray();
    QString sessionAlarmExt = record.value("SessionAlarmExt").isNull() ? QString("") : record.value("SessionAlarmExt").toString();
    QByteArray breakAlarm = record.value("BreakAlarm").isNull() ? QByteArray() : record.value("BreakAlarm").toByteArray();
    QString breakAlarmExt = record.value("BreakAlarmExt").isNull() ? QString("") : record.value("BreakAlarmExt").toString();

    return SettingsDTO(
        userID,
        sessionDuration,
        shortBreakDuration,
        longBreakDuration ,
        sessionAlarmVolume ,
        breakAlarmVolume ,
        breakAlarmRepetitions ,
        shortBreakAfterSessions ,
        longBreakAfterShortBreaks ,
        quickActionShortcut ,
        sessionAlarm ,
        sessionAlarmExt ,
        breakAlarm ,
        breakAlarmExt
    );
};

