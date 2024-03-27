#include "userstats.h"

UserStats::UserStats()
{
    TaskStats = QMap<QDate, DayTaskStats>();
}


QMap<QDate,DayTaskStats> UserStats::GetUserStats() const
{
    return TaskStats;
}

void UserStats::AddTaskCompletion()
{
    if(TaskStats.contains(QDate::currentDate()))
    {
        TaskStats[QDate::currentDate()].TaskCompletedCount += 1;
    }
    else
    {
        TaskStats.insert(QDate::currentDate(), DayTaskStats(1,0));
    }
}
void UserStats::AddTimeSpend(float minutes)
{
    if(TaskStats.contains(QDate::currentDate()))
    {
        TaskStats[QDate::currentDate()].TimeSpendInHours += minutes/60;
    }
    else
    {
        TaskStats.insert(QDate::currentDate(), DayTaskStats(0, minutes/60));
    }
}

