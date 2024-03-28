#ifndef USERSTATS_H
#define USERSTATS_H

#include "QMap"
#include "QDate"
#include "daytaskstats.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "userstatsdto.h"
class UserStats
{
public:
    UserStats();
    UserStats(const UserStatsDTO& dto);
    QMap<QDate,DayTaskStats> GetUserStats() const;
    void AddTaskCompletion(const QDate date = QDate::currentDate());
    void AddTimeSpend(const float minutes, const QDate date = QDate::currentDate());


    QString ToJson() const
    {
        QJsonObject taskStatsObj;
        for(auto it = TaskStats.constBegin(); it != TaskStats.constEnd(); ++it) {
            taskStatsObj[it.key().toString(Qt::ISODate)] = it.value().ToJson();
        }
        return QJsonDocument(taskStatsObj).toJson();
    }
    QList<QPair<QString,QVariant>> ToData();
    QList<QPair<QString,QVariant>> ToData(const int userID);

private:
    QMap<QDate,DayTaskStats> TaskStats;
};



#endif // USERSTATS_H
