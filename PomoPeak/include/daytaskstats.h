#ifndef DAYTASKSTATS_H
#define DAYTASKSTATS_H
#include "QJsonObject"
struct DayTaskStats
{
public:
    DayTaskStats();
    DayTaskStats(int taskCompletedCount, float timeSpendInHours);
    int TaskCompletedCount = 0;
    float TimeSpendInHours = 0;

    QJsonObject ToJson() const
    {
        QJsonObject obj;
        obj["TaskCompletedCount"] = TaskCompletedCount;
        obj["TimeSpendInHours"] = TimeSpendInHours;
        return obj;
    }
};

#endif // DAYTASKSTATS_H
