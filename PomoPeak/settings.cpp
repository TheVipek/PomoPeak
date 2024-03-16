#include "settings.h"
#include <QDir>
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

    CurrentSessionAlarm.setFileName(DefaultSessionAlarm.fileName());
    CurrentBreakAlarm.setFileName(DefaultBreakAlarm.fileName());
}

Settings::Settings(const SettingsDTO& dto)
{
    SessionDuration = dto.SessionDuration;
    ShortBreakDuration = dto.ShortBreakDuration;
    LongBreakDuration = dto.LongBreakDuration;
    SessionAlarmVolume = dto.SessionAlarmVolume;
    BreakAlarmVolume = dto.BreakAlarmVolume;
    BreakAlarmRepetitions = dto.BreakAlarmRepetitions;

    ShortBreakAfterSessions = dto.ShortBreakAfterSessions;
    LongBreakAfterShortBreaks = dto.LongBreakAfterShortBreaks;

    QuickActionShortcut = QKeySequence(dto.QuickActionShortcut);


    //Load file from custom session alarm path


    qDebug() << "loading file";
    //dto.SessionAlarm
    QFile fileFromDB = QFile(QCoreApplication::applicationDirPath() + TempFilesPath);
    if(fileFromDB.open(QIODevice::WriteOnly))
    {
        fileFromDB.write(dto.SessionAlarm);
        fileFromDB.close();
    }
    if(fileFromDB.exists() && fileFromDB.size() > 0 && fileFromDB.size() != DefaultSessionAlarm.size())
    {
        qDebug() << "exists";
        QDir dir(QCoreApplication::applicationDirPath() + SessionAlarmsPath);
        dir.setFilter(QDir::Files);
        QStringList files = dir.entryList();

        bool found = false;
        for(const QString& fileInDir : files)
        {
            QFile file(dir.filePath(fileInDir));
            if(file.size() == fileFromDB.size())
            {
                CurrentSessionAlarm.setFileName(file.fileName());
                qDebug() << "setting existing";
                found = true;
                break;
            }
        }

        if(!found)
        {
            qDebug() << "creating new one";
            QString from = fileFromDB.fileName();
            QString filename = QFileInfo(fileFromDB).fileName();
            QString to = QDir(QCoreApplication::applicationDirPath() + SessionAlarmsPath).filePath(filename);
            QFile::rename(from, to);
        }
    }
    else
    {
        CurrentSessionAlarm.setFileName(DefaultSessionAlarm.fileName());
    }

    QFile breakFromDB = QFile(dto.BreakAlarm);
    if(breakFromDB.exists() && breakFromDB.size() > 0 && breakFromDB.size() != DefaultBreakAlarm.size())
    {
        QDir dir(QCoreApplication::applicationDirPath() + BreakAlarmsPath);
        dir.setFilter(QDir::Files);
        QStringList files = dir.entryList();

        bool found = false;
        for(const QString& fileInDir : files)
        {
            QFile file(dir.filePath(fileInDir));
            if(file.size() == fileFromDB.size())
            {
                CurrentBreakAlarm.setFileName(file.fileName());
                found = true;
                break;
            }
        }

        if(!found)
        {
            QString from = breakFromDB.fileName();
            QString filename = QFileInfo(breakFromDB).fileName();
            QString to = QDir(QCoreApplication::applicationDirPath() + BreakAlarmsPath).filePath(filename);
            QFile::rename(from, to);
        }
    }
    else
    {
        CurrentBreakAlarm.setFileName(DefaultBreakAlarm.fileName());
    }
}

QList<QPair<QString,QVariant>> Settings::ToData()
{
    QByteArray currentSessArr;
    if(CurrentSessionAlarm.open(QIODevice::ReadOnly))
    {
        currentSessArr = CurrentSessionAlarm.readAll();
    }
    QByteArray currentBreakArr;
    if(CurrentBreakAlarm.open(QIODevice::ReadOnly))
    {
        currentBreakArr = CurrentBreakAlarm.readAll();
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
        {"SessionAlarm",currentSessArr},
        {"BreakAlarm", currentBreakArr}
    };
}

QList<QPair<QString,QVariant>> Settings::ToData(const int userID)
{
    QByteArray currentSessArr;
    if(CurrentSessionAlarm.open(QIODevice::ReadOnly))
    {
        currentSessArr = CurrentSessionAlarm.readAll();
    }
    QByteArray currentBreakArr;
    if(CurrentBreakAlarm.open(QIODevice::ReadOnly))
    {
        currentBreakArr = CurrentBreakAlarm.readAll();
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
        {"SessionAlarm", currentSessArr},
        {"BreakAlarm", currentBreakArr}
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
