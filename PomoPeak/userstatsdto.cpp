#include "userstatsdto.h"
#include <QJsonDocument>
UserStatsDTO UserStatsDTO::FromSqlRecord(const QSqlRecord& record)
{
    QString userID = record.value("UserID").isNull() ? QString("DefaultUserID") : record.value("UserID").toString();
    QString dataJSON = record.value("TaskStats").isNull() ? QString("") : record.value("TaskStats").toString();
    QJsonObject jsonObject = QJsonDocument::fromJson(dataJSON.toUtf8()).object();
    QMap<QDate, DayTaskStats> taskStats = QMap<QDate,DayTaskStats>();

    for (const QString& key : jsonObject.keys()) {
        QDate date = QDate::fromString(key, Qt::ISODate);
        QJsonObject dayTaskStatsObject = jsonObject.value(key).toObject();
        DayTaskStats dayTaskStats = DayTaskStats(dayTaskStatsObject["TaskCompletedCount"].toInt(), dayTaskStatsObject["TimeSpendInHours"].toDouble());
        taskStats.insert(date, dayTaskStats);
    }

    return UserStatsDTO(userID,taskStats);
}
