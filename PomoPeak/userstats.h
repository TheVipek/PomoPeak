#ifndef USERSTATS_H
#define USERSTATS_H

#include "QMap"
#include "QDate"
#include "daytaskstats.h"
#include <QJsonObject>
class UserStats
{
public:
    UserStats();
    QMap<QDate,DayTaskStats> GetUserStats() const;
    void AddTaskCompletion(const QDate date = QDate::currentDate());
    void AddTimeSpend(const float minutes, const QDate date = QDate::currentDate());


    QJsonObject ToJson() const
    {
        QJsonObject obj;
        QJsonObject taskStatsObj;
        for(auto it = TaskStats.constBegin(); it != TaskStats.constEnd(); ++it) {
            taskStatsObj[it.key().toString(Qt::ISODate)] = it.value().ToJson();
        }
        obj["TaskStats"] = taskStatsObj;
        return obj;
    }

private:
    QMap<QDate,DayTaskStats> TaskStats;
};



#endif // USERSTATS_H
