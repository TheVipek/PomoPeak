#include "userstats.h"

UserStats::UserStats()
{
    TaskStats = QMap<QDate, DayTaskStats>();
}
UserStats::UserStats(const UserStatsDTO& stats)
{

    TaskStats = stats.Data;
}

QMap<QDate,DayTaskStats> UserStats::GetUserStats() const
{
    return TaskStats;
}

void UserStats::AddTaskCompletion(const QDate date, int val)
{
    if(TaskStats.contains(date))
    {
        TaskStats[date].TaskCompletedCount += val;
    }
    else
    {
        TaskStats.insert(date, DayTaskStats(val,0));
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

QList<QPair<QString,QVariant>> UserStats::ToData()
{
    return
    {
        {"TaskStats", ToJson()}
    };
}
QList<QPair<QString,QVariant>> UserStats::ToData(const int userID)
{

    return
    {
        {"UserID", userID},
        {"TaskStats", ToJson()}
    };
}
