#include "settings.h"

QString Settings::customSessionAlarmName =  "customButtonSound";
QString Settings::customBreakAlarmName =  "customBreakNotification";


Settings::Settings()
{
    SessionDuration = 25 * 60;
        ShortBreakDuration = 5 * 60;
    LongBreakDuration = 10 * 60;
    SessionAlarm = DefaultSessionAlarmPath;
    SessionAlarmVolume = 100;
    BreakAlarm = DefaultBreakAlarmPath;
    BreakAlarmVolume = 100;
    BreakAlarmRepetitions = 5;

    ShortBreakAfterSessions = 1;
    LongBreakAfterShortBreaks = 2;

    QuickActionShortcut = QKeySequence(Qt::Key_Space);
}

Settings::Settings(const SettingsDTO& dto)
{
    SessionDuration = dto.SessionDuration;
    ShortBreakDuration = dto.ShortBreakDuration;
    LongBreakDuration = dto.LongBreakDuration;
    SessionAlarmVolume = dto.SessionAlarmVolume;
    BreakAlarmVolume = dto.SessionAlarmVolume;
    BreakAlarmRepetitions = dto.BreakAlarmRepetitions;

    ShortBreakAfterSessions = dto.ShortBreakAfterSessions;
    LongBreakAfterShortBreaks = dto.LongBreakAfterShortBreaks;

    QuickActionShortcut = QKeySequence(dto.QuickActionShortcut);

    QFile currentSession = QFile(QCoreApplication::applicationDirPath() + CustomSessionAlarmPath);
    if(currentSession.exists())
    {
        QByteArray currentSessionData = currentSession.readAll();
        if(!dto.SessionAlarm.isNull() && dto.SessionAlarm.data() != nullptr && currentSessionData != dto.SessionAlarm)
        {
            currentSession.remove();
            QFile newSessionAlarm = QFile(QCoreApplication::applicationDirPath() + CustomSessionAlarmPath);
            newSessionAlarm.write(dto.SessionAlarm);
            if(newSessionAlarm.fileName() != customSessionAlarmName)
            {
                newSessionAlarm.rename(customSessionAlarmName);
            }
        }
        SessionAlarm = CustomSessionAlarmPath;
    }
    else
    {
        SessionAlarm = DefaultSessionAlarmPath;
    }

    QFile currentBreak = QFile(QCoreApplication::applicationDirPath() + CustomBreakAlarmPath);
    if(currentBreak.exists())
    {
        QByteArray currentBreakData = currentBreak.readAll();
        if(!dto.BreakAlarm.isNull() && dto.BreakAlarm.data() != nullptr && currentBreakData != dto.BreakAlarm)
        {
            currentBreak.remove();
            QFile newBreakAlarm = QFile(QCoreApplication::applicationDirPath() + CustomBreakAlarmPath);
            newBreakAlarm.write(dto.BreakAlarm);
            if(newBreakAlarm.fileName() != customBreakAlarmName)
            {
                newBreakAlarm.rename(customBreakAlarmName);
            }

        }
        BreakAlarm = CustomBreakAlarmPath;
    }
    else
    {
        BreakAlarm = DefaultSessionAlarmPath;
    }

}

QVariantMap Settings::ToVariantMap(QString userID)
{
    QByteArray sessionArray;
    QByteArray breakArray;

    if(SessionAlarm == CustomBreakAlarmPath)
    {
        QFile sessionAlarm = QFile(SessionAlarm);
        QFile breakAlarm = QFile(BreakAlarm);

        sessionArray = sessionAlarm.readAll();
        breakArray = breakAlarm.readAll();
    }

    return {
        {"UserID", userID},
        {"SessionDuration", SessionDuration},
        {"ShortBreakDuration", ShortBreakDuration},
        {"LongBreakDuration", LongBreakAfterShortBreaks},
        {"SessionAlarmVolume", SessionAlarmVolume},
        {"BreakAlarmVolume", BreakAlarmVolume},
        {"BreakAlarmRepetitions", BreakAlarmRepetitions},
        {"ShortBreakAfterSessions", ShortBreakAfterSessions},
        {"LongBreakAfterShortBreaks", LongBreakAfterShortBreaks},
        {"QuickActionShortcut", QuickActionShortcut.toString()},
        {"SessionAlarm",sessionArray},
        {"BreakAlarm", breakArray}
    };
};
