#ifndef USERSTATS_H
#define USERSTATS_H

#include "QMap"
#include "QDate"
#include "daytaskstats.h"
class UserStats
{
public:
    UserStats();
    QMap<QDate,DayTaskStats> GetUserStats();
    void AddTaskCompletion();
    void AddTimeSpend(int time);
private:
    QMap<QDate,DayTaskStats> TaskStats;
};

#endif // USERSTATS_H
