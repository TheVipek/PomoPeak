#ifndef DAYTASKSTATS_H
#define DAYTASKSTATS_H

struct DayTaskStats
{
public:
    DayTaskStats();
    DayTaskStats(int taskCompletedCount, int timeSpend);
    int TaskCompletedCount = 0;
    int TimeSpend = 0;
};

#endif // DAYTASKSTATS_H
