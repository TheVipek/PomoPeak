#include "settings.h"

QString Settings::customSessionAlarmName =  "customButtonSound";
QString Settings::customBreakAlarmName =  "customBreakNotification";


Settings::Settings()
{
    SessionDuration = 25 * 60;
    ShortBreakDuration = 5 * 60;
    LongBreakDuration = 10 * 60;
    SessionAlarmVolume = 100;
    BreakAlarmVolume = 100;
    BreakAlarmRepetitions = 5;
    ShortBreakAfterSessions = 1;
    LongBreakAfterShortBreaks = 2;

    QuickActionShortcut = QKeySequence(Qt::Key_Space);

    CurrentSessionAlarmExt = ".wav";
    CurrentBreakAlarmExt = ".wav";

    SessionAlarm = QCoreApplication::applicationDirPath() + DefaultSessionAlarmPath;
    SessionAlarmName = SessionAlarm.section('/', -1);

    BreakAlarm = QCoreApplication::applicationDirPath() + DefaultBreakAlarmPath;
    BreakAlarmName = BreakAlarm.section('/', -1);
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



    //Load file from custom session alarm path
    QFile currentSession = QFile(QCoreApplication::applicationDirPath() + CustomSessionAlarmPath);
    if(currentSession.exists())
    {
        QByteArray currentSessionData = currentSession.readAll();

        //compare it with file from dto
        if(!dto.SessionAlarm.isNull() && dto.SessionAlarm.data() != nullptr && currentSessionData != dto.SessionAlarm)
        {
            //if its different, update
            currentSession.remove();
            QFile newSessionAlarm = QFile(QCoreApplication::applicationDirPath() + CustomSessionAlarmPath);
            newSessionAlarm.write(dto.SessionAlarm);
            if(newSessionAlarm.fileName() != customSessionAlarmName)
            {
                newSessionAlarm.rename(customSessionAlarmName);
            }
        }
        //set session alarm to custom path
        SessionAlarm = CustomSessionAlarmPath;
        CurrentSessionAlarmExt = dto.SessionAlarmExt;
    }
    else
    {
        //set session alarm to default path
        SessionAlarm = DefaultSessionAlarmPath;
        CurrentSessionAlarmExt = ".wav";
    }


    //Load file from custom break alarm path
    QFile currentBreak = QFile(QCoreApplication::applicationDirPath() + CustomBreakAlarmPath);
    if(currentBreak.exists())
    {
        QByteArray currentBreakData = currentBreak.readAll();

        //compare it with file from dto
        if(!dto.BreakAlarm.isNull() && dto.BreakAlarm.data() != nullptr && currentBreakData != dto.BreakAlarm)
        {
            //if its different, update
            currentBreak.remove();
            QFile newBreakAlarm = QFile(QCoreApplication::applicationDirPath() + CustomBreakAlarmPath);
            newBreakAlarm.write(dto.BreakAlarm);
            if(newBreakAlarm.fileName() != customBreakAlarmName)
            {
                newBreakAlarm.rename(customBreakAlarmName);
            }

        }
        //set break alarm to custom path
        BreakAlarm = CustomBreakAlarmPath;
        CurrentBreakAlarmExt = dto.BreakAlarmExt;
    }
    else
    {
        //set reak alarm to defaul break path
        BreakAlarm = DefaultBreakAlarmPath;
        CurrentBreakAlarmExt = ".wav";
    }

    SessionAlarmName = SessionAlarm.section('/', -1);
    BreakAlarmName = BreakAlarm.section('/', -1);
}

QList<QPair<QString,QVariant>> Settings::ToData()
{
    QByteArray sessionArray;
    QByteArray breakArray;

    if(SessionAlarm == CustomSessionAlarmPath)
    {
        QFile sessionAlarm = QFile(SessionAlarm);
        sessionArray = sessionAlarm.readAll();
    }
    if(BreakAlarm == CustomBreakAlarmPath)
    {
        QFile breakAlarm = QFile(BreakAlarm);
        breakArray = breakAlarm.readAll();
    }
    return {
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
}

QList<QPair<QString,QVariant>> Settings::ToData(const int userID)
{
    QByteArray sessionArray;
    QByteArray breakArray;

    if(SessionAlarm == CustomSessionAlarmPath)
    {
        QFile sessionAlarm = QFile(SessionAlarm);
        sessionArray = sessionAlarm.readAll();
    }
    if(BreakAlarm == CustomBreakAlarmPath)
    {
         QFile breakAlarm = QFile(BreakAlarm);
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

float Settings::GetSessionVolumeForAudio()
{
    return (float)SessionAlarmVolume / 100;
}
float Settings::GetBreakVolumeForAudio()
{
    return (float)BreakAlarmVolume / 100;
}
