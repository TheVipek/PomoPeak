#include "settingsdto.h"
SettingsDTO SettingsDTO::FromSqlRecord(const QSqlRecord& record)
{
    QString userID = record.value("UserID").isNull() ? QString("DefaultUserID") : record.value("UserID").toString();
    qint16 sessionDuration = record.value("SessionDuration").isNull() ? 0 : static_cast<qint16>(record.value("SessionDuration").toInt());
    qint16 shortBreakDuration = record.value("ShortBreakDuration").isNull() ? 0 : static_cast<qint16>(record.value("ShortBreakDuration").toInt());
    qint16 longBreakDuration = record.value("LongBreakDuration").isNull() ? 0 : static_cast<qint16>(record.value("LongBreakDuration").toInt());
    qint16 sessionAlarmVolume = record.value("SessionAlarmVolume").isNull() ? 0 : static_cast<qint16>(record.value("SessionAlarmVolume").toInt());
    qint16 breakAlarmVolume = record.value("BreakAlarmVolume").isNull() ? 0 : static_cast<qint16>(record.value("BreakAlarmVolume").toInt());
    qint16 breakAlarmRepetitions = record.value("BreakAlarmRepetitions").isNull() ? 0 : static_cast<qint16>(record.value("BreakAlarmRepetitions").toInt());\
    qint16 longBreakFrequency = record.value("LongBreakFrequency").isNull() ? 0 : static_cast<qint16>(record.value("LongBreakFrequency").toInt());
    QKeySequence quickActionShortcut = record.value("QuickActionShortcut").isNull() ? QKeySequence() : static_cast<QKeySequence>(record.value("QuickActionShortcut").toString());
    QByteArray sessionAlarm = record.value("SessionAlarm").isNull() ? QByteArray() : record.value("SessionAlarm").toByteArray();
    QString sessionAlarmName = record.value("SessionAlarmName").isNull() ? QString("SessionAlarmName") : record.value("SessionAlarmName").toString();
    QString sessionAlarmExt = record.value("SessionAlarmExt").isNull() ? QString(".") : record.value("SessionAlarmExt").toString();
    QByteArray breakAlarm = record.value("BreakAlarm").isNull() ? QByteArray() : record.value("BreakAlarm").toByteArray();
    QString breakAlarmName = record.value("BreakAlarmName").isNull() ? QString("BreakAlarmName") : record.value("BreakAlarmName").toString();
    QString breakAlarmExt = record.value("BreakAlarmExt").isNull() ? QString(".") : record.value("BreakAlarmExt").toString();
    bool notifications = record.value("Notifications").isNull() ? true : record.value("Notifications").toInt() != 0;
    bool alarmSound = record.value("AlarmSound").isNull() ? true : record.value("AlarmSound").toInt() != 0;
    Skin::SkinTypes skin = record.value("Skin").isNull() ? Skin::SkinTypes::White : static_cast<Skin::SkinTypes>(record.value("Skin").toInt());;

    return SettingsDTO(
        userID,
        sessionDuration,
        shortBreakDuration,
        longBreakDuration ,
        sessionAlarmVolume ,
        breakAlarmVolume ,
        breakAlarmRepetitions ,
        longBreakFrequency,
        quickActionShortcut ,
        sessionAlarm ,
        sessionAlarmName,
        sessionAlarmExt,
        breakAlarm,
        breakAlarmName,
        breakAlarmExt,
        notifications,
        alarmSound,
        skin
    );
};

