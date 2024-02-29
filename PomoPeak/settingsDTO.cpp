#include <QtTypes>
#include <QByteArray>
#include <QString>
#include <QVariantMap>>

struct SettingsDTO
{
    qint16 sessionDuration;
    qint16 shortBreakDuration;
    qint16 longBreakDuration;
    QByteArray sessionAlarm;
    qint16 sessionAlarmVolume;
    QByteArray breakAlarm;
    qint16 breakAlarmVolume;
    qint16 breakAlarmRepetitions;
    QString quickActionShortcut;

    static SettingsDTO fromVariantMap(const QVariantMap& map)
    {
        SettingsDTO dto;


        dto.sessionDuration = static_cast<qint16>(map["sessionDuration"].toInt());
        dto.shortBreakDuration = static_cast<qint16>(map["shortBreakDuration"].toInt());
    }
};
