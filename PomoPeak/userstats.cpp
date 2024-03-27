#include "userstats.h"

UserStats::UserStats()
{
    TaskStats = QMap<QDate, DayTaskStats>();
}


QMap<QDate,DayTaskStats> UserStats::GetUserStats() const
{
    return TaskStats;
}

void UserStats::AddTaskCompletion(const QDate date)
{
    if(TaskStats.contains(date))
    {
        TaskStats[date].TaskCompletedCount += 1;
    }
    else
    {
        TaskStats.insert(date, DayTaskStats(1,0));
    }
}
void UserStats::AddTimeSpend(const float minutes, const QDate date)
{
    if(TaskStats.contains(date))
    {
        TaskStats[date].TimeSpendInHours += minutes/60;
    }
    else
    {
        TaskStats.insert(date, DayTaskStats(0, minutes/60));
    }
}

