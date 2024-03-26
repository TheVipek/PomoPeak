#include "userstats.h"

UserStats::UserStats()
{
    TaskStats = QMap<QDate, DayTaskStats>();
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
void UserStats::AddTimeSpend(int time)
{
    if(TaskStats.contains(QDate::currentDate()))
    {
        TaskStats[QDate::currentDate()].TimeSpend += time;
    }
    else
    {
        TaskStats.insert(QDate::currentDate(), DayTaskStats(0,time));
    }
}

