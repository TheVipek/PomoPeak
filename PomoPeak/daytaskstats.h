#ifndef DAYTASKSTATS_H
#define DAYTASKSTATS_H
#include "QJsonObject"
struct DayTaskStats
{
public:
    DayTaskStats();
    DayTaskStats(int taskCompletedCount, int timeSpend);
    int TaskCompletedCount = 0;
    int TimeSpend = 0;

    QJsonObject ToJson() const
    {
        QJsonObject obj;
        obj["TaskCompletedCount"] = TaskCompletedCount;
        obj["TimeSpend"] = TimeSpend;
        return obj;
    }
};

#endif // DAYTASKSTATS_H
