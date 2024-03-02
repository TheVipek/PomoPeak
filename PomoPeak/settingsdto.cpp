#include "settingsdto.h"
SettingsDTO SettingsDTO::FromSqlRecord(const QSqlRecord& record)
{
    return SettingsDTO(
        record.value("UserID").toString(),
        static_cast<qint16>(record.value("SessionDuration").toInt()),
        static_cast<qint16>(record.value("ShortBreakDuration").toInt()),
        static_cast<qint16>(record.value("LongBreakDuration").toInt()),
        static_cast<qint16>(record.value("SessionAlarmVolume").toInt()),
        static_cast<qint16>(record.value("BreakAlarmVolume").toInt()),
        static_cast<qint16>(record.value("BreakAlarmRepetitions").toInt()),
        static_cast<qint16>(record.value("ShortBreakAfterSessions").toInt()),
        static_cast<qint16>(record.value("LongBreakAfterShortBreaks").toInt()),
        static_cast<QKeySequence>(record.value("QuickActionShortcut").toString()),
        record.value("SessionAlarm").toByteArray(),
        record.value("BreakAlarm").toByteArray()
    );
};

