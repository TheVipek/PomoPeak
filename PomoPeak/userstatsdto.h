#ifndef USERSTATSDTO_H
#define USERSTATSDTO_H

#include <QJsonObject>
#include <QSqlRecord>
#include <QDate>
#include "daytaskstats.h"
class UserStatsDTO
{
public:
    UserStatsDTO(QString const id, const QMap<QDate,DayTaskStats> data) : UserID(id), Data(data)
        {};

    const QString UserID;
    const QMap<QDate,DayTaskStats> Data;

    static UserStatsDTO FromSqlRecord(const QSqlRecord& record);
};

#endif // USERSTATSDTO_H
