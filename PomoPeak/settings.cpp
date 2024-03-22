#include "settings.h"
#include <QDir>
Settings::Settings()
{
    SetSessionDuration(25);
    SetShortBreakDuration(5);
    SetLongBreakDuration(10);
    SessionAlarmVolume = 100;
    BreakAlarmVolume = 100;
    BreakAlarmRepetitions = 5;
    ShortBreakAfterSessions = 1;
    LongBreakAfterShortBreaks = 2;

    QuickActionShortcut = QKeySequence(Qt::Key_Space);

    CurrentSessionAlarm.setFileName(DefaultSessionAlarm.fileName());
    CurrentBreakAlarm.setFileName(DefaultBreakAlarm.fileName());

    Notifications = true;
    AlarmSound = true;

    Skin = Skin::SkinTypes::White;
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

    Notifications = dto.Notifications;
    AlarmSound = dto.AlarmSound;

    Skin = dto.Skin;
    //Load file from custom session alarm path


    qDebug() << "loading file";
    QFile fileFromDB = QFile(QCoreApplication::applicationDirPath() + TempFilesPath + dto.SessionAlarmName + dto.SessionAlarmExt);
    if(fileFromDB.open(QIODevice::WriteOnly))
    {
        fileFromDB.write(dto.SessionAlarm);
        fileFromDB.close();
    }
    qDebug() << fileFromDB.fileName();
    if(fileFromDB.size() > 0 && fileFromDB.size() != DefaultSessionAlarm.size())
    {
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
                QFile::remove(fileFromDB.fileName());
                qDebug() << "setting existing";
                found = true;
                break;
            }
        }

        if(!found)
        {
            QString from = fileFromDB.fileName();
            QString filename = QFileInfo(fileFromDB).fileName();
            QString to = QDir(QCoreApplication::applicationDirPath() + SessionAlarmsPath).filePath(filename);
            if(!QFile::rename(from, to))
            {
                qDebug() << "deleting to";
                QFile::remove(to);
                QFile::rename(from,to);
            }
            CurrentSessionAlarm.setFileName((to));
        }
    }
    else
    {
        CurrentSessionAlarm.setFileName(DefaultSessionAlarm.fileName());
        QFile::remove(fileFromDB.fileName());
    }

    QFile breakFromDB = QFile(QCoreApplication::applicationDirPath() + TempFilesPath + dto.BreakAlarmName + dto.BreakAlarmExt);
    if(breakFromDB.open(QIODevice::WriteOnly))
    {
        breakFromDB.write(dto.BreakAlarm);
        breakFromDB.close();
    }
    qDebug() << breakFromDB.fileName();
    if(breakFromDB.size() > 0 && breakFromDB.size() != DefaultBreakAlarm.size())
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
                QFile::remove(breakFromDB.fileName());
                qDebug() << "setting existing";
                found = true;
                break;
            }
        }

        if(!found)
        {
            QString from = breakFromDB.fileName();
            QString filename = QFileInfo(breakFromDB).fileName();
            QString to = QDir(QCoreApplication::applicationDirPath() + BreakAlarmsPath).filePath(filename);
            if(!QFile::rename(from, to))
            {
                qDebug() << "deleting to";
                QFile::remove(to);
                QFile::rename(from,to);
            }
            CurrentBreakAlarm.setFileName(to);
        }
    }
    else
    {
        CurrentBreakAlarm.setFileName(DefaultBreakAlarm.fileName());
        QFile::remove(breakFromDB.fileName());
    }


}

QList<QPair<QString,QVariant>> Settings::ToData()
{
    QByteArray currentSessArr;
    QFileInfo sessionAlarmInfo(CurrentSessionAlarm);
    QFileInfo breakAlarmInfo(CurrentBreakAlarm);
    if(CurrentSessionAlarm.open(QIODevice::ReadOnly))
    {
        currentSessArr = CurrentSessionAlarm.readAll();
    }
    QByteArray currentBreakArr;
    if(CurrentBreakAlarm.open(QIODevice::ReadOnly))
    {
        currentBreakArr = CurrentBreakAlarm.readAll();
    }

    QVariant skinVariant = QVariant::fromValue(static_cast<int>(Skin));

    return {
        {"SessionDuration", SessionDuration},
        {"ShortBreakDuration", ShortBreakDuration},
        {"LongBreakDuration", LongBreakDuration},
        {"SessionAlarmVolume", SessionAlarmVolume},
        {"BreakAlarmVolume", BreakAlarmVolume},
        {"BreakAlarmRepetitions", BreakAlarmRepetitions},
        {"ShortBreakAfterSessions", ShortBreakAfterSessions},
        {"LongBreakAfterShortBreaks", LongBreakAfterShortBreaks},
        {"QuickActionShortcut", QuickActionShortcut.toString()},
        {"SessionAlarm",currentSessArr},
        {"SessionAlarmName", sessionAlarmInfo.baseName()},
        {"SessionAlarmExt", "." +sessionAlarmInfo.suffix()},
        {"BreakAlarm", currentBreakArr},
        {"BreakAlarmName", breakAlarmInfo.baseName()},
        {"BreakAlarmExt", "." + breakAlarmInfo.suffix()},
        {"Notifications", Notifications != 0},
        {"AlarmSound", AlarmSound != 0},
        {"Skin", skinVariant}
    };
}

QList<QPair<QString,QVariant>> Settings::ToData(const int userID)
{
    QByteArray currentSessArr;
    QFileInfo sessionAlarmInfo(CurrentSessionAlarm);
    QFileInfo breakAlarmInfo(CurrentBreakAlarm);
    if(CurrentSessionAlarm.open(QIODevice::ReadOnly))
    {
        currentSessArr = CurrentSessionAlarm.readAll();
    }
    QByteArray currentBreakArr;
    if(CurrentBreakAlarm.open(QIODevice::ReadOnly))
    {
        currentBreakArr = CurrentBreakAlarm.readAll();
    }

    QVariant skinVariant = QVariant::fromValue(static_cast<int>(Skin));

    return {
        {"UserID", userID},
        {"SessionDuration", SessionDuration},
        {"ShortBreakDuration", ShortBreakDuration},
        {"LongBreakDuration", LongBreakDuration},
        {"SessionAlarmVolume", SessionAlarmVolume},
        {"BreakAlarmVolume", BreakAlarmVolume},
        {"BreakAlarmRepetitions", BreakAlarmRepetitions},
        {"ShortBreakAfterSessions", ShortBreakAfterSessions},
        {"LongBreakAfterShortBreaks", LongBreakAfterShortBreaks},
        {"QuickActionShortcut", QuickActionShortcut.toString()},
        {"SessionAlarm", currentSessArr},
        {"SessionAlarmName", sessionAlarmInfo.baseName()},
        {"SessionAlarmExt", "." + sessionAlarmInfo.suffix()},
        {"BreakAlarm", currentBreakArr},
        {"BreakAlarmName", breakAlarmInfo.baseName()},
        {"BreakAlarmExt", "." + breakAlarmInfo.suffix()},
        {"Notifications", Notifications != 0},
        {"AlarmSound", AlarmSound != 0},
        {"Skin", skinVariant}
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
void Settings::SetSessionDuration(double value)
{
    SessionDuration = value * 60;
}
void Settings::SetShortBreakDuration(double value)
{
    ShortBreakDuration = value * 60;
}
void Settings::SetLongBreakDuration(double value)
{
    LongBreakDuration = value * 60;
}
